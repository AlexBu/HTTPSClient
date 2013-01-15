#include "StdAfx.h"
#include "Page.h"

CPage::CPage(void)
:reqStr(L"")
,reqData(L"")
,respStr(L"")
,status(-1)
,isGet(TRUE)
{
}

CPage::~CPage(void)
{
}

void CPage::GetPageData( CHTTPContent& content )
{
	if(isGet)
	{
		content.SendDatabyGet(reqStr);
	}
	else
	{
		content.SendDatabyPost(reqStr, reqData);
	}
	content.GetResponseStr(respStr);
}

int CPage::GetStatus()
{
	return status;
}
