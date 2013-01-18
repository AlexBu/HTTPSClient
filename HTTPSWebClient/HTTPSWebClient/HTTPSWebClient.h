
// HTTPSWebClient.h : PROJECT_NAME Ӧ�ó������ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������
#include "ValidationCtrl.h"
#include "regex.h"
#include "HTTPContent.h"
#include "Info.h"
#include "LoginRandPage.h"
#include "LoginPage.h"
#include "QueryPage.h"
#include "BookPage.h"
#include "CheckPage.h"
#include "QueuePage.h"
#include "ConfirmPage.h"
#include "WaitPage.h"
#include "LoginValPage.h"

#include "Log.h"

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
	BOOL GetValidatePic(const CString& ValPicAddr, CValPicCtrl& picCtrl);
	void LoginToSite(CString& result);
	void ConnectToURL( const CString& URLString );
	void BookTickets(CString& result);

private:
	CHTTPContent httpContent;
	CLoginRandPage loginRandPage;
	CLoginPage loginPage;
	CQueryPage queryPage;
	CBookPage bookPage;
	CCheckPage checkPage;
	CQueuePage queuePage;
	CConfirmPage confirmPage;
	CWaitPage waitPage;
	CLoginValPage loginValPage;
public:
	LoginInfo loginInfo;
	QueryInfo queryInfo;
	TrainInfo trainInfo;
	TicketInfo ticketInfo;
	OrderInfo orderInfo;
private:
	//CLoginRandThread loginRandThread;
};
extern CHTTPSWebClientApp theApp;