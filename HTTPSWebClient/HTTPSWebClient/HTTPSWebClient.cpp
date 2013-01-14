
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
									  CString& result)
{
	// build up a test information
	CString queryStr;
	CString queryRespStr;

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

	httpContent.SendDatabyGet(queryStr);
	httpContent.GetResponseStr(queryRespStr);

	// split results
	CString pattern = L"javascript:getSelected\\(\\'"
		L"{[^#]+}#{[^#]+}#{[^#]+}#{[^#]+}#{[^#]+}#{[^#]+}#"
		L"{[^#]+}#{[^#]+}#{[^#]+}#{[^#]+}#{[^#]+}#{[^#]+}#{[^#]+}#{[^#\\']+}\\')>";
	regex.patternLoad(pattern);

	CString restStr;
	CString matchStr;

	matchStr = queryRespStr;

	// get the first record
	while(regex.contextMatch(matchStr, restStr))
	{
		if(regex.matchCount() == 14)
		{
			CString tempStr;

			regex.matchGet(0, tempStr);
			if(tempStr == train)
				break;

			ticketInfo.trainCodeSet(tempStr);

			regex.matchGet(1, tempStr);
			ticketInfo.durationSet(tempStr);

			regex.matchGet(2, tempStr);
			ticketInfo.trainStartTimeSet(tempStr);

			regex.matchGet(3, tempStr);
			ticketInfo.trainNoSet(tempStr);

			regex.matchGet(4, tempStr);
			ticketInfo.stationFromCodeSet(tempStr);

			regex.matchGet(5, tempStr);
			ticketInfo.stationToCodeSet(tempStr);

			regex.matchGet(6, tempStr);
			ticketInfo.timeArriveSet(tempStr);

			regex.matchGet(7, tempStr);
			ticketInfo.stationFromTeNameSet(tempStr);
			ticketInfo.stationFromNameSet(tempStr);

			regex.matchGet(8, tempStr);
			ticketInfo.stationToTeNameSet(tempStr);
			ticketInfo.stationToNameSet(tempStr);

			regex.matchGet(9, tempStr);
			ticketInfo.stationFromNoSet(tempStr);

			regex.matchGet(10, tempStr);
			ticketInfo.stationToNoSet(tempStr);

			regex.matchGet(11, tempStr);
			ticketInfo.infoDetailSet(tempStr);

			regex.matchGet(12, tempStr);
			ticketInfo.mmStrSet(tempStr);

			regex.matchGet(13, tempStr);
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

		}

		matchStr = restStr;
		restStr.Empty();
	}

	// test on the first result
	// build login string
	CString bookTrainAdr, bookTrainStr;
	bookTrainAdr = _T("/otsweb/order/querySingleAction.do?method=submutOrderRequest");
	ticketInfo.strBuild(bookTrainStr);

	httpContent.SendDatabyPost(bookTrainAdr, bookTrainStr);
	httpContent.GetResponseStr(queryRespStr);

	// organize the output
	result = queryRespStr;

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
