#include "StdAfx.h"
#include "ConfirmPage.h"
#include "regex.h"

CConfirmPage::CConfirmPage(void)
:reqStr(L"")
,reqData(L"")
,respStr(L"")
{
}

CConfirmPage::~CConfirmPage(void)
{
}

void CConfirmPage::BuildRequest( OrderInfo& input )
{
	reqStr = L"/otsweb/order/confirmPassengerAction.do?method=confirmSingleForQueueOrder";
	reqData = input.orderInfo;
}

void CConfirmPage::GetPageData( CHTTPContent& content )
{
	content.SendDatabyPost(reqStr, reqData);
	content.GetResponseStr(respStr);
}

void CConfirmPage::ParseOutput( OrderInfo& output )
{
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

	if(output.errMsg == L"Y")
	{
		// return ok status
	}
}
