
// HTTPSWebClient.h : PROJECT_NAME Ӧ�ó������ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������


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