
// pic_trans.h : PROJECT_NAME Ӧ�ó������ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������


// Cpic_transApp:
// �йش����ʵ�֣������ pic_trans.cpp
//

class Cpic_transApp : public CWinAppEx
{
public:
	Cpic_transApp();

// ��д
	public:
	virtual BOOL InitInstance();

// ʵ��

	DECLARE_MESSAGE_MAP()
};

extern Cpic_transApp theApp;