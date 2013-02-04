#include "StdAfx.h"
#include "HTTPContent.h"

#define MAX_CONTENT_SIZE	(1L*1024*1024)
#define GB2312_CODEPAGE		(936)

CHTTPContent::CHTTPContent()
	:siteAdr(L"")
	,status(0)
	,buffUsed(0)
{
	buff = new BYTE[MAX_CONTENT_SIZE];

	// load a cdn list for testing
#ifdef CDN_SWITCH
	CStdioFile cdnFile(L"cdns.txt", CFile::modeRead);

	while(TRUE)
	{
		CString str;
		if(cdnFile.ReadString(str) == TRUE)
		{
			cdnlist.Add(str);
		}
		else
		{
			break;
		}
	}
	cdnFile.Close();
#endif
}

CHTTPContent::~CHTTPContent(void)
{
	if(buff)
	{
		delete []buff;
		buff = NULL;
	}

	if(status == 1)
	{
		WinHttpCloseHandle(hConnect);
		WinHttpCloseHandle(hSession);
		hConnect = NULL;
		hSession = NULL;
	}
	buffUsed = 0;

	cdnlist.RemoveAll();
	/*while(!cdnlist.IsEmpty())
	{
		CString* str = cdnlist.GetAt(0);
		cdnlist.RemoveAt(0);
		delete str;
	}*/
}

BOOL CHTTPContent::SendDatabyGet( const CString& URL )
{
	SwitchSite();

	HINTERNET hRequest = 0;

	if (status == -1)
		goto SENDGETDATAFAIL;

	buffUsed = 0;

	// Create an HTTP request handle
	hRequest = WinHttpOpenRequest( hConnect, L"GET",
			URL,
			NULL, 
			WINHTTP_NO_REFERER,
			WINHTTP_DEFAULT_ACCEPT_TYPES,
#ifdef HTTPS_12306
			WINHTTP_FLAG_SECURE
#else
			WINHTTP_FLAG_REFRESH
#endif
			);

	if(hRequest == NULL)
		goto SENDGETDATAFAIL;

#ifdef HTTPS_12306
	DWORD queryFlags;
	DWORD flagSize = sizeof(queryFlags);
	if(FALSE == WinHttpQueryOption (hRequest, WINHTTP_OPTION_SECURITY_FLAGS,
		(LPVOID)&queryFlags, &flagSize))
		goto SENDGETDATAFAIL;

	queryFlags |= SECURITY_FLAG_IGNORE_UNKNOWN_CA;
	queryFlags |= SECURITY_FLAG_IGNORE_CERT_DATE_INVALID;
	queryFlags |= SECURITY_FLAG_IGNORE_CERT_CN_INVALID;

	queryFlags &= ~SECURITY_FLAG_SECURE;
	queryFlags &= ~SECURITY_FLAG_STRENGTH_WEAK;
	queryFlags &= ~SECURITY_FLAG_STRENGTH_MEDIUM;
	queryFlags &= ~SECURITY_FLAG_STRENGTH_STRONG;

	if(FALSE == WinHttpSetOption (hRequest, WINHTTP_OPTION_SECURITY_FLAGS,
		&queryFlags, sizeof (flagSize) ))
		goto SENDGETDATAFAIL;
#endif

	if(FALSE == WinHttpAddRequestHeaders( hRequest, 
		_T("Cache-Control: no-cache"),
		-1,
		WINHTTP_ADDREQ_FLAG_ADD) )
		goto SENDGETDATAFAIL;

	if(refStr.IsEmpty() == FALSE)
	{
		CString host_ref;
#ifdef HTTPS_12306
		host_ref.Format(L"Referer: https://dynamic.12306.cn%s", refStr);
#else
		host_ref.Format(L"Referer: http://dynamic.12306.cn%s", refStr);
#endif
		if(FALSE == WinHttpAddRequestHeaders( hRequest, 
			host_ref,
			-1,
			WINHTTP_ADDREQ_FLAG_ADD) )
			goto SENDGETDATAFAIL;
	}

#ifdef CDN_SWITCH
	// test: replace host field
	if(FALSE == WinHttpAddRequestHeaders( hRequest, 
		_T("Host: dynamic.12306.cn"),
		-1,
		0) )	// must be zero
		goto SENDGETDATAFAIL;
#endif

	// disable cookie
	// WINHTTP_DISABLE_COOKIES
	DWORD cookieFlags;
	DWORD cookieFlagSize = sizeof(cookieFlags);
	cookieFlags = WINHTTP_DISABLE_COOKIES;
	if(FALSE == WinHttpSetOption (hRequest, WINHTTP_OPTION_DISABLE_FEATURE,
		&cookieFlags, sizeof (cookieFlagSize) ))
		goto SENDGETDATAFAIL;

	// add cookie if there is any
	if(!cookieStr.IsEmpty())
	{
		CString tmpStr;
		tmpStr.Format(L"Cookie: %s", cookieStr);
		if(FALSE == WinHttpAddRequestHeaders( hRequest, 
			tmpStr,
			-1,
			WINHTTP_ADDREQ_FLAG_ADD) )
			goto SENDGETDATAFAIL;
	}

	// Send a request
	if( FALSE == WinHttpSendRequest( hRequest,
			WINHTTP_NO_ADDITIONAL_HEADERS,
			0, NULL, 0, 0, 0))
		goto SENDGETDATAFAIL;

	if(FALSE == WinHttpReceiveResponse( hRequest, NULL) )
		goto SENDGETDATAFAIL;

	// try to get status code
	TCHAR statuscode[128];
	DWORD statussize = 128;
	if(FALSE == WinHttpQueryHeaders(hRequest, WINHTTP_QUERY_STATUS_CODE, WINHTTP_HEADER_NAME_BY_INDEX,
		statuscode, &statussize, WINHTTP_NO_HEADER_INDEX))
		goto SENDGETDATAFAIL;
	if(_tstol(statuscode) != 200)
	{
		// see who will fall into this branch?
		int i = 10;
	}

	// try to get cookie
	GetCookie(hRequest);

	

	// Keep checking for data until there is nothing left.
	DWORD availSize = 0;
	DWORD dwDownloaded = 0;
	DWORD buffPostion = 0;	// writer position of the buffer

	do 
	{
		// Check for available data.
		if (FALSE == WinHttpQueryDataAvailable( hRequest, &availSize))
			goto SENDGETDATAFAIL;

		// buffer overflow
		if(buffPostion + availSize > MAX_CONTENT_SIZE)
			goto SENDGETDATAFAIL;

		// Read the Data.
		if (FALSE == WinHttpReadData( hRequest, (LPVOID)(buff + buffPostion), 
			availSize, &dwDownloaded))
		{
			goto SENDGETDATAFAIL;
		}
		else
		{
			buffPostion += dwDownloaded;
		}
	} while (dwDownloaded > 0);

	buffUsed = buffPostion;

	WinHttpCloseHandle(hRequest);
	status = 1;
	return TRUE;

SENDGETDATAFAIL:
	if(hRequest)
		WinHttpCloseHandle(hRequest);
	status = -1;
	return FALSE;
}

