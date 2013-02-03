
// HTTPSWebClient.cpp : 定义应用程序的类行为。
//

#include "stdafx.h"
#include "HTTPSWebClient.h"
#include "HTTPSWebClientDlg.h"

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
	}
	else if (nResponse == IDCANCEL)
	{
	}

	// 由于对话框已关闭，所以将返回 FALSE 以便退出应用程序，
	//  而不是启动应用程序的消息泵。

	return FALSE;
}

void CHTTPSWebClientApp::BookTickets()
{
	// assign a worker thread to book
	AfxBeginThread(BookWorker, this);
}

UINT AFX_CDECL CHTTPSWebClientApp::BookWorker( LPVOID param )
{
	CString statusMsg;

	CHTTPSWebClientApp* app = (CHTTPSWebClientApp*)param;

	// build a state table
	STATUS state_table[12][16] = {
		//ok		general	http	logic	val		user	pass	overl	noseat	order	many	real	cancel	inqueue	server	notrain
		{LOGINV,	EXIT,	EXIT,	EXIT,	EXIT,	EXIT,	EXIT,	EXIT,	EXIT,	EXIT,	EXIT,	EXIT,	EXIT,	EXIT,	EXIT,	EXIT	},	//START
		{RAND,		EXIT,	START,	EXIT,	LOGINV,	EXIT,	EXIT,	EXIT,	EXIT,	EXIT,	EXIT,	EXIT,	EXIT,	EXIT,	LOGINV,	EXIT	},	//LOGINV
		{LOGIN,		RAND,	START,	EXIT,	EXIT,	EXIT,	EXIT,	EXIT,	EXIT,	EXIT,	EXIT,	EXIT,	EXIT,	EXIT,	RAND,	EXIT	},	//RAND
		{QUERY,		LOGINV,	START,	EXIT,	LOGINV,	EXIT,	EXIT,	RAND,	EXIT,	EXIT,	EXIT,	EXIT,	EXIT,	EXIT,	LOGIN,	EXIT	},	//LOGIN
		{BOOK,		QUERY,	START,	EXIT,	EXIT,	EXIT,	EXIT,	EXIT,	EXIT,	EXIT,	EXIT,	EXIT,	EXIT,	EXIT,	QUERY,	EXIT	},	//QUERY
		{BOOKV,		QUERY,	START,	EXIT,	EXIT,	EXIT,	EXIT,	EXIT,	QUERY,	EXIT,	EXIT,	EXIT,	EXIT,	EXIT,	BOOK,	EXIT	},	//BOOK
		{CHECK,		BOOKV,	START,	EXIT,	BOOKV,	EXIT,	EXIT,	EXIT,	EXIT,	EXIT,	EXIT,	EXIT,	BOOKV,	EXIT,	BOOKV,	EXIT	},	//BOOKV
		{QUEUE,		CHECK,	START,	BOOK,	BOOKV,	EXIT,	EXIT,	EXIT,	EXIT,	EXIT,	EXIT,	EXIT,	EXIT,	EXIT,	CHECK,	EXIT	},	//CHECK
		{CONFRM,	QUEUE,	START,	BOOK,	QUEUE,	EXIT,	EXIT,	EXIT,	EXIT,	EXIT,	EXIT,	EXIT,	EXIT,	EXIT,	QUEUE,	EXIT	},	//QUEUE
		{WAIT,		CONFRM,	START,	BOOK,	CONFRM,	EXIT,	EXIT,	EXIT,	EXIT,	EXIT,	EXIT,	EXIT,	EXIT,	EXIT,	CONFRM,	EXIT	},	//CONFRM
		{EXIT,		WAIT,	EXIT,	EXIT,	WAIT,	EXIT,	EXIT,	EXIT,	EXIT,	EXIT,	EXIT,	EXIT,	EXIT,	WAIT,	WAIT,	EXIT	},	//WAIT
		{EXIT,		EXIT,	EXIT,	EXIT,	EXIT,	EXIT,	EXIT,	EXIT,	EXIT,	EXIT,	EXIT,	EXIT,	EXIT,	EXIT,	EXIT,	EXIT	},	//EXIT
	};
	STATUS current_status = START;
	int return_val = ERROR_OK;
	BOOL machine_stop = FALSE;
	while(TRUE)
	{
		// do action
		switch(current_status)
		{
		case START:
			return_val = app->ResetHttpContent();
			break;
		case LOGINV:
			return_val = app->GetLoginValPageAction();
			break;
		case RAND:
			return_val = app->GetRandPageAction();
			break;
		case LOGIN:
			return_val = app->GetLoginPageAction();
			// do something after login success
			if(return_val == ERROR_OK)
				app->m_pMainWnd->SendMessage(WM_LOGIN, 0, 0);
			break;
		case QUERY:
			return_val = app->GetQueryPageAction();
			break;
		case BOOK:
			return_val = app->GetBookPageAction();
			break;
		case BOOKV:
			return_val = app->GetBookValPageAction();
			break;
		case CHECK:
			return_val = app->GetCheckPageAction();
			break;
		case QUEUE:
			return_val = app->GetQueuePageAction();
			break;
		case CONFRM:
			return_val = app->GetConfirmPageAction();
			break;
		case WAIT:
			return_val = app->GetWaitPageAction();
			break;
		case EXIT:
			// fall through
		default:
			machine_stop = TRUE;
			break;
		}

		if(machine_stop == TRUE)
			break;

		// jump to next status
		STATUS next_status = state_table[current_status][- return_val];
		

		if(next_status == EXIT)
		{
			// log and exit
			CString statusMsg;
			statusMsg.Format(L"status machine exit, exit code: %d", current_status);
			app->SendString(statusMsg);
		}
		current_status = next_status;
	}
	

	return 0;
}

