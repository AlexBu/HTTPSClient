
// HTTPSWebClient.cpp : ����Ӧ�ó��������Ϊ��
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
