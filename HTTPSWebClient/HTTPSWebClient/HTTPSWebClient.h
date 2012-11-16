
// HTTPSWebClient.h : PROJECT_NAME 应用程序的主头文件
//

#pragma once

#ifndef __AFXWIN_H__
	#error "在包含此文件之前包含“stdafx.h”以生成 PCH 文件"
#endif

#include "resource.h"		// 主符号


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
	void GetFromURL( const CString& URLString );
	void PostToURL(const CString& webResString, const CString& PostString);
	void GetValidatePic(const CString& ValPicAddr);
	void LoginToSite(const CString& usernameStr, const CString& passwordStr, const CString& validateStr);

	BOOL ConnectToURL( const CString& URLString );
	HINTERNET SendRequest(int verb, const CString& webResString, const BYTE* addtionData, const DWORD addtionSize);
	BOOL GetResponse(HINTERNET hRequest);
	DWORD GetValidBufferSize();
	LPVOID GetBufferData();

private:
	HINTERNET	hSession;
	HINTERNET	hConnect;
	BYTE*		picBuff;
	BYTE*		htmlResponceBuff;
	DWORD		htmlResponceSize;
};
extern CHTTPSWebClientApp theApp;