BOOL CHTTPContent::SendDatabyPost( const CString& URL, const CString& additionalData )
{
	SwitchSite();

	HINTERNET hRequest = 0;

	if (status == -1)
		goto SENDGETDATAFAIL;

	buffUsed = 0;

	// Create an HTTP request handle
	hRequest = WinHttpOpenRequest( hConnect, L"POST",
		URL,
		NULL, 
		WINHTTP_NO_REFERER,
		WINHTTP_DEFAULT_ACCEPT_TYPES,
#ifdef HTTPS_12306
		WINHTTP_FLAG_SECURE
#else
		WINHTTP_FLAG_REFRESH
#endif
		);

	if(hRequest == NULL)
		goto SENDGETDATAFAIL;
#ifdef HTTPS_12306
	DWORD queryFlags;
	DWORD flagSize = sizeof(queryFlags);
	if(FALSE == WinHttpQueryOption (hRequest, WINHTTP_OPTION_SECURITY_FLAGS,
		(LPVOID)&queryFlags, &flagSize))
		goto SENDGETDATAFAIL;

	queryFlags |= SECURITY_FLAG_IGNORE_UNKNOWN_CA;
	queryFlags |= SECURITY_FLAG_IGNORE_CERT_DATE_INVALID;
	queryFlags |= SECURITY_FLAG_IGNORE_CERT_CN_INVALID;

	queryFlags &= ~SECURITY_FLAG_SECURE;
	queryFlags &= ~SECURITY_FLAG_STRENGTH_WEAK;
	queryFlags &= ~SECURITY_FLAG_STRENGTH_MEDIUM;
	queryFlags &= ~SECURITY_FLAG_STRENGTH_STRONG;

	if(FALSE == WinHttpSetOption (hRequest, WINHTTP_OPTION_SECURITY_FLAGS,
		&queryFlags, sizeof (flagSize) ))
		goto SENDGETDATAFAIL;
#endif

	// add additional headers
	if(FALSE == WinHttpAddRequestHeaders( hRequest, 
		_T("Content-Type: application/x-www-form-urlencoded"),
		-1,
		WINHTTP_ADDREQ_FLAG_ADD) )
		goto SENDGETDATAFAIL;

	if(FALSE == WinHttpAddRequestHeaders( hRequest, 
		_T("Cache-Control: no-cache"),
		-1,
		WINHTTP_ADDREQ_FLAG_ADD) )
		goto SENDGETDATAFAIL;

#ifdef CDN_SWITCH
	// test: replace host field
	if(FALSE == WinHttpAddRequestHeaders( hRequest, 
		_T("Host: dynamic.12306.cn"),
		-1,
		0) )	// must be zero
		goto SENDGETDATAFAIL;
#endif

	//if(FALSE == WinHttpAddRequestHeaders( hRequest, 
	//	_T("x-requested-with: XMLHttpRequest"),
	//	-1,
	//	WINHTTP_ADDREQ_FLAG_ADD) )
	//	goto SENDGETDATAFAIL;

	// Accept: image/gif, image/jpeg, image/pjpeg, image/pjpeg, application/x-shockwave-flash, application/xaml+xml, application/vnd.ms-xpsdocument, application/x-ms-xbap, application/x-ms-application, application/vnd.ms-excel, application/vnd.ms-powerpoint, application/msword, */*
	//if(FALSE == WinHttpAddRequestHeaders( hRequest, 
	//	_T("Accept: image/gif, image/jpeg, image/pjpeg, image/pjpeg, application/x-shockwave-flash, application/xaml+xml, application/vnd.ms-xpsdocument, application/x-ms-xbap, application/x-ms-application, application/vnd.ms-excel, application/vnd.ms-powerpoint, application/msword, */*"),
	//	-1,
	//	WINHTTP_ADDREQ_FLAG_ADD) )
	//	goto SENDGETDATAFAIL;

	// Referer: https://dynamic.12306.cn/otsweb/loginAction.do?method=init
	if(refStr.IsEmpty() == FALSE)
	{
		CString host_ref;
#ifdef HTTPS_12306
		host_ref.Format(L"Referer: https://dynamic.12306.cn%s", refStr);
#else
		host_ref.Format(L"Referer: http://dynamic.12306.cn%s", refStr);
#endif
		if(FALSE == WinHttpAddRequestHeaders( hRequest, 
			host_ref,
			-1,
			WINHTTP_ADDREQ_FLAG_ADD) )
			goto SENDGETDATAFAIL;
	}
	

	// Accept-Language: en-us
	//if(FALSE == WinHttpAddRequestHeaders( hRequest, 
	//	_T("Accept-Language: en-us"),
	//	-1,
	//	WINHTTP_ADDREQ_FLAG_ADD) )
	//	goto SENDGETDATAFAIL;

	// Accept-Encoding: gzip, deflate
	//if(FALSE == WinHttpAddRequestHeaders( hRequest, 
	//	_T("Accept-Encoding: gzip, deflate"),
	//	-1,
	//	WINHTTP_ADDREQ_FLAG_ADD) )
	//	goto SENDGETDATAFAIL;

	// disable cookie
	// WINHTTP_DISABLE_COOKIES
	DWORD cookieFlags;
	DWORD cookieFlagSize = sizeof(cookieFlags);
	cookieFlags = WINHTTP_DISABLE_COOKIES;
	if(FALSE == WinHttpSetOption (hRequest, WINHTTP_OPTION_DISABLE_FEATURE,
		&cookieFlags, sizeof (cookieFlagSize) ))
		goto SENDGETDATAFAIL;

	// add cookie if there is any
	if(!cookieStr.IsEmpty())
	{
		CString tmpStr;
		tmpStr.Format(L"Cookie: %s", cookieStr);
		if(FALSE == WinHttpAddRequestHeaders( hRequest, 
			tmpStr,
			-1,
			WINHTTP_ADDREQ_FLAG_ADD) )
			goto SENDGETDATAFAIL;
	}

	// transform post string to MBCS type
	char* postData = NULL;
	int postDataSize = 0;
	PCTSTR addDataBuf = additionalData.GetString();
	int addDataSize = additionalData.GetLength();
	postDataSize = WideCharToMultiByte(GB2312_CODEPAGE, 0, addDataBuf, addDataSize, NULL, 0, 0, 0);
	if(postDataSize > 0)
	{
		postData = new char[postDataSize + 1];
		ZeroMemory(postData, (postDataSize + 1)*sizeof(char));
		WideCharToMultiByte(GB2312_CODEPAGE, 0, addDataBuf, addDataSize, postData, postDataSize, 0, 0);
	}

	// Send a request
	if( FALSE == WinHttpSendRequest( hRequest,
		WINHTTP_NO_ADDITIONAL_HEADERS,
		0, postData, postDataSize, postDataSize, 0))
		goto SENDGETDATAFAIL;

	if(postDataSize > 0)
		delete []postData;

	if(FALSE == WinHttpReceiveResponse( hRequest, NULL) )
		goto SENDGETDATAFAIL;

	GetCookie(hRequest);

	// Keep checking for data until there is nothing left.
	DWORD availSize = 0;
	DWORD dwDownloaded = 0;
	DWORD buffPostion = 0;	// writer position of the buffer

	do 
	{
		// Check for available data.
		if (FALSE == WinHttpQueryDataAvailable( hRequest, &availSize))
			goto SENDGETDATAFAIL;

		// buffer overflow
		if(buffPostion + availSize > MAX_CONTENT_SIZE)
			goto SENDGETDATAFAIL;

		// Read the Data.
		if (FALSE == WinHttpReadData( hRequest, (LPVOID)(buff + buffPostion), 
			availSize, &dwDownloaded))
		{
			goto SENDGETDATAFAIL;
		}
		else
		{
			buffPostion += dwDownloaded;
		}
	} while (dwDownloaded > 0);

	buffUsed = buffPostion;

	WinHttpCloseHandle(hRequest);
	status = 1;
	return TRUE;

SENDGETDATAFAIL:
	if(hRequest)
		WinHttpCloseHandle(hRequest);
	status = -1;
	return FALSE;
}

