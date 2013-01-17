
// HTTPSWebClient.cpp : ����Ӧ�ó��������Ϊ��
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


// CHTTPSWebClientApp ����

CHTTPSWebClientApp::CHTTPSWebClientApp()
{
	// TODO: �ڴ˴���ӹ�����룬
	// ��������Ҫ�ĳ�ʼ�������� InitInstance ��
}


// Ψһ��һ�� CHTTPSWebClientApp ����

CHTTPSWebClientApp theApp;


// CHTTPSWebClientApp ��ʼ��

BOOL CHTTPSWebClientApp::InitInstance()
{
	// ���һ�������� Windows XP �ϵ�Ӧ�ó����嵥ָ��Ҫ
	// ʹ�� ComCtl32.dll �汾 6 ����߰汾�����ÿ��ӻ���ʽ��
	//����Ҫ InitCommonControlsEx()�����򣬽��޷��������ڡ�
	INITCOMMONCONTROLSEX InitCtrls;
	InitCtrls.dwSize = sizeof(InitCtrls);
	// ��������Ϊ��������Ҫ��Ӧ�ó�����ʹ�õ�
	// �����ؼ��ࡣ
	InitCtrls.dwICC = ICC_WIN95_CLASSES;
	InitCommonControlsEx(&InitCtrls);

	CWinAppEx::InitInstance();

	// ��׼��ʼ��

	CHTTPSWebClientDlg dlg;
	m_pMainWnd = &dlg;
	INT_PTR nResponse = dlg.DoModal();
	if (nResponse == IDOK)
	{
		// TODO: �ڴ˷��ô����ʱ��
		//  ��ȷ�������رնԻ���Ĵ���
	}
	else if (nResponse == IDCANCEL)
	{
		// TODO: �ڴ˷��ô����ʱ��
		//  ��ȡ�������رնԻ���Ĵ���
	}

	// ���ڶԻ����ѹرգ����Խ����� FALSE �Ա��˳�Ӧ�ó���
	//  ����������Ӧ�ó������Ϣ�á�

	return FALSE;
}

void CHTTPSWebClientApp::ConnectToURL( const CString& URLString )
{
	httpContent.ConnectSite(URLString);
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

void CHTTPSWebClientApp::LoginToSite(CString& result)
{
	// post request to get rand number
	loginRandPage.BuildRequest();
	loginRandPage.GetPageData(httpContent);
	loginRandPage.ParseOutput(loginInfo);

	if(loginRandPage.GetStatus() != 0)
	{
		result = L"log in failed: get rand error.";
		return;
	}

	loginPage.BuildRequest(loginInfo);
	loginPage.GetPageData(httpContent);
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
	queryPage.BuildRequest(queryInfo);

QUERY:
	do{
		queryPage.GetPageData(httpContent);
		queryPage.ParseOutput(trainInfo);
		if(queryPage.GetStatus() == ERROR_HTTP)
		{
			result += L"\r\nquery page: http connection failed!";
			return;
		}
		// add delay
		Sleep(1 * 1000);
	} while (queryPage.GetStatus() != 0);

	result += L"\r\nbooking tickets...";
	bookPage.BuildRequest(trainInfo);
	bookPage.GetPageData(httpContent);
	bookPage.ParseOutput(ticketInfo);

	if(bookPage.GetStatus() != 0)
	{
		result.AppendFormat(L"\r\nbook failed: error code %d", bookPage.GetStatus());
		goto QUERY;
	}

	result += L"\r\nchecking order...";
	checkPage.BuildRequest(ticketInfo);
	checkPage.GetPageData(httpContent);
	checkPage.ParseOutput(orderInfo);

	if(checkPage.GetStatus() != 0)
	{
		result.AppendFormat(L"\r\ncheck failed: error code %d", checkPage.GetStatus());
		goto QUERY;
	}

	result += L"\r\nconfirming order...";
	queuePage.BuildRequest(ticketInfo);
	queuePage.GetPageData(httpContent);
	queuePage.ParseOutput(orderInfo);
	
	if(queuePage.GetStatus() != 0)
	{
		result.AppendFormat(L"\r\nqueue failed: error code %d", queuePage.GetStatus());
		goto QUERY;
	}

	result += L"\r\nqueueing order...";
	confirmPage.BuildRequest(orderInfo);
	confirmPage.GetPageData(httpContent);
	confirmPage.ParseOutput(orderInfo);

	if(confirmPage.GetStatus() != 0)
	{
		result.AppendFormat(L"\r\nconfirm failed: error code %d", confirmPage.GetStatus());
		goto QUERY;
	}

	result += L"\r\nwaiting for result...";
	waitPage.BuildRequest(orderInfo);
	waitPage.GetPageData(httpContent);
	waitPage.ParseOutput(orderInfo);

	if(waitPage.GetStatus() != 0)
	{
		result.AppendFormat(L"\r\nwait failed: error code %d", waitPage.GetStatus());
	}
	else
	{
		result += L"\r\nbuy ticket success!";
	}
}