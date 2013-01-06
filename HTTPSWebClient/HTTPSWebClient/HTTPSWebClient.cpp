
// HTTPSWebClient.cpp : 定义应用程序的类行为。
//

#include "stdafx.h"
#include "HTTPSWebClient.h"
#include "HTTPSWebClientDlg.h"

#include "jpeglib.h"
#include "jpg.h"

#include <atlrx.h>

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
	//SetRegistryKey(_T("应用程序向导生成的本地应用程序"));


	hConnect = NULL;
	hSession = NULL;
	htmlResponseBuff = new BYTE[MAX_JPG_SIZE];
	picBuff = new BYTE[MAX_JPG_SIZE];

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

	//close previous connections
	if (hConnect) WinHttpCloseHandle(hConnect);
	if (hSession) WinHttpCloseHandle(hSession);
	hConnect = NULL;
	hSession = NULL;

	delete []picBuff;
	delete []htmlResponseBuff;

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

	ConvertToUTF();

	((CHTTPSWebClientDlg*)m_pMainWnd)->RespondString = htmlResponseStr;

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

	ConvertToUTF();

	((CHTTPSWebClientDlg*)m_pMainWnd)->RespondString = htmlResponseStr;

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

	unsigned int bmpHeight = 0;
	unsigned int bmpWidth = 0;
	DWORD bmpSize = 0;

	if(!convertToBMP(&bmpWidth, &bmpHeight, &bmpSize))
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

	ConvertToUTF();
	// get rand number from response

	CString patternRand = L"{\\d+}";
	regex.patternLoad(patternRand);
	CString restStr;
	regex.contextMatch(htmlResponseStr, restStr);
	CString rand;
	regex.matchGet(0, rand);

	// build login string
	CString loginAdr, loginStr;
	loginAdr = _T("/otsweb/loginAction.do?method=login");
	loginStr.Format(L"loginRand=%s"
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

	ConvertToUTF();

	CString patternTitle = L"<[tT][iI][tT][lL][eE]>{[^</>]+}</[tT][iI][tT][lL][eE]>";
	regex.patternLoad(patternTitle);
	CString restStr2;
	regex.contextMatch(htmlResponseStr, restStr2);
	CString titleStr;
	regex.matchGet(0, titleStr);

	if(titleStr == L"系统消息")
	{
		((CHTTPSWebClientDlg*)m_pMainWnd)->RespondString = L"log in success!";
	}

	return;
}

