
// HTTPSWebClient.cpp : 定义应用程序的类行为。
//

#include "stdafx.h"
#include "HTTPSWebClient.h"
#include "HTTPSWebClientDlg.h"

#include "jpeglib.h"
#include "jpg.h"

#include <atlrx.h>

// delay second
#define GENERAL_DELAY		(5)


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
	httpContent.ConnectSite(URLString);
}

BOOL CHTTPSWebClientApp::GetValidatePic(const CString& ValPicAddr, CValPicCtrl& picCtrl)
{
	// get validation picture data and display it on picture control
	unsigned int bmpHeight = 0;
	unsigned int bmpWidth = 0;

	DWORD jpgSize = SMALL_JPG_SIZE;
	BYTE* jpgBuff = new BYTE[jpgSize];

	DWORD bmpSize = SMALL_JPG_SIZE;
	BYTE* bmpBuff = new BYTE[bmpSize];

	httpContent.SendDatabyGet(ValPicAddr);
	httpContent.GetResponseRaw(jpgBuff, jpgSize);

	bmpHeight = bmpHeightGet(jpgBuff, jpgSize);
	bmpWidth = bmpWidthGet(jpgBuff, jpgSize);

	if(bmpFromJpeg(jpgBuff, jpgSize, bmpBuff, &bmpSize) == 0)
	{
		// set to picture control
		picCtrl.imageAttrSet(bmpHeight, bmpWidth);
		picCtrl.imageBuffSet(bmpBuff, bmpSize);
	}

	delete []jpgBuff;
	delete []bmpBuff;

	return TRUE;
}

void CHTTPSWebClientApp::LoginToSite(CString& result)
{
	// try to create a thread to do login things
	CWinThread* loginThread = AfxBeginThread(loginWorker, this);
	return;
	// get validation code for login action
	CValidationDialog dlg;
	loginValPage.BuildRequest();
	loginValPage.GetPageData(httpContent);
	loginValPage.ParseOutput();
	dlg.pic.imageAttrSet(loginValPage.height, loginValPage.width);
	dlg.pic.imageBuffSet(loginValPage.bmpbuff, loginValPage.bmpsize);
	if(dlg.DoModal() == IDOK)
	{
		// OK, get code
		loginInfo.validate = dlg.code;
	}
	else
	{
		// cancel, quit
		CLog::GetLog().AddLog(L"user cancel validation code input, quit login\r\n");
		return;
	}

	// post request to get rand number
	loginRandPage.BuildRequest();
	loginRandPage.GetPageStr(httpContent);
	loginRandPage.ParseOutput(loginInfo);

	if(loginRandPage.GetStatus() != 0)
	{
		result = L"log in failed: get rand error.";
		return;
	}

	loginPage.BuildRequest(loginInfo);
	loginPage.GetPageStr(httpContent);
	loginPage.ParseOutput();

	if(loginPage.GetStatus() == 0)
		result = L"log in success!";
	else
		result.Format(L"login failed: error code %d", loginPage.GetStatus());
	return;
}

