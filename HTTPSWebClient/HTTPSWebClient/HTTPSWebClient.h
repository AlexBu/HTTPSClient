
// HTTPSWebClient.h : PROJECT_NAME Ӧ�ó������ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������
#include "ValidationCtrl.h"
#include "regex.h"
#include "HTTPConnection.h"
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
	void BookTickets();
private:
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
	static UINT AFX_CDECL BookWorker(LPVOID param);

	void SendString(CString &msg);

	void PrepareLoginValDlg();
	void PrepareBookValDlg();
	void GetLoginValCodeInput();
	void GetBookValCodeInput();

	int GetLoginValPageAction(CHTTPConnection& httpContent);
	int GetBookValPageAction(CHTTPConnection& httpContent);
	int GetRandPageAction(CHTTPConnection& httpContent);
	int GetLoginPageAction(CHTTPConnection& httpContent);
	int GetQueryPageAction(CHTTPConnection& httpContent);
	int GetBookPageAction(CHTTPConnection& httpContent);
	int GetCheckPageAction(CHTTPConnection& httpContent);
	int GetQueuePageAction(CHTTPConnection& httpContent);
	int GetConfirmPageAction(CHTTPConnection& httpContent);
	int GetWaitPageAction(CHTTPConnection& httpContent);

	int ResetHttpContent(CHTTPConnection& httpContent);
	void CDNListProbe( CHTTPConnection& httpContent );

};
extern CHTTPSWebClientApp theApp;