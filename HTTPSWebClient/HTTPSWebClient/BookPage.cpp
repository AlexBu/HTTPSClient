#include "StdAfx.h"
#include "BookPage.h"
#include "regex.h"
#include "Utility.h"

CBookPage::CBookPage(void)
{
	isGet = FALSE;
}

CBookPage::~CBookPage(void)
{
}

void CBookPage::BuildRequest( TrainInfo& input )
{
	reqStr = L"/otsweb/order/querySingleAction.do?method=submutOrderRequest";
 	reqData.Format(L"station_train_code=%s"
 		L"&train_date=%s"
 		L"&seattype_num="
 		L"&from_station_telecode=%s"
 		L"&to_station_telecode=%s"
 		L"&include_student=%s"
 		L"&from_station_telecode_name=%s"
 		L"&to_station_telecode_name=%s"
 		L"&round_train_date=%s"
 		L"&round_start_time_str=%s"
 		L"&single_round_type=%s"
 		L"&train_pass_type=%s"
 		L"&train_class_arr=%s"
 		L"&start_time_str=%s"
 		L"&lishi=%s"
 		L"&train_start_time=%s"
 		L"&trainno4=%s"
 		L"&arrive_time=%s"
 		L"&from_station_name=%s"
 		L"&to_station_name=%s"
 		L"&from_station_no=%s"
 		L"&to_station_no=%s"
 		L"&ypInfoDetail=%s"
 		L"&mmStr=%s"
 		L"&locationCode=%s",
 		input.trainCode,
 		input.trainDate,
 		input.stationFromCode,
 		input.stationToCode,
 		studentGet(input),
 		stationFromTeNameGet(input),
 		stationToTeNameGet(input),
 		input.trainRoundDate,
 		trainRoundTimeStrGet(input),
 		roundTypeGet(input),
 		passTypeGet(input),
 		trainClassGet(input),
 		timeStartStrGet(input),
 		durationGet(input),
 		trainStartTimeGet(input),
 		input.trainNo,
 		timeArriveGet(input),
 		stationFromNameGet(input),
 		stationToNameGet(input),
 		input.stationFromNo,
 		input.stationToNo,
 		input.infoDetail,
 		input.mmStr,
 		input.locationCode
 		);
}