void CHTTPSWebClientApp::BookTickets(CString& result)
{
	result += L"\r\nquerying train...";
	CLog::GetLog().AddLog(L"query action start\r\n");
	queryPage.BuildRequest(queryInfo);

QUERY:

	queryPage.GetPageStr(httpContent);
	queryPage.ParseOutput(trainInfo);

	Sleep(GENERAL_DELAY * 1000);

	int queryStatus = queryPage.GetStatus();
	if(queryStatus == ERROR_OK)
	{
		// pass to next phase
	}
	else if(queryStatus == ERROR_HTTP)
	{
		result += L"\r\nquery page: http connection failed!";
		return;
	}
	else
	{
		// add query delay
		CLog::GetLog().AddLog(L"no proper train found, wait for retry\r\n");
		goto QUERY;
	}


	result += L"\r\nbooking tickets...";
	CLog::GetLog().AddLog(L"book action start\r\n");
	bookPage.BuildRequest(trainInfo);
BOOK:
	bookPage.GetPageStr(httpContent);
	bookPage.ParseOutput(ticketInfo);

	Sleep(GENERAL_DELAY * 1000);

	int bookStatus = bookPage.GetStatus();
	if(bookStatus == ERROR_OK)
	{
		// pass to next phase
	}
	else if(bookStatus == ERROR_HTTP)
	{
		result += L"\r\nbook page: http connection failed!";
		return;
	}
	else
	{
		result.AppendFormat(L"\r\nbook failed: error code %d", bookStatus);
		goto BOOK;
	}

	// add check order page validation code query
	CValidationDialog dlg;
	// add some rand number?
	CString ValPicAddr = L"/otsweb/passCodeAction.do?rand=randp";
	GetValidatePic(ValPicAddr, dlg.pic);
	if(dlg.DoModal() == IDOK)
	{
		// OK, get code
		ticketInfo.randCode = dlg.code;
	}
	else
	{
		// cancel, quit
		CLog::GetLog().AddLog(L"user cancel validation code input, quit login\r\n");
		return;
	}


	result += L"\r\nchecking order...";
	CLog::GetLog().AddLog(L"check action start\r\n");
	checkPage.BuildRequest(ticketInfo);
CHECK:
	checkPage.GetPageStr(httpContent);
	checkPage.ParseOutput(orderInfo);

	Sleep(GENERAL_DELAY * 1000);

	int checkStatus = checkPage.GetStatus();
	if(checkStatus == ERROR_OK)
	{
		// pass to next phase
	}
	else if(checkStatus == ERROR_VALIDATE)
	{
		result.AppendFormat(L"\r\ncheck failed: error code %d", checkStatus);
		result += L"\r\nre-enter validation code!";
		// resend from here will need validate code!
		return;
	}
	else if(checkStatus == ERROR_HTTP)
	{
		result += L"\r\ncheck page: http connection failed!";
		result += L"\r\nretry";
		return;
	}
	else
	{
		result.AppendFormat(L"\r\ncheck failed: error code %d", checkStatus);
		result += L"\r\nretry";
		goto CHECK;
	}



	result += L"\r\nqueueing order...";
	CLog::GetLog().AddLog(L"queue action start\r\n");
	queuePage.BuildRequest(ticketInfo);
QUEUE:
	queuePage.GetPageStr(httpContent);
	queuePage.ParseOutput(orderInfo);
	
	Sleep(GENERAL_DELAY * 1000);

	int queueStatus = queuePage.GetStatus();
	if(queueStatus == ERROR_OK)
	{
		// pass to next phase
	}
	else if(queueStatus == ERROR_HTTP)
	{
		result += L"\r\nqueue page: http connection failed!";
		result += L"\r\nretry";
		return;
	}
	else
	{
		result.AppendFormat(L"\r\nqueue failed: error code %d", queueStatus);
		result += L"\r\nretry";
		goto QUEUE;
	}


	result += L"\r\nconfirming order...";
	CLog::GetLog().AddLog(L"confirm action start\r\n");
	confirmPage.BuildRequest(orderInfo);
CONFIRM:
	confirmPage.GetPageStr(httpContent);
	confirmPage.ParseOutput(orderInfo);

	Sleep(GENERAL_DELAY * 1000);

	int confirmStatus = confirmPage.GetStatus();

	if(confirmStatus == ERROR_OK)
	{
		// pass to next phase
	}
	else if(confirmStatus == ERROR_HTTP)
	{
		result += L"\r\nconfirm page: http connection failed!";
		result += L"\r\nretry";
		return;
	}
	else if(confirmStatus == ERROR_VALIDATE)
	{
		result.AppendFormat(L"\r\nconfirm failed: error code %d", confirmStatus);
		result += L"\r\nre-enter validation code!";
		// resend from here will need validate code!
		return;
	}
	else
	{
		result.AppendFormat(L"\r\nconfirm failed: error code %d", confirmStatus);
		result += L"\r\nretry";
		goto CONFIRM;
	}


	result += L"\r\nwaiting for result...";
	CLog::GetLog().AddLog(L"wait action start\r\n");
	waitPage.BuildRequest(orderInfo);
	waitPage.GetPageStr(httpContent);
	waitPage.ParseOutput(orderInfo);

	Sleep(GENERAL_DELAY * 1000);

	int waitStatus = waitPage.GetStatus();
	if(waitStatus == ERROR_OK)
	{
		result += L"\r\nbuy ticket success!";
	}
	else if(waitStatus == ERROR_HTTP)
	{
		result += L"\r\nwait page: http connection failed!";
		result += L"\r\nretry";
		return;
	}
	else
	{
		result.AppendFormat(L"\r\nwait failed: error code %d", waitStatus);
	}
}

