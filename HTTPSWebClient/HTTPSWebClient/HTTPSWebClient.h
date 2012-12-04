
// HTTPSWebClient.h : PROJECT_NAME Ӧ�ó������ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������
#include "ValidatePicture.h"
#include "regex.h"

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