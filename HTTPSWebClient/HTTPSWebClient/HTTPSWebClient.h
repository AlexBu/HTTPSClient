
// HTTPSWebClient.h : PROJECT_NAME 应用程序的主头文件
//

#pragma once

#ifndef __AFXWIN_H__
	#error "在包含此文件之前包含“stdafx.h”以生成 PCH 文件"
#endif

#include "resource.h"		// 主符号
#include "ValidationCtrl.h"
#include "regex.h"
#include "HTTPContent.h"
#include "Info.h"
#include "RandPage.h"
#include "LoginPage.h"
#include "QueryPage.h"
#include "BookPage.h"
#include "CheckPage.h"
#include "QueuePage.h"
#include "ConfirmPage.h"
#include "WaitPage.h"
#include "LoginValPage.h"
#include "BookValPage.h"

#include "ValidationDialog.h"

#include "Log.h"

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
	BOOL GetValidatePic(const CString& ValPicAddr, CValPicCtrl& picCtrl);
	void ConnectToURL( const CString& URLString );
	void BookTickets(CString& result);

private:
	CHTTPContent httpContent;
	CRandPage randPage;
	CLoginPage loginPage;
	CQueryPage queryPage;
	CBookPage bookPage;
	CCheckPage checkPage;
	CQueuePage queuePage;
	CConfirmPage confirmPage;
	CWaitPage waitPage;
	CLoginValPage loginValPage;
	CBookValPage bookValPage;
public:
	LoginInfo loginInfo;
	QueryInfo queryInfo;
	TrainInfo trainInfo;
	TicketInfo ticketInfo;
	OrderInfo orderInfo;
public:
	CValidationDialog validationDialog;
private:
	CEvent valEvent;
public:
	static UINT AFX_CDECL bookWorker(LPVOID param);

	void SendString(CString &msg);

	void PrepareLoginValDlg();
	void PrepareBookValDlg();
	void GetLoginValCodeInput();
	void GetBookValCodeInput();

	int GetLoginValPageAction();
	int GetBookValPageAction();
	int GetRandPageAction();
	int GetLoginPageAction();
	int GetQueryPageAction();
	int GetBookPageAction();
	int GetCheckPageAction();
	int GetQueuePageAction();
	int GetConfirmPageAction();
	int GetWaitPageAction();
};
extern CHTTPSWebClientApp theApp;