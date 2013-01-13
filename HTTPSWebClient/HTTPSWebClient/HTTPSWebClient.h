
// HTTPSWebClient.h : PROJECT_NAME Ӧ�ó������ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������
#include "ValidatePicture.h"
#include "regex.h"
#include "QueryPage.h"
#include "HTTPContent.h"

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
	void GetFromURL( const CString& URLString, CString& result );
	void PostToURL(const CString& webResString, const CString& PostString, CString& result);
	BOOL GetValidatePic(const CString& ValPicAddr, CValPic& picCtrl);
	void LoginToSite(const CString& usernameStr, const CString& passwordStr, const CString& validateStr, CString& result);
	void ConnectToURL( const CString& URLString );
	void QueryTickets(CString& date, CString& train, CString& stationFrom, CString& stationTo, CString& result);
	void BookTickets(CString& date);

private:
	CRegex		regex;
	CQueryPage	ticketInfo;
	CHTTPContent httpContent;
};
extern CHTTPSWebClientApp theApp;