void CHTTPSWebClientApp::PrepareLoginValDlg()
{
	validationDialog.pic.imageAttrSet(loginValPage.height, loginValPage.width);
	validationDialog.pic.imageBuffSet(loginValPage.bmpbuff, loginValPage.bmpsize);
}

void CHTTPSWebClientApp::PrepareBookValDlg()
{
	validationDialog.pic.imageAttrSet(bookValPage.height, bookValPage.width);
	validationDialog.pic.imageBuffSet(bookValPage.bmpbuff, bookValPage.bmpsize);
}

void CHTTPSWebClientApp::GetLoginValCodeInput()
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
		CLog::GetLog().AddLog(L"user cancel validation code input, quit login");
	}
	// notify worker thread things have been complete
	SetEvent(valEvent);
}

void CHTTPSWebClientApp::GetBookValCodeInput()
{
	if(validationDialog.DoModal() == IDOK)
	{
		// OK, get code
		ticketInfo.randCode = validationDialog.code;
	}
	else
	{
		// cancel, quit
		ticketInfo.randCode.Empty();
		CLog::GetLog().AddLog(L"user cancel validation code input, quit login");
	}
	// notify worker thread things have been complete
	SetEvent(valEvent);
}

void CHTTPSWebClientApp::SendString( CString &msg )
{
	m_pMainWnd->SendMessage(WM_SETSTR, 0, (LPARAM)(LPCTSTR)msg);
}

int CHTTPSWebClientApp::GetLoginValPageAction()
{
	CString statusMsg;

	statusMsg.Format(L"request validation picture");
	SendString(statusMsg);

	loginValPage.BuildRequest();
	loginValPage.GetPageData(httpContent);
	loginValPage.ParseOutput();

	//Sleep(GENERAL_DELAY * 1000);

	int loginValStatus = loginValPage.GetStatus();

	if(loginValStatus != ERROR_OK)
	{
		statusMsg.Format(L"request validation picture failed, error code: %d", loginValStatus);
		SendString(statusMsg);
		return loginValStatus;
	}

	// send message to gui to get validation code from user input
	PrepareLoginValDlg();

	m_pMainWnd->SendMessage(WM_GETCODE, 0, 0);

	// wait for validation code to be retrieved
	WaitForSingleObject(valEvent, INFINITE);

	// check result
	if(loginInfo.validate.IsEmpty())
	{
		statusMsg.Format(L"user canceled this request, login abort");
		SendString(statusMsg);
		loginValStatus = ERROR_USER_CANCEL;
		return loginValStatus;
	}

	statusMsg.Format(L"get validation code: %s", loginInfo.validate);
	SendString(statusMsg);

	loginValStatus = ERROR_OK;
	return loginValStatus;
}