HINTERNET CHTTPSWebClientApp::SendRequest(int verb, const CString& refererStr, const CString& acceptTypStr, const CString& webResStr, const BYTE* addtionData, const DWORD addtionSize)
{
	if (hConnect == NULL)
		return NULL;

	// clear response buffer first
	memset(htmlResponseBuff, 0, MAX_JPG_SIZE);
	htmlResponseSize = 0;

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
		if (FALSE == WinHttpReadData( hRequest, (LPVOID)(htmlResponseBuff + buffPostion), 
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

	htmlResponseSize = buffPostion;

	WinHttpCloseHandle(hRequest);
	return TRUE;
}

LPVOID CHTTPSWebClientApp::GetBufferData()
{
	return htmlResponseBuff;
}

DWORD CHTTPSWebClientApp::GetValidBufferSize()
{
	return htmlResponseSize;
}

void CHTTPSWebClientApp::ConvertToUTF()
{
	htmlResponseStr.Empty();
	if(GetValidBufferSize() > 0)
	{
		LPTSTR unicodeBuf = NULL;
		int unicodeBufSize = MultiByteToWideChar(CP_UTF8, 0, (LPCSTR)GetBufferData(), GetValidBufferSize(), unicodeBuf, 0);
		if(unicodeBufSize > 0)
		{
			LPTSTR unicodeBuf = htmlResponseStr.GetBuffer(unicodeBufSize + 1);
			MultiByteToWideChar(CP_UTF8, 0, (LPCSTR)GetBufferData(), GetValidBufferSize(), unicodeBuf, unicodeBufSize);
			htmlResponseStr.ReleaseBuffer(unicodeBufSize);
		}
	}

	
}

BOOL CHTTPSWebClientApp::convertToBMP(unsigned int *bmpWidth,
									  unsigned int *bmpHeight,
									  DWORD *bmpSize)
{
	// convert to bmp src
	BYTE* jpgBuff = (BYTE*)GetBufferData();
	DWORD jpgSize = GetValidBufferSize();

	if(jpgBuff == NULL || jpgSize == 0)
		return FALSE;

	*bmpHeight = bmpHeightGet(jpgBuff, jpgSize);
	*bmpWidth = bmpWidthGet(jpgBuff, jpgSize);

	if(bmpFromJpeg(jpgBuff, jpgSize, picBuff, bmpSize))
		return FALSE;

	return TRUE;
}

void CHTTPSWebClientApp::QueryTickets(CString& date)
{
	// build up a test information
	CString queryStr;

	CString refererStr = _T("/otsweb/loginAction.do?method=init");
	CString acptTypStr = _T("text/html, application/xhtml+xml, */*");

	queryStr.Format( L"/otsweb/order/querySingleAction.do?method=queryLeftTicket"
		L"&orderRequest.train_date=%s"
		L"&orderRequest.from_station_telecode=NJH"
		L"&orderRequest.to_station_telecode=SHH"
		L"&orderRequest.train_no="
		L"&trainPassType=QB%%23D%%23Z%%23T%%23K%%23QT%%23"
		L"&trainClass=D%%23"
		L"&includeStudent=00"
		L"&seatTypeAndNum="
		L"&orderRequest.start_time_str=00%%3A00--24%%3A00", date);
	HINTERNET hRequest = SendRequest(0, refererStr, acptTypStr, queryStr, NULL, 0);

	if(hRequest)
		GetResponse(hRequest);
	else
		return;

	ConvertToUTF();

	CString resultStr;

	// split results
	// "({[^\\,]+},)+"
	//CString pattern = L"{[^\\\\,]+},{[^\\\\,]+},{[^\\\\,]+},{[^\\\\,]+},{[^\\\\,]+},"
	//				L"{[^\\\\,]+},{[^\\\\,]+},{[^\\\\,]+},{[^\\\\,]+},{[^\\\\,]+},"
	//				L"{[^\\\\,]+},{[^\\\\,]+},{[^\\\\,]+},{[^\\\\,]+},{[^\\\\,]+},"
	//				L"{[^\\\\,]+},{[^\\\\,]+}\\\\n";
	CString pattern = L"javascript:getSelected\\(\\'"
		L"{[^#]+}#{[^#]+}#{[^#]+}#{[^#]+}#{[^#]+}#{[^#]+}#"
		L"{[^#]+}#{[^#]+}#{[^#]+}#{[^#]+}#{[^#]+}#{[^#]+}#{[^#]+}#{[^#\\']+}\\')>";
	regex.patternLoad(pattern);

	CString restStr;
	CString matchStr;

	matchStr = htmlResponseStr;

	// get the first record
	while(regex.contextMatch(matchStr, restStr))
	{		
		if(regex.matchCount() == 14)
		{
			CString tempStr;

			regex.matchGet(0, tempStr);
			resultStr.AppendFormat(L"%s\r\n", tempStr);
			ticketInfo.trainCodeSet(tempStr);

			regex.matchGet(1, tempStr);
			resultStr.AppendFormat(L"%s\r\n", tempStr);
			ticketInfo.durationSet(tempStr);

			regex.matchGet(2, tempStr);
			resultStr.AppendFormat(L"%s\r\n", tempStr);
			ticketInfo.trainStartTimeSet(tempStr);

			regex.matchGet(3, tempStr);
			resultStr.AppendFormat(L"%s\r\n", tempStr);
			ticketInfo.trainNoSet(tempStr);

			regex.matchGet(4, tempStr);
			resultStr.AppendFormat(L"%s\r\n", tempStr);
			ticketInfo.stationFromCodeSet(tempStr);

			regex.matchGet(5, tempStr);
			resultStr.AppendFormat(L"%s\r\n", tempStr);
			ticketInfo.stationToCodeSet(tempStr);

			regex.matchGet(6, tempStr);
			resultStr.AppendFormat(L"%s\r\n", tempStr);
			ticketInfo.timeArriveSet(tempStr);

			regex.matchGet(7, tempStr);
			resultStr.AppendFormat(L"%s\r\n", tempStr);
			ticketInfo.stationFromTeNameSet(tempStr);
			ticketInfo.stationFromNameSet(tempStr);

			regex.matchGet(8, tempStr);
			resultStr.AppendFormat(L"%s\r\n", tempStr);
			ticketInfo.stationToTeNameSet(tempStr);
			ticketInfo.stationToNameSet(tempStr);

			regex.matchGet(9, tempStr);
			resultStr.AppendFormat(L"%s\r\n", tempStr);
			ticketInfo.stationFromNoSet(tempStr);

			regex.matchGet(10, tempStr);
			resultStr.AppendFormat(L"%s\r\n", tempStr);
			ticketInfo.stationToNoSet(tempStr);

			regex.matchGet(11, tempStr);
			resultStr.AppendFormat(L"%s\r\n", tempStr);
			ticketInfo.infoDetailSet(tempStr);

			regex.matchGet(12, tempStr);
			resultStr.AppendFormat(L"%s\r\n", tempStr);
			ticketInfo.mmStrSet(tempStr);

			regex.matchGet(13, tempStr);
			resultStr.AppendFormat(L"%s\r\n", tempStr);
			ticketInfo.locationCodeSet(tempStr);

			CString str(L"");
			ticketInfo.studentSet(str);
			ticketInfo.trainRoundTimeStrSet(str);
			ticketInfo.roundTypeSet(str);
			ticketInfo.passTypeSet(str);
			ticketInfo.trainClassSet(str);
			ticketInfo.timeStartStrSet(str);

			ticketInfo.trainRoundDateSet(date);
			ticketInfo.trainDateSet(date);

			// test on the first result
			break;
		}

		resultStr += L"\r\n";

		matchStr = restStr;
		restStr.Empty();
	}

	// test on the first result
	// build login string
	CString bookTrainAdr, bookTrainStr;
	bookTrainAdr = _T("/otsweb/order/querySingleAction.do?method=submutOrderRequest");
	ticketInfo.strBuild(bookTrainStr);

	// transform post string to MBCS type
	char* postData = NULL;
	int postDataSize = 0;
	postDataSize = WideCharToMultiByte(936, 0, bookTrainStr.GetString(), bookTrainStr.GetLength(), NULL, 0, 0, 0);
	if(postDataSize > 0)
	{
		postData = new char[postDataSize + 1];
		ZeroMemory(postData, (postDataSize + 1)*sizeof(char));
		WideCharToMultiByte(936, 0, bookTrainStr.GetString(), bookTrainStr.GetLength(), postData, postDataSize, 0, 0);
	}

	refererStr = _T("/otsweb/loginAction.do?method=init");
	acptTypStr = _T("text/html, application/xhtml+xml, */*");
	hRequest = SendRequest(1, refererStr, acptTypStr, bookTrainAdr, (const BYTE*)postData, postDataSize);

	if(hRequest)
		GetResponse(hRequest);
	else
		return;

	if(postDataSize > 0)
	{
		delete []postData;
	}

	ConvertToUTF();

	// organize the output
	((CHTTPSWebClientDlg*)m_pMainWnd)->RespondString = htmlResponseStr;

	return;
}

void CHTTPSWebClientApp::BookTickets( CString& date )
{
	// currently it must be used after QueryTickets

	// analyze query result page
}