void CHTTPContent::GetResponseStr( CString& result )
{
	result.Empty();
	if(status == -1 || buffUsed == 0)
		return;

	LPTSTR unicodeBuf = NULL;
	int unicodeBufSize = MultiByteToWideChar(CP_UTF8, 0, (LPCSTR)buff, buffUsed, unicodeBuf, 0);
	if(unicodeBufSize > 0)
	{
		LPTSTR unicodeBuf = result.GetBuffer(unicodeBufSize + 1);
		MultiByteToWideChar(CP_UTF8, 0, (LPCSTR)buff, buffUsed, unicodeBuf, unicodeBufSize);
		result.ReleaseBuffer(unicodeBufSize);
	}
}

void CHTTPContent::GetResponseRaw( BYTE* result, DWORD& size )
{
	if(status == -1 || buffUsed == 0)
		return;

	//size = (size < buffUsed) ? size : buffUsed;
	size = buffUsed;
	memcpy(result, buff, size );
}

void CHTTPContent::ConnectInit( const CString& site )
{
	CleanCookie();
	ConnectSite(site);
}

void CHTTPContent::SwitchSite()
{
#ifdef CDN_SWITCH
	static int cdn_index = 0;
	if(cdn_index >= cdnlist.GetCount())
		cdn_index = 0;
	ConnectSite(cdnlist[cdn_index++]);
#endif
}

