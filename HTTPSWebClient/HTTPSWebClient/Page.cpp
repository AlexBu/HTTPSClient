#include "StdAfx.h"
#include "Page.h"
#include "Log.h"

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
	CLog& log = CLog::GetLog();

	if(isGet)
	{
		content.SendDatabyGet(reqStr);
		log.AddLog(reqStr);
	}
	else
	{
		content.SendDatabyPost(reqStr, reqData);
		log.AddLog(reqStr);
		log.AddLog(reqData);
	}
	content.GetResponseStr(respStr);
	log.AddLog(respStr);
}

int CPage::GetStatus()
{
	return status;
}

CString CPage::GetResponse()
{
	return respStr;
}
