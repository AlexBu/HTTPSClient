#include "StdAfx.h"
#include "WaitPage.h"

CWaitPage::CWaitPage(void)
{
}

CWaitPage::~CWaitPage(void)
{
}

void CWaitPage::BuildRequest( OrderInfo& input )
{
	reqStr = L"/otsweb/order/myOrderAction.do?method=queryOrderWaitTime";
	refStr = L"Referer: https://dynamic.12306.cn/otsweb/order/confirmPassengerAction.do?method=init";
}

void CWaitPage::ParseOutput( OrderInfo& output )
{
	if(status == ERROR_HTTP)
		return;
	// split results
	CRegex regex;
	CString pattern, restStr;

	pattern = L"\\\"waitTime\\\":{-?\\d+}";
	regex.patternLoad(pattern);
	if( (regex.contextMatch(respStr, restStr) == TRUE) && (regex.matchCount() == 1) )
	{
		regex.matchGet(0, output.waitTime);
		status = 0;
		CLog::GetLog().AddLog(L"wait page success!");
		CLog::GetLog().AddLog(output.waitTime);
	}
	else
	{
		status = -1;
		CLog::GetLog().AddLog(L"general error!");
	}
}
