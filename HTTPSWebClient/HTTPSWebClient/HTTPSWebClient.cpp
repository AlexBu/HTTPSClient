
// HTTPSWebClient.cpp : ����Ӧ�ó��������Ϊ��
//

#include "stdafx.h"
#include "HTTPSWebClient.h"
#include "HTTPSWebClientDlg.h"

#include "jpeglib.h"
#include "jpg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CHTTPSWebClientApp

BEGIN_MESSAGE_MAP(CHTTPSWebClientApp, CWinAppEx)
	ON_COMMAND(ID_HELP, &CWinApp::OnHelp)
END_MESSAGE_MAP()


// CHTTPSWebClientApp ����

CHTTPSWebClientApp::CHTTPSWebClientApp()
{
	// TODO: �ڴ˴���ӹ�����룬
	// ��������Ҫ�ĳ�ʼ�������� InitInstance ��
}


// Ψһ��һ�� CHTTPSWebClientApp ����

CHTTPSWebClientApp theApp;


// CHTTPSWebClientApp ��ʼ��

BOOL CHTTPSWebClientApp::InitInstance()
{
	// ���һ�������� Windows XP �ϵ�Ӧ�ó����嵥ָ��Ҫ
	// ʹ�� ComCtl32.dll �汾 6 ����߰汾�����ÿ��ӻ���ʽ��
	//����Ҫ InitCommonControlsEx()�����򣬽��޷��������ڡ�
	INITCOMMONCONTROLSEX InitCtrls;
	InitCtrls.dwSize = sizeof(InitCtrls);
	// ��������Ϊ��������Ҫ��Ӧ�ó�����ʹ�õ�
	// �����ؼ��ࡣ
	InitCtrls.dwICC = ICC_WIN95_CLASSES;
	InitCommonControlsEx(&InitCtrls);

	CWinAppEx::InitInstance();

	// ��׼��ʼ��
	// ���δʹ����Щ���ܲ�ϣ����С
	// ���տ�ִ���ļ��Ĵ�С����Ӧ�Ƴ�����
	// ����Ҫ���ض���ʼ������
	// �������ڴ洢���õ�ע�����
	// TODO: Ӧ�ʵ��޸ĸ��ַ�����
	// �����޸�Ϊ��˾����֯��
	//SetRegistryKey(_T("Ӧ�ó��������ɵı���Ӧ�ó���"));

	hConnect = NULL;
	hSession = NULL;
	htmlResponceBuff = new BYTE[MAX_JPG_SIZE];
	picBuff = new BYTE[MAX_JPG_SIZE];

	CHTTPSWebClientDlg dlg;
	m_pMainWnd = &dlg;
	INT_PTR nResponse = dlg.DoModal();
	if (nResponse == IDOK)
	{
		// TODO: �ڴ˷��ô����ʱ��
		//  ��ȷ�������رնԻ���Ĵ���
	}
	else if (nResponse == IDCANCEL)
	{
		// TODO: �ڴ˷��ô����ʱ��
		//  ��ȡ�������رնԻ���Ĵ���
	}

	// ���ڶԻ����ѹرգ����Խ����� FALSE �Ա��˳�Ӧ�ó���
	//  ����������Ӧ�ó������Ϣ�á�

	//close previous connections
	if (hConnect) WinHttpCloseHandle(hConnect);
	if (hSession) WinHttpCloseHandle(hSession);
	hConnect = NULL;
	hSession = NULL;

	delete []picBuff;
	delete []htmlResponceBuff;

	return FALSE;
}