UINT CHTTPSWebClientApp::loginWorker( LPVOID param )
{
	CString statusMsg;

	CHTTPSWebClientApp* app = (CHTTPSWebClientApp*)param;
	CWnd* gui = app->m_pMainWnd;

	statusMsg.Format(L"request validation picture");
	app->SendString(statusMsg);

	app->loginValPage.BuildRequest();
	app->loginValPage.GetPageData(app->httpContent);
	app->loginValPage.ParseOutput();

	// send message to gui to get validation code from user input
	app->PrepareValidationDialog();

	gui->SendMessage(WM_GETCODE, 0, 0);

	// wait for validation code to be retrieved
	WaitForSingleObject(app->valEvent, INFINITE);

	// check result
	if(app->loginInfo.validate.IsEmpty())
	{
		statusMsg.Format(L"user canceled this request, login abort");
		app->SendString(statusMsg);
		return 0;
	}

	statusMsg.Format(L"get validation code: %s.", app->loginInfo.validate);
	gui->SendMessage(WM_SETSTR, 0, (LPARAM)(LPCTSTR)statusMsg);

	statusMsg.Format(L"request login random number");
	app->SendString(statusMsg);

	// post request to get rand number
	app->loginRandPage.BuildRequest();
	app->loginRandPage.GetPageStr(app->httpContent);
	app->loginRandPage.ParseOutput(app->loginInfo);

	if(app->loginRandPage.GetStatus() != 0)
	{
		statusMsg.Format(L"request login rand failed, error code: %d", app->loginRandPage.GetStatus());
		app->SendString(statusMsg);
		return 0;
	}

	statusMsg.Format(L"send login request");
	app->SendString(statusMsg);

	app->loginPage.BuildRequest(app->loginInfo);
	app->loginPage.GetPageStr(app->httpContent);
	app->loginPage.ParseOutput();

	if(app->loginPage.GetStatus() == 0)
	{
		statusMsg.Format(L"login success!");
		app->SendString(statusMsg);
	}
	else
	{
		statusMsg.Format(L"login failed, error code: %d", app->loginPage.GetStatus());
		app->SendString(statusMsg);
	}

	return 0;
}

void CHTTPSWebClientApp::PrepareValidationDialog()
{
	validationDialog.pic.imageAttrSet(loginValPage.height, loginValPage.width);
	validationDialog.pic.imageBuffSet(loginValPage.bmpbuff, loginValPage.bmpsize);
}

void CHTTPSWebClientApp::GetValidationCode()
{
	if(validationDialog.DoModal() == IDOK)
	{
		// OK, get code
		loginInfo.validate = validationDialog.code;
	}
	else
	{
		// cancel, quit
		loginInfo.validate.Empty();
		CLog::GetLog().AddLog(L"user cancel validation code input, quit login\r\n");
		return;
	}
	// notify worker thread things have been complete
	SetEvent(valEvent);
}

void CHTTPSWebClientApp::SendString( CString &msg )
{
	m_pMainWnd->SendMessage(WM_SETSTR, 0, (LPARAM)(LPCTSTR)msg);
}
