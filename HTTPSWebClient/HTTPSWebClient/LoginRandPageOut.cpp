#include "StdAfx.h"
#include "LoginRandPageOut.h"

CLoginRandPageOut::CLoginRandPageOut(void)
:rand(L"")
{
}

CLoginRandPageOut::~CLoginRandPageOut(void)
{
}

void CLoginRandPageOut::randSet( CString& str )
{
	rand = str;
}

CString CLoginRandPageOut::randGet()
{
	return rand;
}