void CBookPage::ParseOutput( TicketInfo& output )
{
	// split results
	CRegex regex;
	CString pattern, restStr, matchStr;

	matchStr = respStr;

	pattern = L"org\\.apache\\.struts\\.taglib\\.html\\.TOKEN\\\" value=\\\"{[^\\\"]+}\\\"";
	regex.patternLoad(pattern);
	regex.contextMatch(matchStr, restStr);
	if(regex.matchCount() == 0)
	{
		status = -1;
		return;
	}
	regex.matchGet(0, output.token);
	matchStr = restStr;
	restStr.Empty();

	pattern = L"left_ticket\\\"\\b*\\n\\b*value=\\\"{[^\\\"]+}\\\"";
	regex.patternLoad(pattern);
	regex.contextMatch(matchStr, restStr);
	if(regex.matchCount() == 0)
	{
		status = -1;
		return;
	}
	regex.matchGet(0, output.leftTicketStr);
	matchStr = restStr;
	restStr.Empty();

	pattern = L"orderRequest\\.train_date\\\" value=\\\"{[^\\\"]+}\\\"";
	regex.patternLoad(pattern);
	regex.contextMatch(matchStr, restStr);
	if(regex.matchCount() == 0)
	{
		status = -1;
		return;
	}
	regex.matchGet(0, output.train_date);
	matchStr = restStr;
	restStr.Empty();

	pattern = L"orderRequest\\.train_no\\\" value=\\\"{[^\\\"]+}\\\"";
	regex.patternLoad(pattern);
	regex.contextMatch(matchStr, restStr);
	if(regex.matchCount() == 0)
	{
		status = -1;
		return;
	}
	regex.matchGet(0, output.train_no);
	matchStr = restStr;
	restStr.Empty();

	pattern = L"orderRequest\\.station_train_code\\\" value=\\\"{[^\\\"]+}\\\"";
	regex.patternLoad(pattern);
	regex.contextMatch(matchStr, restStr);
	if(regex.matchCount() == 0)
	{
		status = -1;
		return;
	}
	regex.matchGet(0, output.station_train_code);
	matchStr = restStr;
	restStr.Empty();

	pattern = L"orderRequest\\.from_station_telecode\\\" value=\\\"{[^\\\"]+}\\\"";
	regex.patternLoad(pattern);
	regex.contextMatch(matchStr, restStr);
	if(regex.matchCount() == 0)
	{
		status = -1;
		return;
	}
	regex.matchGet(0, output.from_station_telecode);
	matchStr = restStr;
	restStr.Empty();

	pattern = L"orderRequest\\.to_station_telecode\\\" value=\\\"{[^\\\"]+}\\\"";
	regex.patternLoad(pattern);
	regex.contextMatch(matchStr, restStr);
	if(regex.matchCount() == 0)
	{
		status = -1;
		return;
	}
	regex.matchGet(0, output.to_station_telecode);
	matchStr = restStr;
	restStr.Empty();

	pattern = L"orderRequest\\.bed_level_order_num\\\" value=\\\"{[^\\\"]+}\\\"";
	regex.patternLoad(pattern);
	regex.contextMatch(matchStr, restStr);
	if(regex.matchCount() == 0)
	{
		status = -1;
		return;
	}
	regex.matchGet(0, output.bed_level_order_num);
	matchStr = restStr;
	restStr.Empty();

	pattern = L"orderRequest\\.start_time\\\" value=\\\"{[^\\\"]+}\\\"";
	regex.patternLoad(pattern);
	regex.contextMatch(matchStr, restStr);
	if(regex.matchCount() == 0)
	{
		status = -1;
		return;
	}
	regex.matchGet(0, output.start_time);
	matchStr = restStr;
	restStr.Empty();

	pattern = L"orderRequest\\.end_time\\\" value=\\\"{[^\\\"]+}\\\"";
	regex.patternLoad(pattern);
	regex.contextMatch(matchStr, restStr);
	if(regex.matchCount() == 0)
	{
		status = -1;
		return;
	}
	regex.matchGet(0, output.end_time);
	matchStr = restStr;
	restStr.Empty();

	pattern = L"orderRequest\\.from_station_name\\\" value=\\\"{[^\\\"]+}\\\"";
	regex.patternLoad(pattern);
	regex.contextMatch(matchStr, restStr);
	if(regex.matchCount() == 0)
	{
		status = -1;
		return;
	}
	regex.matchGet(0, output.from_station_name);
	matchStr = restStr;
	restStr.Empty();

	pattern = L"orderRequest\\.to_station_name\\\" value=\\\"{[^\\\"]+}\\\"";
	regex.patternLoad(pattern);
	regex.contextMatch(matchStr, restStr);
	if(regex.matchCount() == 0)
	{
		status = -1;
		return;
	}
	regex.matchGet(0, output.to_station_name);
	matchStr = restStr;
	restStr.Empty();

	pattern = L"orderRequest\\.cancel_flag\\\" value=\\\"{[^\\\"]+}\\\"";
	regex.patternLoad(pattern);
	regex.contextMatch(matchStr, restStr);
	if(regex.matchCount() == 0)
	{
		status = -1;
		return;
	}
	regex.matchGet(0, output.cancel_flag);
	matchStr = restStr;
	restStr.Empty();

	pattern = L"orderRequest\\.id_mode\\\" value=\\\"{[^\\\"]+}\\\"";
	regex.patternLoad(pattern);
	regex.contextMatch(matchStr, restStr);
	if(regex.matchCount() == 0)
	{
		status = -1;
		return;
	}
	regex.matchGet(0, output.id_mode);
	matchStr = restStr;
	restStr.Empty();

	pattern = L"submit_form_confirm\\(\\\'confirmPassenger\\\',\\\'{[^\\\']+}\\\'";
	regex.patternLoad(pattern);
	regex.contextMatch(matchStr, restStr);
	if(regex.matchCount() == 0)
	{
		status = -1;
		return;
	}
	regex.matchGet(0, output.tFlag);
	matchStr = restStr;
	restStr.Empty();

	status = 0;
}

CString CBookPage::studentGet( TrainInfo& input )
{
	return L"00";
}

CString CBookPage::stationFromTeNameGet( TrainInfo& input )
{
	return GetUTF8Str(input.stationFromName);
}

CString CBookPage::stationToTeNameGet( TrainInfo& input )
{
	return GetUTF8Str(input.stationToTeName);
}

CString CBookPage::trainRoundTimeStrGet( TrainInfo& input )
{
	return L"00%3A00--24%3A00";
}

CString CBookPage::roundTypeGet( TrainInfo& input )
{
	return L"1";
}

CString CBookPage::passTypeGet( TrainInfo& input )
{
	return L"QB";
}

CString CBookPage::trainClassGet( TrainInfo& input )
{
	return L"QB%23D%23Z%23T%23K%23QT%23";
}

CString CBookPage::timeStartStrGet( TrainInfo& input )
{
	return L"00%3A00--24%3A00";
}

CString CBookPage::durationGet( TrainInfo& input )
{
	CString str = input.duration;
	str.Replace(L":", L"%3A");
	return str;
}

CString CBookPage::trainStartTimeGet( TrainInfo& input )
{
	CString str = input.trainStartTime;
	str.Replace(L":", L"%3A");
	return str;
}

CString CBookPage::timeArriveGet( TrainInfo& input )
{
	CString str = input.timeArrive;
	str.Replace(L":", L"%3A");
	return str;
}

CString CBookPage::stationFromNameGet( TrainInfo& input )
{
	return GetUTF8Str(input.stationFromTeName);
}

CString CBookPage::stationToNameGet( TrainInfo& input )
{
	return GetUTF8Str(input.stationToTeName);
}
