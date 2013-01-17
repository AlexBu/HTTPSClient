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
	else if(output.errMsg.Find(L"非法") != -1)
	{
		// logic error
		CLog::GetLog().AddLog(L"illegal request!");
		status = ERROR_LOGIC;
	}
	else if(output.errMsg.Find(L"输入的验证码不正确") != -1)
	{
		CLog::GetLog().AddLog(L"wrong validate code!");
		status = ERROR_VALIDATE;
	}
	else
	{
		CLog::GetLog().AddLog(output.errMsg);
		status = ERROR_GENERAL;
	}
}
