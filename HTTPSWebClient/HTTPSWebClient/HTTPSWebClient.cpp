
// HTTPSWebClient.cpp : 定义应用程序的类行为。
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


// CHTTPSWebClientApp 构造

CHTTPSWebClientApp::CHTTPSWebClientApp()
{
	// TODO: 在此处添加构造代码，
	// 将所有重要的初始化放置在 InitInstance 中
}


// 唯一的一个 CHTTPSWebClientApp 对象

CHTTPSWebClientApp theApp;


// CHTTPSWebClientApp 初始化

BOOL CHTTPSWebClientApp::InitInstance()
{
	// 如果一个运行在 Windows XP 上的应用程序清单指定要
	// 使用 ComCtl32.dll 版本 6 或更高版本来启用可视化方式，
	//则需要 InitCommonControlsEx()。否则，将无法创建窗口。
	INITCOMMONCONTROLSEX InitCtrls;
	InitCtrls.dwSize = sizeof(InitCtrls);
	// 将它设置为包括所有要在应用程序中使用的
	// 公共控件类。
	InitCtrls.dwICC = ICC_WIN95_CLASSES;
	InitCommonControlsEx(&InitCtrls);

	CWinAppEx::InitInstance();

	// 标准初始化
	// 如果未使用这些功能并希望减小
	// 最终可执行文件的大小，则应移除下列
	// 不需要的特定初始化例程
	// 更改用于存储设置的注册表项
	// TODO: 应适当修改该字符串，
	// 例如修改为公司或组织名
	SetRegistryKey(_T("应用程序向导生成的本地应用程序"));

	CHTTPSWebClientDlg dlg;
	m_pMainWnd = &dlg;
	INT_PTR nResponse = dlg.DoModal();
	if (nResponse == IDOK)
	{
		// TODO: 在此放置处理何时用
		//  “确定”来关闭对话框的代码
	}
	else if (nResponse == IDCANCEL)
	{
		// TODO: 在此放置处理何时用
		//  “取消”来关闭对话框的代码
	}

	// 由于对话框已关闭，所以将返回 FALSE 以便退出应用程序，
	//  而不是启动应用程序的消息泵。
	return FALSE;
}

void CHTTPSWebClientApp::ConnectToURL( const CString& URLString )
{
	DWORD dwSize = 0;
	DWORD dwDownloaded = 0;
	LPSTR pszOutBuffer;
	BOOL  bResults = FALSE;
	HINTERNET  hSession = NULL, 
		hConnect = NULL,
		hRequest = NULL;

	// Use WinHttpOpen to obtain a session handle.
	hSession = WinHttpOpen( L"WinHTTP Example/1.0",  
		WINHTTP_ACCESS_TYPE_NO_PROXY,
		//WINHTTP_ACCESS_TYPE_DEFAULT_PROXY,
		WINHTTP_NO_PROXY_NAME, 
		WINHTTP_NO_PROXY_BYPASS, 0);

	//TODO: the URL is invalid
	// Specify an HTTP server.
	if (hSession)
		hConnect = WinHttpConnect( hSession, URLString,
		INTERNET_DEFAULT_HTTPS_PORT, 0);

	// Create an HTTP request handle.
	if (hConnect)
		hRequest = WinHttpOpenRequest( hConnect, L"GET", NULL,
		NULL, WINHTTP_NO_REFERER, 
		WINHTTP_DEFAULT_ACCEPT_TYPES, 
		WINHTTP_FLAG_BYPASS_PROXY_CACHE
		//WINHTTP_FLAG_SECURE
		);

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
					//cout << pszOutBuffer;
					((CHTTPSWebClientDlg*)m_pMainWnd)->GETString += pszOutBuffer;

				// Free the memory allocated to the buffer.
				delete [] pszOutBuffer;
			}

		} while (dwSize>0);


		// Report any errors.
		if (!bResults)
			printf("Error %d has occurred.\n",GetLastError());

		// Close any open handles.
		if (hRequest) WinHttpCloseHandle(hRequest);
		if (hConnect) WinHttpCloseHandle(hConnect);
		if (hSession) WinHttpCloseHandle(hSession);


	return;
}