BOOL CHTTPSWebClientApp::ConnectToURL( const CString& URLString )
{

	// Use WinHttpOpen to obtain a session handle.
	hSession = WinHttpOpen( L"Mozilla/5.0 (compatible; MSIE 9.0; Windows NT 6.1; Trident/5.0)",  
		WINHTTP_ACCESS_TYPE_DEFAULT_PROXY,
		WINHTTP_NO_PROXY_NAME, 
		WINHTTP_NO_PROXY_BYPASS, 0);

	// Specify an HTTP server.
	if (hSession == NULL)
		return FALSE;

	hConnect = WinHttpConnect( hSession, URLString,
		INTERNET_DEFAULT_HTTPS_PORT, 0);

	if(hConnect == NULL)
		return FALSE;

	return TRUE;
}

void CHTTPSWebClientApp::GetFromURL( const CString& webResString )
{
	CString refererStr = _T("/otsweb/loginAction.do?method=init");
	CString acptTypStr = _T("text/html, application/xhtml+xml, */*");

	HINTERNET hRequest = SendRequest(0, refererStr, acptTypStr, webResString, NULL, 0);

	if(hRequest)
		GetResponse(hRequest);
	else
		return;

	if(GetValidBufferSize() == 0)
	{
		((CHTTPSWebClientDlg*)m_pMainWnd)->RespondString.Empty();
	}
	else
	{
		LPTSTR unicodeBuf = NULL;
		int unicodeBufSize = MultiByteToWideChar(CP_UTF8, 0, (LPCSTR)GetBufferData(), GetValidBufferSize(), unicodeBuf, 0);
		if(unicodeBufSize > 0)
		{
			unicodeBuf = new TCHAR[unicodeBufSize + 1];
			ZeroMemory(unicodeBuf, (unicodeBufSize + 1)*sizeof(TCHAR));
			MultiByteToWideChar(CP_UTF8, 0, (LPCSTR)GetBufferData(), GetValidBufferSize(), unicodeBuf, unicodeBufSize);
			((CHTTPSWebClientDlg*)m_pMainWnd)->RespondString = unicodeBuf;
			delete []unicodeBuf;
		}
	}

	return;
}

void CHTTPSWebClientApp::PostToURL(const CString& webResString, const CString& PostString)
{
	// transform post string to MBCS type
	char* postData = NULL;
	int postDataSize = 0;
	postDataSize = WideCharToMultiByte(936, 0, PostString.GetString(), PostString.GetLength(), NULL, 0, 0, 0);
	if(postDataSize > 0)
	{
		postData = new char[postDataSize + 1];
		ZeroMemory(postData, (postDataSize + 1)*sizeof(char));
		WideCharToMultiByte(936, 0, PostString.GetString(), PostString.GetLength(), postData, postDataSize, 0, 0);
	}

	CString refererStr = _T("/otsweb/loginAction.do?method=init");
	CString acptTypStr = _T("text/html, application/xhtml+xml, */*");

	HINTERNET hRequest = SendRequest(1, refererStr, acptTypStr, webResString, (const BYTE*)postData, postDataSize);

	if(hRequest)
		GetResponse(hRequest);

	if(postDataSize > 0)
	{
		delete []postData;
	}

	if(hRequest == NULL)
		return;

	if(GetValidBufferSize() == 0)
	{
		((CHTTPSWebClientDlg*)m_pMainWnd)->RespondString.Empty();
	}
	else
	{
		LPTSTR unicodeBuf = NULL;
		int unicodeBufSize = MultiByteToWideChar(CP_UTF8, 0, (LPCSTR)GetBufferData(), GetValidBufferSize(), unicodeBuf, 0);
		if(unicodeBufSize > 0)
		{
			unicodeBuf = new TCHAR[unicodeBufSize + 1];
			ZeroMemory(unicodeBuf, (unicodeBufSize + 1)*sizeof(TCHAR));
			MultiByteToWideChar(CP_UTF8, 0, (LPCSTR)GetBufferData(), GetValidBufferSize(), unicodeBuf, unicodeBufSize);
			((CHTTPSWebClientDlg*)m_pMainWnd)->RespondString = unicodeBuf;
			delete []unicodeBuf;
		}
	}

	return;
}

