#include "StdAfx.h"
#include "ConfirmPage.h"
#include "regex.h"
#include "Log.h"

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
		status = ERROR_OK;
	}
	else if(output.errMsg.Find(L"·Ç·¨") != -1)
	{
		// logic error
		CLog::GetLog().AddLog(respStr);
		status = ERROR_LOGIC;
	}
	else
	{
		CLog::GetLog().AddLog(respStr);
		status = ERROR_GENERAL;
	}
}
