#include "StdAfx.h"
#include "ConfirmPage.h"

CConfirmPage::CConfirmPage(void)
{
	isGet = FALSE;
}

CConfirmPage::~CConfirmPage(void)
{
}

void CConfirmPage::BuildRequest( OrderInfo& input )
{
	reqStr = L"/otsweb/order/confirmPassengerAction.do?method=confirmSingleForQueueOrder";
	reqData = input.orderInfo;
}

void CConfirmPage::ParseOutput( OrderInfo& output )
{
	if(status == ERROR_HTTP)
		return;
	// split results
	CRegex regex;
	CString pattern, restStr, matchStr;

	matchStr = respStr;

	pattern = L"\\{\\\"errMsg\\\":{\\q}\\}";
	regex.patternLoad(pattern);
	regex.contextMatch(matchStr, restStr);
	regex.matchGet(0, output.errMsg);
	matchStr = restStr;
	restStr.Empty();

	if(output.errMsg == L"\"Y\"")
	{
		CLog::GetLog().AddLog(L"confirm page success!");
		status = ERROR_OK;
	}
	else
	{
		CLog::GetLog().AddLog(output.errMsg);
		status = ERROR_GENERAL;
	}
}