BOOL CHTTPSWebClientApp::GetValidatePic(const CString& ValPicAddr, CValPic& picCtrl)
{
	// get validation picture data and display it on picture control
	CString refererStr = _T("/otsweb/loginAction.do?method=init");
	CString acptTypStr = _T("image/png, image/svg+xml, image/*;q=0.8, */*;q=0.5");
	HINTERNET hRequest = SendRequest(0, refererStr, acptTypStr, ValPicAddr, NULL, 0);

	if(hRequest)
		GetResponse(hRequest);
	else
		return FALSE;

	// convert to bmp src
	BYTE* jpgBuff = (BYTE*)GetBufferData();
	DWORD jpgSize = GetValidBufferSize();

	if(jpgBuff == NULL || jpgSize == 0)
		return FALSE;

	unsigned int bmpHeight = bmpHeightGet(jpgBuff, jpgSize);
	unsigned int bmpWidth = bmpWidthGet(jpgBuff, jpgSize);
	unsigned int bmpComp = bmpCompGet(jpgBuff, jpgSize);

	DWORD bmpSize = 0;
	if(bmpFromJpeg(jpgBuff, jpgSize, picBuff, &bmpSize))
		return FALSE;

	// set to picture control
	picCtrl.imageAttrSet(bmpHeight, bmpWidth);
	picCtrl.imageBuffSet(picBuff, bmpSize);

	return TRUE;
}

void CHTTPSWebClientApp::LoginToSite(const CString& usernameStr, 
									 const CString& passwordStr,
									 const CString& validateStr)
{

	// post request to get rand number
	CString requestRandStr = _T("/otsweb/loginAction.do?method=loginAysnSuggest");

	CString refererStr = _T("/otsweb/loginAction.do?method=init");
	CString acptTypStr = _T("text/html, application/xhtml+xml, */*");
	HINTERNET hRequest = SendRequest(1, refererStr, acptTypStr, requestRandStr, NULL, 0);

	if(hRequest)
		GetResponse(hRequest);
	else
		return;

	// get rand number from response
	int rand = 0;
	sscanf_s((char*)GetBufferData(), "{\"loginRand\":\"%d\",\"randError\":\"Y\"}", &rand);
	if(rand >= 1000 || rand <= 99)
		return;

	// build login string
	CString loginAdr, loginStr;
	loginAdr = _T("/otsweb/loginAction.do?method=login");
	loginStr.Format(L"loginRand=%d"
		L"&refundLogin=N"
		L"&refundFlag=Y"
		L"&loginUser.user_name=%s"
		L"&nameErrorFocus="
		L"&user.password=%s"
		L"&passwordErrorFocus="
		L"&randCode=%s"
		L"&randErrorFocus=", 
		rand, 
		//usernameStr, passwordStr, 
		_T("bkp84335"), _T("bsp2236"),
		validateStr);

	// transform post string to MBCS type
	char* postData = NULL;
	int postDataSize = 0;
	postDataSize = WideCharToMultiByte(936, 0, loginStr.GetString(), loginStr.GetLength(), NULL, 0, 0, 0);
	if(postDataSize > 0)
	{
		postData = new char[postDataSize + 1];
		ZeroMemory(postData, (postDataSize + 1)*sizeof(char));
		WideCharToMultiByte(936, 0, loginStr.GetString(), loginStr.GetLength(), postData, postDataSize, 0, 0);
	}

	refererStr = _T("/otsweb/loginAction.do?method=init");
	acptTypStr = _T("text/html, application/xhtml+xml, */*");
	hRequest = SendRequest(1, refererStr, acptTypStr, loginAdr, (const BYTE*)postData, postDataSize);

	if(hRequest)
		GetResponse(hRequest);
	else
		return;

	if(postDataSize > 0)
	{
		delete []postData;
	}

	if(GetValidBufferSize() == 0)
	{
		((CHTTPSWebClientDlg*)m_pMainWnd)->RespondString.Empty();
	}
	else
	{
		LPTSTR unicodeBuf = NULL;
		int unicodeBufSize = MultiByteToWideChar(CP_UTF8, 0, (LPCSTR)GetBufferData(), GetValidBufferSize(), unicodeBuf, 0);
		if(unicodeBufSize > 0)
		{
			unicodeBuf = new TCHAR[unicodeBufSize + 1];
			ZeroMemory(unicodeBuf, (unicodeBufSize + 1)*sizeof(TCHAR));
			MultiByteToWideChar(CP_UTF8, 0, (LPCSTR)GetBufferData(), GetValidBufferSize(), unicodeBuf, unicodeBufSize);
			((CHTTPSWebClientDlg*)m_pMainWnd)->RespondString = unicodeBuf;
			delete []unicodeBuf;
		}
	}

	return;
}

