#include "StdAfx.h"
#include "QueuePage.h"
#include "regex.h"

CQueuePage::CQueuePage(void)
{
}

CQueuePage::~CQueuePage(void)
{
}

void CQueuePage::BuildRequest( TicketInfo& input )
{
	reqStr.Format(L"/otsweb/order/confirmPassengerAction.do?method=getQueueCount"
		L"&train_date=%s"
		L"&train_no=%s"
		L"&station=%s"
		L"&seat=%s"
		L"&from=%s"
		L"&to=%s"
		L"&ticket=%s",
		input.train_date,
		input.train_no,
		input.station_train_code,
		input.passengers[0].seat,	// the first passenger's seat type
		input.from_station_telecode,
		input.to_station_telecode,
		input.leftTicketStr
		);
}

void CQueuePage::ParseOutput( OrderInfo& output )
{
	if(status == ERROR_HTTP)
		return;
	// split results
	CRegex regex;
	CString pattern, restStr, matchStr;

	matchStr = respStr;

	pattern = L"\\\"ticket\\\":{\\q}";
	regex.patternLoad(pattern);
	regex.contextMatch(matchStr, restStr);
	if(regex.matchCount() == 1)
	{
		status = 0;
		regex.matchGet(0, output.ticketStr);
	}
	else
	{
		status = -1;
	}
	matchStr = restStr;
	restStr.Empty();
}
