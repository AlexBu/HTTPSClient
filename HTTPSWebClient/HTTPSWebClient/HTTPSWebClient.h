
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
};

extern CHTTPSWebClientApp theApp;