HINTERNET CHTTPSWebClientApp::SendRequest(int verb, const CString& refererStr, const CString& acceptTypStr, const CString& webResStr, const BYTE* addtionData, const DWORD addtionSize)
{
	if (hConnect == NULL)
		return NULL;

	// clear response buffer first
	memset(htmlResponceBuff, 0, MAX_JPG_SIZE);
	htmlResponceSize = 0;

	CString verbStr;
	if(verb == 0)
		verbStr = _T("GET");
	else
		verbStr = _T("POST");

	// Create an HTTP request handle.
	LPCTSTR acptTypStrPointer = acceptTypStr;
	HINTERNET hRequest = WinHttpOpenRequest( hConnect, verbStr,
			webResStr,
			NULL, 
			//refererStr, 
			//_T("https://dynamic.12306.cn/otsweb/loginAction.do?method=init"),
			WINHTTP_NO_REFERER,
			//&acptTypStrPointer,
			WINHTTP_DEFAULT_ACCEPT_TYPES,
			WINHTTP_FLAG_SECURE
			);

	if(hRequest == NULL)
	{
		return NULL;
	}

#if 1
	DWORD dwFlags;
	DWORD dwBuffLen = sizeof(dwFlags);
	if(FALSE == WinHttpQueryOption (hRequest, WINHTTP_OPTION_SECURITY_FLAGS,
		(LPVOID)&dwFlags, &dwBuffLen))
	{
		WinHttpCloseHandle(hRequest);
		return NULL;
	}

	dwFlags |= SECURITY_FLAG_IGNORE_UNKNOWN_CA;
	dwFlags |= SECURITY_FLAG_IGNORE_CERT_DATE_INVALID;
	dwFlags |= SECURITY_FLAG_IGNORE_CERT_CN_INVALID;

	dwFlags &= ~SECURITY_FLAG_SECURE;
	dwFlags &= ~SECURITY_FLAG_STRENGTH_WEAK;
	dwFlags &= ~SECURITY_FLAG_STRENGTH_MEDIUM;
	dwFlags &= ~SECURITY_FLAG_STRENGTH_STRONG;

	if(FALSE == WinHttpSetOption (hRequest, WINHTTP_OPTION_SECURITY_FLAGS,
		&dwFlags, sizeof (dwFlags) ))
	{
		WinHttpCloseHandle(hRequest);
		return NULL;
	}
#endif

	// add additional headers
	//if(FALSE == WinHttpAddRequestHeaders( hRequest, 
	//	_T("Accept-Encoding: gzip, deflate"),
	//	-1,
	//	WINHTTP_ADDREQ_FLAG_ADD) )
	//{
	//	WinHttpCloseHandle(hRequest);
	//	return NULL;
	//}
	//if(FALSE == WinHttpAddRequestHeaders( hRequest, 
	//	_T("Cache-Control: no-cache"),
	//	-1,
	//	WINHTTP_ADDREQ_FLAG_ADD) )
	//{
	//	WinHttpCloseHandle(hRequest);
	//	return NULL;
	//}
	//if(FALSE == WinHttpAddRequestHeaders( hRequest, 
	//	_T("Accept: text/html, application/xhtml+xml, */*"),
	//	-1,
	//	WINHTTP_ADDREQ_FLAG_ADD) )
	//{
	//	WinHttpCloseHandle(hRequest);
	//	return NULL;
	//}
	//if(FALSE == WinHttpAddRequestHeaders( hRequest, 
	//	_T("Accept-Language: zh-CN"),
	//	-1,
	//	WINHTTP_ADDREQ_FLAG_ADD) )
	//{
	//	WinHttpCloseHandle(hRequest);
	//	return NULL;
	//}
	if(FALSE == WinHttpAddRequestHeaders( hRequest, 
		_T("Content-Type: application/x-www-form-urlencoded"),
		-1,
		WINHTTP_ADDREQ_FLAG_ADD) )
	{
		WinHttpCloseHandle(hRequest);
		return NULL;
	}

	// Send a request
	DWORD reqDataSize = addtionSize;
	if(addtionData == NULL)
	{
		reqDataSize = 0;
	}

	if( FALSE == WinHttpSendRequest( hRequest,
			WINHTTP_NO_ADDITIONAL_HEADERS,
			0, (LPVOID)addtionData, reqDataSize, 
			reqDataSize, 0))
	{
		WinHttpCloseHandle(hRequest);
		return NULL;
	}

	//WINHTTP_CERTIFICATE_INFO certInfo;
	//DWORD certInfoLen = sizeof(certInfo);
	//BOOL res = WinHttpQueryOption(hRequest,
	//	WINHTTP_OPTION_SECURITY_CERTIFICATE_STRUCT,
	//	&certInfo,
	//	&certInfoLen);

	//DWORD dwFlags;
	//DWORD dwBuffLen = sizeof(dwFlags);
	//if(FALSE == WinHttpQueryOption (hRequest, WINHTTP_OPTION_SECURITY_FLAGS,
	//	(LPVOID)&dwFlags, &dwBuffLen))
	//{
	//	WinHttpCloseHandle(hRequest);
	//	return NULL;
	//}

	return hRequest;
}

