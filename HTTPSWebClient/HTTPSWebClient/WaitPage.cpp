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
	if(status == ERROR_HTTP)
		return;
	// split results
	CRegex regex;
	CString pattern, restStr;

	pattern = L"\\\"waitTime\\\":\\d+";
	regex.patternLoad(pattern);
	regex.contextMatch(respStr, restStr);
	regex.matchGet(0, output.waitTime);
	long time = _ttol(output.waitTime);

	if(time > 0)
	{
		status = 0;
	}
	else
	{
		status = -1;
	}
}
