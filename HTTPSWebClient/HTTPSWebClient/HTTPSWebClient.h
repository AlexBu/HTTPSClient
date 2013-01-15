
// HTTPSWebClient.h : PROJECT_NAME 应用程序的主头文件
//

#pragma once

#ifndef __AFXWIN_H__
	#error "在包含此文件之前包含“stdafx.h”以生成 PCH 文件"
#endif

#include "resource.h"		// 主符号
#include "ValidatePicture.h"
#include "regex.h"
#include "HTTPContent.h"
#include "Info.h"
#include "LoginRandPage.h"
#include "LoginPage.h"
#include "QueryPage.h"
#include "BookPage.h"
#include "CheckPage.h"
#include "ConfirmPage.h"
#include "WaitPage.h"

// CHTTPSWebClientApp:
// 有关此类的实现，请参阅 HTTPSWebClient.cpp
//

class CHTTPSWebClientApp : public CWinAppEx
{
public:
	CHTTPSWebClientApp();

// 重写
	public:
	virtual BOOL InitInstance();

// 实现

	DECLARE_MESSAGE_MAP()
public:
	BOOL GetValidatePic(const CString& ValPicAddr, CValPic& picCtrl);
	void LoginToSite(const CString& usernameStr, const CString& passwordStr, const CString& validateStr, CString& result);
	void ConnectToURL( const CString& URLString );
	void QueryTickets(CString& date, CString& train, CString& stationFrom, CString& stationTo, CString& result, CString& validateStr);

private:
	CHTTPContent httpContent;
	LoginInfo loginInfo;
	CLoginRandPage loginRandPage;
	CLoginPage loginPage;
	QueryInfo queryInfo;
	CQueryPage queryPage;
	TrainInfo trainInfo;
	CBookPage bookPage;
	TicketInfo ticketInfo;
	CCheckPage checkPage;
	OrderInfo orderInfo;
	CConfirmPage confirmPage;
	CWaitPage waitPage;
public:
	PassengerInfo passengerInfo[5];
};
extern CHTTPSWebClientApp theApp;