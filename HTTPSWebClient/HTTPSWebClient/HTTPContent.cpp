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
}

BOOL CHTTPContent::SendDatabyGet( const CString& URL )
{
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
			WINHTTP_FLAG_SECURE
			);

	if(hRequest == NULL)
		goto SENDGETDATAFAIL;

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

	if(FALSE == WinHttpAddRequestHeaders( hRequest, 
		_T("Cache-Control: no-cache"),
		-1,
		WINHTTP_ADDREQ_FLAG_ADD) )
		goto SENDGETDATAFAIL;

	if(refStr.IsEmpty() == FALSE)
	{
		if(FALSE == WinHttpAddRequestHeaders( hRequest, 
			refStr,
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
		WINHTTP_FLAG_SECURE
		);

	if(hRequest == NULL)
		goto SENDGETDATAFAIL;

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
		if(FALSE == WinHttpAddRequestHeaders( hRequest, 
			refStr,
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
		INTERNET_DEFAULT_HTTPS_PORT, 0);

	if(hConnect == NULL)
		status = -1;

	status = 1;
}

void CHTTPContent::SetRefStr( CString& str )
{
	refStr = str;
}