int CHTTPSWebClientApp::GetBookValPageAction()
{
	CString statusMsg;

	statusMsg.Format(L"get book validation code");
	SendString(statusMsg);

	bookValPage.BuildRequest();
	bookValPage.GetPageData(httpContent);
	bookValPage.ParseOutput();

	Sleep(GENERAL_DELAY * 1000);

	int bookValStatus = bookValPage.GetStatus();

	if(bookValStatus != ERROR_OK)
	{
		statusMsg.Format(L"request validation picture failed, error code: %d", bookValStatus);
		SendString(statusMsg);
		return bookValStatus;
	}
	// send message to gui to get validation code from user input
	PrepareBookValDlg();

	m_pMainWnd->SendMessage(WM_GETCODE, 0, 1);

	// wait for validation code to be retrieved
	WaitForSingleObject(valEvent, INFINITE);

	// check result
	if(ticketInfo.randCode.IsEmpty())
	{
		statusMsg.Format(L"user canceled this request, login abort");
		SendString(statusMsg);
		bookValStatus = ERROR_USER_CANCEL;
		return bookValStatus;
	}

	statusMsg.Format(L"get validation code: %s", ticketInfo.randCode);
	SendString(statusMsg);
	
	bookValStatus = ERROR_OK;
	return bookValStatus;
}

int CHTTPSWebClientApp::GetRandPageAction()
{
	CString statusMsg;

	statusMsg.Format(L"request login random number");
	SendString(statusMsg);

	// post request to get rand number
	randPage.BuildRequest();
	randPage.GetPageStr(httpContent);
	randPage.ParseOutput(loginInfo);

	//Sleep(GENERAL_DELAY * 1000);

	int loginRandStatus = randPage.GetStatus();
	if(loginRandStatus != ERROR_OK)
	{
		statusMsg.Format(L"request login rand failed, error code: %d", loginRandStatus);
		SendString(statusMsg);
	}

	return loginRandStatus;
}

int CHTTPSWebClientApp::GetLoginPageAction()
{
	CString statusMsg;

	statusMsg.Format(L"send login request");
	SendString(statusMsg);

	loginPage.BuildRequest(loginInfo);
	loginPage.GetPageStr(httpContent);
	loginPage.ParseOutput();

	//Sleep(GENERAL_DELAY * 1000);

	int loginStatus = loginPage.GetStatus();
	if( loginStatus == ERROR_OK)
	{
		statusMsg.Format(L"login success!");
		SendString(statusMsg);
	}
	else
	{
		statusMsg.Format(L"login failed, error code: %d", loginStatus);
		SendString(statusMsg);
	}
	return loginStatus;
}

int CHTTPSWebClientApp::GetQueryPageAction()
{
	CString statusMsg;

	statusMsg.Format(L"querying train...");
	SendString(statusMsg);

	CLog::GetLog().AddLog(L"query action start");
	queryPage.BuildRequest(queryInfo);
	queryPage.GetPageStr(httpContent);
	queryPage.ParseOutput(trainInfo);

	Sleep(GENERAL_DELAY * 1000);

	int queryStatus = queryPage.GetStatus();
	if(queryStatus == ERROR_OK)
	{
		statusMsg.Format(L"query success!");
		SendString(statusMsg);
	}
	else if(queryStatus == ERROR_HTTP)
	{
		statusMsg.Format(L"query page: http connection failed!");
		SendString(statusMsg);
	}
	else
	{
		// add query delay
		CLog::GetLog().AddLog(L"no proper train found, wait for retry");
	}

	return queryStatus;
}

int CHTTPSWebClientApp::GetBookPageAction()
{
	CString statusMsg;

	statusMsg.Format(L"booking tickets...");
	SendString(statusMsg);
	CLog::GetLog().AddLog(L"book action start");
	bookPage.BuildRequest(trainInfo);
	bookPage.GetPageStr(httpContent);
	bookPage.ParseOutput(ticketInfo);

	Sleep(GENERAL_DELAY * 1000);

	int bookStatus = bookPage.GetStatus();
	if(bookStatus == ERROR_OK)
	{
		statusMsg.Format(L"book success!");
		SendString(statusMsg);
	}
	else if(bookStatus == ERROR_HTTP)
	{
		statusMsg.Format(L"book page: http connection failed!");
		SendString(statusMsg);
	}
	else
	{
		statusMsg.Format(L"book failed: error code %d", bookStatus);
		SendString(statusMsg);
	}
	return bookStatus;
}

