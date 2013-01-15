
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

void CHTTPSWebClientApp::LoginToSite(const CString& usernameStr, 
									 const CString& passwordStr,
									 const CString& validateStr,
									 CString& result)
{
	// post request to get rand number
	loginRandPage.BuildRequest();
	loginRandPage.GetPageData(httpContent);
	loginRandPage.ParseOutput(loginInfo);

	if(loginRandPage.GetStatus() != 0)
	{
		result = L"log in failed: retrieve rand error.";
		return;
	}

	loginInfo.username = usernameStr;
	loginInfo.password = passwordStr;
	loginInfo.validate = validateStr;

	loginPage.BuildRequest(loginInfo);
	loginPage.GetPageData(httpContent);
	loginPage.ParseOutput();

	if(loginPage.GetStatus() == 0)
		result = L"log in success!";
	else
		result.Format(L"login failed: error code %d", loginPage.GetStatus());
	return;
}

void CHTTPSWebClientApp::QueryTickets(CString& date, 
									  CString& train, 
									  CString& stationFrom, 
									  CString& stationTo, 
									  CString& result,
									  CString& validateStr)
{
	result += L"\r\nquerying train...";

	queryInfo.departDate = date;
	trainInfo.trainCode = train;
	queryPage.BuildRequest(queryInfo);
	queryPage.GetPageData(httpContent);
	queryPage.ParseOutput(trainInfo);

	if(queryPage.GetStatus() != 0)
	{
		result.Format(L"query failed: error code %d", queryPage.GetStatus());
		return;
	}

	result += L"\r\nbooking tickets...";

	trainInfo.trainDate = date;
	trainInfo.trainRoundDate = date;
	bookPage.BuildRequest(trainInfo);
	bookPage.GetPageData(httpContent);
	bookPage.ParseOutput(ticketInfo);

	if(bookPage.GetStatus() != 0)
	{
		result.Format(L"book failed: error code %d", bookPage.GetStatus());
		return;
	}
	result += L"\r\nchecking order...";


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

	if(checkPage.GetStatus() != 0)
	{
		result.Format(L"check failed: error code %d", checkPage.GetStatus());
		return;
	}

	result += L"\r\nconfirming order...";

	confirmPage.BuildRequest(orderInfo);
	confirmPage.GetPageData(httpContent);
	confirmPage.ParseOutput(orderInfo);

	if(confirmPage.GetStatus() != 0)
	{
		result.Format(L"confirm failed: error code %d", confirmPage.GetStatus());
		return;
	}

	result += L"\r\nwaiting for result...";

	waitPage.BuildRequest(orderInfo);
	waitPage.GetPageData(httpContent);
	waitPage.ParseOutput(orderInfo);

	if(waitPage.GetStatus() != 0)
	{
		result.Format(L"wait failed: error code %d", waitPage.GetStatus());
		return;
	}
	Sleep(5 * 1000);

	waitPage.BuildRequest(orderInfo);
	waitPage.GetPageData(httpContent);
	waitPage.ParseOutput(orderInfo);

	if(waitPage.GetStatus() != 0)
	{
		result.Format(L"wait failed: error code %d", waitPage.GetStatus());
		return;
	}
	else
	{
		result.AppendFormat(L"\r\norder no = %s", orderInfo.orderNo);
	}
}