void CHTTPContent::SetRefStr( CString& str )
{
	refStr = str;
}

void CHTTPContent::CleanCookie()
{
	cookieStr.Empty();
}

void CHTTPContent::ConnectSite( const CString& site )
{
	siteAdr = site;

	// Use WinHttpOpen to obtain a session handle.
	hSession = WinHttpOpen( L"Mozilla/5.0 (compatible; MSIE 9.0; Windows NT 6.1; Trident/5.0)",  
		WINHTTP_ACCESS_TYPE_DEFAULT_PROXY,
		WINHTTP_NO_PROXY_NAME, 
		WINHTTP_NO_PROXY_BYPASS, 0);

	if (hSession == NULL)
		status = -1;

	hConnect = WinHttpConnect( hSession, siteAdr,
#ifdef HTTPS_12306
		INTERNET_DEFAULT_HTTPS_PORT, 
#else
		INTERNET_DEFAULT_HTTP_PORT,
#endif
		0);

	if(hConnect == NULL)
		status = -1;

	status = 1;
}

void CHTTPContent::GetCookie( HINTERNET hRequest )
{
	TCHAR cookie[128], cookie2[128];
	DWORD cookiesize = 128;
	TCHAR cookieheader[128] = L"Set-Cookie:";
	DWORD cookieindex = 0;
	if(cookieStr.IsEmpty())
	{
		CString tmpStr, tmpStr2;
		// store cookie
		if( (TRUE == WinHttpQueryHeaders(hRequest, WINHTTP_QUERY_CUSTOM, cookieheader, cookie, &cookiesize, &cookieindex))
			&&
			(TRUE == WinHttpQueryHeaders(hRequest, WINHTTP_QUERY_CUSTOM, cookieheader, cookie2, &cookiesize, &cookieindex))
			)
		{
			// merge cookies together
			tmpStr += cookie;
			tmpStr2 += cookie2;
			int pos = tmpStr.ReverseFind(L';');
			int pos2 = tmpStr2.ReverseFind(L';');
			if( (pos != -1) && (pos != -1) )
			{
				tmpStr = tmpStr.Left(pos);
				tmpStr2 = tmpStr2.Left(pos2);
				cookieStr.Format(L"%s; %s", tmpStr, tmpStr2);
			}
		}
	}
}
