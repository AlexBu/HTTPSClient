
// HTTPSWebClient.cpp : ����Ӧ�ó��������Ϊ��
//

#include "stdafx.h"
#include "HTTPSWebClient.h"
#include "HTTPSWebClientDlg.h"

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

	return FALSE;
}

void CHTTPSWebClientApp::ConnectToURL( const CString& URLString )
{
	//close previous connections
	if (hConnect) WinHttpCloseHandle(hConnect);
	if (hSession) WinHttpCloseHandle(hSession);

	// Use WinHttpOpen to obtain a session handle.
	hSession = WinHttpOpen( L"WinHTTP Example/1.0",  
		//WINHTTP_ACCESS_TYPE_NO_PROXY,
		WINHTTP_ACCESS_TYPE_DEFAULT_PROXY,
		WINHTTP_NO_PROXY_NAME, 
		WINHTTP_NO_PROXY_BYPASS, 0);

	// Specify an HTTP server.
	if (hSession)
		hConnect = WinHttpConnect( hSession, URLString,
		INTERNET_DEFAULT_HTTPS_PORT, 0);

	return;
}

void CHTTPSWebClientApp::GetFromURL( const CString& URLString )
{
	DWORD dwSize = 0;
	DWORD dwDownloaded = 0;
	LPSTR pszOutBuffer;
	BOOL  bResults = FALSE;
	HINTERNET hRequest = NULL;

	// Create an HTTP request handle.
	if (hConnect && hSession)
		hRequest = WinHttpOpenRequest( hConnect, L"GET", URLString,
		NULL, WINHTTP_NO_REFERER, 
		WINHTTP_DEFAULT_ACCEPT_TYPES, 
		//WINHTTP_FLAG_BYPASS_PROXY_CACHE
		WINHTTP_FLAG_SECURE
		);

	DWORD dwFlags;
	DWORD dwBuffLen = sizeof(dwFlags);            
	WinHttpQueryOption (hRequest, WINHTTP_OPTION_SECURITY_FLAGS,
		(LPVOID)&dwFlags, &dwBuffLen);
	dwFlags |= SECURITY_FLAG_IGNORE_UNKNOWN_CA;
	dwFlags |= SECURITY_FLAG_IGNORE_CERT_DATE_INVALID;
	dwFlags |= SECURITY_FLAG_IGNORE_CERT_CN_INVALID;

	WinHttpSetOption (hRequest, WINHTTP_OPTION_SECURITY_FLAGS,
		&dwFlags, sizeof (dwFlags) );

	// Send a request.
	if (hRequest)
		bResults = WinHttpSendRequest( hRequest,
		WINHTTP_NO_ADDITIONAL_HEADERS,
		0, WINHTTP_NO_REQUEST_DATA, 0, 
		0, 0);


	// End the request.
	if (bResults)
		bResults = WinHttpReceiveResponse( hRequest, NULL);

	// Keep checking for data until there is nothing left.
	if (bResults)
	{
		((CHTTPSWebClientDlg*)m_pMainWnd)->RespondString.Empty();
		do 
		{
			// Check for available data.
			dwSize = 0;
			if (!WinHttpQueryDataAvailable( hRequest, &dwSize))
				printf("Error %u in WinHttpQueryDataAvailable.\n",
				GetLastError());

			// Allocate space for the buffer.
			pszOutBuffer = new char[dwSize+1];
			if (!pszOutBuffer)
			{
				printf("Out of memory\n");
				dwSize=0;
			}
			else
			{
				// Read the Data.
				ZeroMemory(pszOutBuffer, dwSize+1);

				if (!WinHttpReadData( hRequest, (LPVOID)pszOutBuffer, 
					dwSize, &dwDownloaded))
					printf( "Error %u in WinHttpReadData.\n", 
					GetLastError());
				else
				{
					//cout << pszOutBuffer;
					LPTSTR unicodeBuf = NULL;
					int unicodeBufSize = MultiByteToWideChar(CP_UTF8, 0, pszOutBuffer, dwSize, unicodeBuf, 0);
					if(unicodeBufSize > 0)
					{
						unicodeBuf = new TCHAR[unicodeBufSize + 1];
						ZeroMemory(unicodeBuf, (unicodeBufSize + 1)*sizeof(TCHAR));
						MultiByteToWideChar(CP_UTF8, 0, pszOutBuffer, dwSize, unicodeBuf, unicodeBufSize);
						((CHTTPSWebClientDlg*)m_pMainWnd)->RespondString += unicodeBuf;
						delete []unicodeBuf;
					}
					
				}

				// Free the memory allocated to the buffer.
				delete [] pszOutBuffer;
			}

		} while (dwSize>0);
	}

	// Report any errors.
	if (!bResults)
		printf("Error %d has occurred.\n",GetLastError());

	// Close any open handles.
	if (hRequest) WinHttpCloseHandle(hRequest);

	return;
}

void CHTTPSWebClientApp::PostToURL(const CString& URLString, const CString& WebResString, const CString& PostString)
{
	//
}