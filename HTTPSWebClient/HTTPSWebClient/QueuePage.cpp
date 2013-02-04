#include "StdAfx.h"
#include "QueuePage.h"

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
		input.passengers[0].seatTyp,	// the first passenger's seat type
		input.from_station_telecode,
		input.to_station_telecode,
		input.leftTicketStr
		);
	refStr = L"/otsweb/order/confirmPassengerAction.do?method=init";
}

void CQueuePage::ParseOutput( OrderInfo& output )
{
	if(status != ERROR_OK)
		return;
	// split results
	CRegex regex;
	CString pattern, restStr, matchStr;

	pattern = L"\\\"op_2\\\":{\\w}";
	regex.patternLoad(pattern);
	matchStr = respStr;
	if(regex.contextMatch(matchStr, restStr) == TRUE)
	{
		regex.matchGet(0, output.isQueue);
		if(output.isQueue == L"true")
		{
			status = ERROR_OVERLOAD;
			return;
		}
	}

	pattern = L"\\\"countT\\\":{\\d+}";
	regex.patternLoad(pattern);
	matchStr = respStr;
	if(regex.contextMatch(matchStr, restStr) == TRUE)
	{
		regex.matchGet(0, output.queueNo);
	}

	pattern = L"\\\"ticket\\\":\\\"{[^\\\"]+}\\\"";
	regex.patternLoad(pattern);
	matchStr = respStr;
	if(regex.contextMatch(matchStr, restStr) == TRUE)
	{
		status = ERROR_OK;
		regex.matchGet(0, output.ticketStr);
		CLog::GetLog().AddLog(L"queue page success!");
	}
	else
	{
		status = ERROR_GENERAL;
		CLog::GetLog().AddLog(L"general error!");
	}

}
