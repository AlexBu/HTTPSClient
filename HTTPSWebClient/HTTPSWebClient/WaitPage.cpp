#include "StdAfx.h"
#include "WaitPage.h"
#include "regex.h"

CWaitPage::CWaitPage(void)
:reqStr(L"")
,respStr(L"")
{
}

CWaitPage::~CWaitPage(void)
{
}

void CWaitPage::BuildRequest( OrderInfo& input )
{
	reqStr = L"/otsweb/order/myOrderAction.do?method=getOrderWaitTime";
}

void CWaitPage::GetPageData( CHTTPContent& content )
{
	content.SendDatabyGet(reqStr);
	content.GetResponseStr(respStr);
}

void CWaitPage::ParseOutput( OrderInfo& output )
{
	// split results
	CRegex regex;
	CString pattern, restStr, matchStr;

	matchStr = respStr;

	pattern = L"\\\"orderId\\\":{\\q}";
	regex.patternLoad(pattern);
	regex.contextMatch(matchStr, restStr);
	regex.matchGet(0, output.orderNo);
	matchStr = restStr;
	restStr.Empty();

	if(output.orderNo.GetLength() > 0)
	{
		// return ok status
	}
	else
	{
		//
	}
}
