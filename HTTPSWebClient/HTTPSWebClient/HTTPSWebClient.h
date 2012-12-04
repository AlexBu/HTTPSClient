
// HTTPSWebClient.h : PROJECT_NAME 应用程序的主头文件
//

#pragma once

#ifndef __AFXWIN_H__
	#error "在包含此文件之前包含“stdafx.h”以生成 PCH 文件"
#endif

#include "resource.h"		// 主符号
#include "ValidatePicture.h"
#include "regex.h"

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
	BOOL GetValidatePic(const CString& ValPicAddr, CValPic& picCtrl);
	void LoginToSite(const CString& usernameStr, const CString& passwordStr, const CString& validateStr);
	BOOL ConnectToURL( const CString& URLString );
	HINTERNET SendRequest(int verb, const CString& refererStr, const CString& acceptTypStr, const CString& webResStr, const BYTE* addtionData, const DWORD addtionSize);
	BOOL GetResponse(HINTERNET hRequest);
	DWORD GetValidBufferSize();
	LPVOID GetBufferData();
	void QueryTickets();
private:
	void ConvertToUTF();
	BOOL convertToBMP(unsigned int *bmpWidth,unsigned int *bmpHeight,DWORD *bmpSize);
private:
	HINTERNET	hSession;
	HINTERNET	hConnect;
	BYTE*		picBuff;
	BYTE*		htmlResponseBuff;
	DWORD		htmlResponseSize;
	CString		htmlResponseStr;
	CRegex		regex;
};
extern CHTTPSWebClientApp theApp;