
// HTTPSWebClient.h : PROJECT_NAME Ӧ�ó������ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������
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
// �йش����ʵ�֣������ HTTPSWebClient.cpp
//

class CHTTPSWebClientApp : public CWinAppEx
{
public:
	CHTTPSWebClientApp();

// ��д
	public:
	virtual BOOL InitInstance();

// ʵ��

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