BOOL CHTTPSWebClientApp::GetResponse(HINTERNET hRequest)
{
	// End the request.
	if (hRequest == NULL)
		return FALSE;
	
	if(WinHttpReceiveResponse( hRequest, NULL) == FALSE)
	{
		WinHttpCloseHandle(hRequest);
		return FALSE;
	}

	// Keep checking for data until there is nothing left.
	DWORD dwSize = 0;
	DWORD dwDownloaded = 0;
	DWORD buffPostion = 0;	// writer position of the buffer

	do 
	{
		// Check for available data.
		if (FALSE == WinHttpQueryDataAvailable( hRequest, &dwSize))
		{
			WinHttpCloseHandle(hRequest);
			return FALSE;
		}

		// Allocate space for the buffer.
		if(buffPostion + dwSize > MAX_JPG_SIZE)
		{
			// buffer overflow!
			WinHttpCloseHandle(hRequest);
			return FALSE;
		}

		// Read the Data.
		if (FALSE == WinHttpReadData( hRequest, (LPVOID)(htmlResponceBuff + buffPostion), 
			dwSize, &dwDownloaded))
		{
			WinHttpCloseHandle(hRequest);
			return FALSE;
		}
		else
		{
			buffPostion += dwDownloaded;
		}

	} while (dwDownloaded > 0);

	htmlResponceSize = buffPostion;

	WinHttpCloseHandle(hRequest);
	return TRUE;
}

LPVOID CHTTPSWebClientApp::GetBufferData()
{
	return htmlResponceBuff;
}

DWORD CHTTPSWebClientApp::GetValidBufferSize()
{
	return htmlResponceSize;
}
