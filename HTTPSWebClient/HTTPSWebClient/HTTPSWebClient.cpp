
// HTTPSWebClient.cpp : 定义应用程序的类行为。
//

#include "stdafx.h"
#include "HTTPSWebClient.h"
#include "HTTPSWebClientDlg.h"

#include "jpeglib.h"
#include "jpg.h"

#include <atlrx.h>

#include "LoginRandPageIn.h"
#include "LoginRandPageOut.h"
#include "LoginRandPage.h"

#include "LoginPageIn.h"
#include "LoginPageOut.h"
#include "LoginPage.h"

#include "QueryPage.h"
#include "BookPage.h"
#include "CheckPage.h"
#include "ConfirmPage.h"
#include "WaitPage.h"

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

void CHTTPSWebClientApp::GetFromURL( const CString& webResString, CString& result )
{
	httpContent.SendDatabyGet(webResString);
	httpContent.GetResponseStr(result);
}

void CHTTPSWebClientApp::PostToURL(const CString& webResString, const CString& PostString, CString& result)
{
	httpContent.SendDatabyPost(webResString, PostString);
	httpContent.GetResponseStr(result);
}

BOOL CHTTPSWebClientApp::GetValidatePic(const CString& ValPicAddr, CValPic& picCtrl)
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

void CHTTPSWebClientApp::LoginToSite(const CString& usernameStr, 
									 const CString& passwordStr,
									 const CString& validateStr,
									 CString& result)
{
	// post request to get rand number
	CLoginRandPageIn loginRandPageIn;
	CLoginRandPageOut loginRandPageOut;
	CLoginRandPage loginRandPage;

	loginRandPage.CollectInput(loginRandPageIn);
	loginRandPage.GetPageData(httpContent);
	loginRandPage.ParseOutput(loginRandPageOut);

	CLoginPageIn loginPageIn;
	CLoginPageOut loginPageOut;
	CLoginPage loginPage;

	loginPageIn.usernameSet(usernameStr);
	loginPageIn.passwordSet(passwordStr);
	loginPageIn.validateSet(validateStr);
	loginPageIn.randSet(loginRandPageOut);

	loginPage.CollectInput(loginPageIn);
	loginPage.GetPageData(httpContent);
	loginPage.ParseOutput(loginPageOut);

	if(loginPageOut.loginGet() == TRUE)
	{
		result = L"log in success!";
	}

	return;
}

void CHTTPSWebClientApp::QueryTickets(CString& date, 
									  CString& train, 
									  CString& stationFrom, 
									  CString& stationTo, 
									  CString& result,
									  CString& validateStr)
{
	QueryInfo queryInfo;
	CQueryPage queryPage;
	TrainInfo trainInfo;

	result += L"\rquerying train...";

	queryInfo.departDate = date;
	trainInfo.trainCode = train;
	queryPage.BuildRequest(queryInfo);
	queryPage.GetPageData(httpContent);
	queryPage.ParseOutput(trainInfo);

	result += L"\rbooking tickets...";

	CBookPage bookPage;
	TicketInfo ticketInfo;

	trainInfo.trainDate = date;
	trainInfo.trainRoundDate = date;
	bookPage.BuildRequest(trainInfo);
	bookPage.GetPageData(httpContent);
	bookPage.ParseOutput(ticketInfo);

	result += L"\rchecking order...";

	CCheckPage checkPage;
	OrderInfo orderInfo;

	ticketInfo.randCode =  validateStr;
	ticketInfo.passengers[0].seat = passengerInfo[0].seat;
	ticketInfo.passengers[0].seat_detail = passengerInfo[0].seat_detail;
	ticketInfo.passengers[0].ticket = passengerInfo[0].ticket;
	ticketInfo.passengers[0].name = passengerInfo[0].name;
	ticketInfo.passengers[0].cardtype = passengerInfo[0].cardtype;
	ticketInfo.passengers[0].cardno = passengerInfo[0].cardno;
	ticketInfo.passengers[0].mobileno = passengerInfo[0].mobileno;
	ticketInfo.passengers[1].seat = passengerInfo[1].seat;
	ticketInfo.passengers[1].seat_detail = passengerInfo[1].seat_detail;
	ticketInfo.passengers[1].ticket = passengerInfo[1].ticket;
	ticketInfo.passengers[1].name = passengerInfo[1].name;
	ticketInfo.passengers[1].cardtype = passengerInfo[1].cardtype;
	ticketInfo.passengers[1].cardno = passengerInfo[1].cardno;
	ticketInfo.passengers[1].mobileno = passengerInfo[1].mobileno;

	checkPage.BuildRequest(ticketInfo);
	checkPage.GetPageData(httpContent);
	checkPage.ParseOutput(orderInfo);

	result += L"\rconfirming order...";

	CConfirmPage confirmPage;
	confirmPage.BuildRequest(orderInfo);
	confirmPage.GetPageData(httpContent);
	confirmPage.ParseOutput(orderInfo);

	result += L"\rwaiting for result...";

	CWaitPage waitPage;
	waitPage.BuildRequest(orderInfo);
	waitPage.GetPageData(httpContent);
	waitPage.ParseOutput(orderInfo);

	Sleep(5 * 1000);

	waitPage.BuildRequest(orderInfo);
	waitPage.GetPageData(httpContent);
	waitPage.ParseOutput(orderInfo);

	// organize the output
	result.AppendFormat(L"\rorder no = %s", orderInfo.orderNo);

	return;
}

void CHTTPSWebClientApp::BookTickets( CString& date )
{
	// precondition: login
	// steps:
	// 1. query train, with loop format
	// 2. book ticket
	// 3. confirm passenger
	// 4. wait for result
	// 5. return result

	//CTestTrainNoInfo test_trainNoInfo;
	//while(queryTrainNo(test_trainNoInfo) != TRUE)
	//	;

	//CTestTicketInfo test_ticketInfo;

	//bookTicket(test_ticketInfo)

	//confirmPassenger();

	//waitForResult();

	//returnResult();
}