int CHTTPSWebClientApp::GetCheckPageAction()
{
	CString statusMsg;

	statusMsg.Format(L"checking order...");
	SendString(statusMsg);
	CLog::GetLog().AddLog(L"check action start");
	checkPage.BuildRequest(ticketInfo);
	checkPage.GetPageStr(httpContent);
	checkPage.ParseOutput(orderInfo);

	Sleep(GENERAL_DELAY * 1000);

	int checkStatus = checkPage.GetStatus();
	if(checkStatus == ERROR_OK)
	{
		statusMsg.Format(L"check success!");
		SendString(statusMsg);
	}
	else if(checkStatus == ERROR_VALIDATE)
	{
		statusMsg.Format(L"check failed: error code %d\r\nre-enter validation code!", checkStatus);
		SendString(statusMsg);
	}
	else if(checkStatus == ERROR_HTTP)
	{
		statusMsg.Format(L"check page: http connection failed!");
		SendString(statusMsg);
	}
	else
	{
		statusMsg.Format(L"check failed: error code %d", checkStatus);
		SendString(statusMsg);
	}

	return checkStatus;
}

int CHTTPSWebClientApp::GetQueuePageAction()
{
	CString statusMsg;

	statusMsg.Format(L"queueing order...");
	SendString(statusMsg);
	CLog::GetLog().AddLog(L"queue action start");
	queuePage.BuildRequest(ticketInfo);
	queuePage.GetPageStr(httpContent);
	queuePage.ParseOutput(orderInfo);

	Sleep(GENERAL_DELAY * 1000);

	int queueStatus = queuePage.GetStatus();
	if(queueStatus == ERROR_OK)
	{
		// update ticket info if success
		ticketInfo.leftTicketStr = orderInfo.ticketStr;

		statusMsg.Format(L"queue success!");
		SendString(statusMsg);
	}
	else if(queueStatus == ERROR_HTTP)
	{
		statusMsg.Format(L"queue page: http connection failed!");
		SendString(statusMsg);
	}
	else
	{
		statusMsg.Format(L"queue failed: error code %d", queueStatus);
		SendString(statusMsg);
	}

	return queueStatus;
}

int CHTTPSWebClientApp::GetConfirmPageAction()
{
	CString statusMsg;

	statusMsg.Format(L"confirming order...");
	SendString(statusMsg);
	CLog::GetLog().AddLog(L"confirm action start");
	confirmPage.BuildRequest(ticketInfo);
	confirmPage.GetPageStr(httpContent);
	confirmPage.ParseOutput(orderInfo);

	Sleep(GENERAL_DELAY * 1000);

	int confirmStatus = confirmPage.GetStatus();

	if(confirmStatus == ERROR_OK)
	{
		statusMsg.Format(L"confirm success!");
		SendString(statusMsg);
	}
	else if(confirmStatus == ERROR_HTTP)
	{
		statusMsg.Format(L"confirm page: http connection failed!");
		SendString(statusMsg);
	}
	else if(confirmStatus == ERROR_VALIDATE)
	{
		statusMsg.Format(L"confirm failed: error code %d\r\nre-enter validation code!", confirmStatus);
		SendString(statusMsg);
	}
	else
	{
		statusMsg.Format(L"confirm failed: error code %d", confirmStatus);
		SendString(statusMsg);
	}

	return confirmStatus;
}

int CHTTPSWebClientApp::GetWaitPageAction()
{
	CString statusMsg;

	statusMsg.Format(L"waiting for result...");
	SendString(statusMsg);
	CLog::GetLog().AddLog(L"wait action start");
	waitPage.BuildRequest(orderInfo);
	waitPage.GetPageStr(httpContent);
	waitPage.ParseOutput(orderInfo);

	Sleep(GENERAL_DELAY * 1000);

	int waitStatus = waitPage.GetStatus();
	if(waitStatus == ERROR_OK)
	{
		statusMsg.Format(L"buy ticket success!");
		SendString(statusMsg);
		if(!orderInfo.orderId.IsEmpty())
		{
			statusMsg.Format(L"order No is: %s", orderInfo.orderId);
			SendString(statusMsg);
		}
	}
	else if(waitStatus == ERROR_HTTP)
	{
		statusMsg.Format(L"wait page: http connection failed!");
		SendString(statusMsg);
	}
	else
	{
		statusMsg.Format(L"wait failed: error code %d", waitStatus);
		SendString(statusMsg);
	}

	return waitStatus;
}

int CHTTPSWebClientApp::ResetHttpContent()
{
	httpContent.ConnectInit(L"dynamic.12306.cn");
	return ERROR_OK;
}
