#include "StdAfx.h"
#include "WaitPage.h"
#include "regex.h"

CWaitPage::CWaitPage(void)
{
}

CWaitPage::~CWaitPage(void)
{
}

void CWaitPage::BuildRequest( OrderInfo& input )
{
	reqStr = L"/otsweb/order/myOrderAction.do?method=getOrderWaitTime";
}

void CWaitPage::ParseOutput( OrderInfo& output )
{
	// split results
	CRegex regex;
	CString pattern, restStr;

	pattern = L"\\\"orderId\\\":{\\q}";
	regex.patternLoad(pattern);
	regex.contextMatch(respStr, restStr);
	regex.matchGet(0, output.orderNo);

	if(output.orderNo.GetLength() > 0)
	{
		status = 0;
	}
	else
	{
		status = -1;
	}
}
