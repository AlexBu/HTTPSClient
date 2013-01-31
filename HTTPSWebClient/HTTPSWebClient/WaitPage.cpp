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
	reqStr = L"/otsweb/order/myOrderAction.do?method=queryOrderWaitTime&tourFlag=dc";
	refStr = L"/otsweb/order/confirmPassengerAction.do?method=init";
}

void CWaitPage::ParseOutput( OrderInfo& output )
{
	if(status != ERROR_OK)
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

		long wait_time = _tstol(output.waitTime);
		if(wait_time >= 0)
		{
			// continue waiting
			status = ERROR_IN_QUEUE;

			// print queue info
		}
		else
		{
			// time out
			status = ERROR_OK;

			// get request id info
			pattern = L"\\\"orderId\\\":\\\"{E\\d+}\\\"";
			regex.patternLoad(pattern);
			if( (regex.contextMatch(respStr, restStr) == TRUE) && (regex.matchCount() == 1) )
			{
				regex.matchGet(0, output.orderId);
			}
			else
			{
				output.orderId.Empty();
			}
		}
	}
	else
	{
		status = ERROR_GENERAL;
		CLog::GetLog().AddLog(L"general error!");
	}
}
