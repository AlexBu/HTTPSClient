
// HTTPSWebClient.h : PROJECT_NAME 应用程序的主头文件
//

#pragma once

#ifndef __AFXWIN_H__
	#error "在包含此文件之前包含“stdafx.h”以生成 PCH 文件"
#endif

#include "resource.h"		// 主符号
#include "ValidatePicture.h"
#include "regex.h"
#include "QueryPage.h"
#include "HTTPContent.h"

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