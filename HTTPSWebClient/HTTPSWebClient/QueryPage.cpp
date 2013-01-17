#include "StdAfx.h"
#include "QueryPage.h"

CQueryPage::CQueryPage(void)
{
}

CQueryPage::~CQueryPage(void)
{
}

void CQueryPage::BuildRequest( QueryInfo& input )
{
	reqStr.Format( L"/otsweb/order/querySingleAction.do?method=queryLeftTicket"
		L"&orderRequest.train_date=%s"
		L"&orderRequest.from_station_telecode=%s"
		L"&orderRequest.to_station_telecode=%s"
		L"&orderRequest.train_no="
		L"&trainPassType=QB"
		L"&trainClass=QB%%23D%%23Z%%23T%%23K%%23QT%%23"
		L"&includeStudent=00"
		L"&seatTypeAndNum="
		L"&orderRequest.start_time_str=00%%3A00--24%%3A00",
		input.departDate,
		from_station_telecodeGet(input),
		to_station_telecodeGet(input)
		);
}

void CQueryPage::ParseOutput( TrainInfo& output )
{
	if(status == ERROR_HTTP)
		return;
	// split results
	CRegex regex;
	CString pattern = L"javascript:getSelected\\(\\'"
		L"{[^#]+}#{[^#]+}#{[^#]+}#{[^#]+}#{[^#]+}#{[^#]+}#"
		L"{[^#]+}#{[^#]+}#{[^#]+}#{[^#]+}#{[^#]+}#{[^#]+}#{[^#]+}#{[^#\\']+}\\')>";
	regex.patternLoad(pattern);

	CString restStr;
	CString matchStr;

	matchStr = respStr;

	status = -1;
	while(regex.contextMatch(matchStr, restStr))
	{
		if(regex.matchCount() == 14)
		{
			CString groupStr;
			regex.matchGet(0, groupStr);
			if(output.trainCode == groupStr)
			{
				regex.matchGet(1, output.duration);
				regex.matchGet(2, output.trainStartTime);
				regex.matchGet(3, output.trainNo);
				regex.matchGet(4, output.stationFromCode);
				regex.matchGet(5, output.stationToCode);
				regex.matchGet(6, output.timeArrive);
				regex.matchGet(7, output.stationFromName);
				regex.matchGet(8, output.stationToName);
				regex.matchGet(9, output.stationFromNo);
				regex.matchGet(10, output.stationToNo);
				regex.matchGet(11, output.infoDetail);
				regex.matchGet(12, output.mmStr);
				regex.matchGet(13, output.locationCode);

				status = 0;
				break;
			}
		}
		matchStr = restStr;
		restStr.Empty();
	}
	if(status == ERROR_OK)
	{
		CLog::GetLog().AddLog(L"query page success!");
	}
	else
	{
		CLog::GetLog().AddLog(L"general error!");
	}
}

CString CQueryPage::from_station_telecodeGet( QueryInfo& input )
{
	return getTeleCodebyName(input.fromStation);
}

CString CQueryPage::to_station_telecodeGet( QueryInfo& input )
{
	return getTeleCodebyName(input.toStation);
}
