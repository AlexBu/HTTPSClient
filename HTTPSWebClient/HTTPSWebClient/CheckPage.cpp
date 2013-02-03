#include "StdAfx.h"
#include "CheckPage.h"

CCheckPage::CCheckPage(void)
{
	isGet = FALSE;
}

CCheckPage::~CCheckPage(void)
{
}

void CCheckPage::BuildRequest( TicketInfo& input )
{
	reqStr.Format(L"/otsweb/order/confirmPassengerAction.do?"
		L"method=checkOrderInfo&rand=%s"
		, input.randCode);

	reqData.Format(L"org.apache.struts.taglib.html.TOKEN=%s"
		L"&leftTicketStr=%s"
		L"&textfield="
		L"&orderRequest.train_date=%s"
		L"&orderRequest.train_no=%s"
		L"&orderRequest.station_train_code=%s"
		L"&orderRequest.from_station_telecode=%s"
		L"&orderRequest.to_station_telecode=%s"
		L"&orderRequest.seat_type_code="
		L"&orderRequest.ticket_type_order_num="
		L"&orderRequest.bed_level_order_num=%s"
		L"&orderRequest.start_time=%s"
		L"&orderRequest.end_time=%s"
		L"&orderRequest.from_station_name=%s"
		L"&orderRequest.to_station_name=%s"
		L"&orderRequest.cancel_flag=%s"
		L"&orderRequest.id_mode=%s"
		L"&passengerTickets=%s"
		L"&passengerTickets=%s"
		L"&randCode=%s"
		L"&orderRequest.reserve_flag=A"
		L"&tFlag=%s"
		,input.token
		,input.leftTicketStr
		,input.train_date
		,input.train_no
		,input.station_train_code
		,input.from_station_telecode
		,input.to_station_telecode
		,input.bed_level_order_num
		,start_timeGet(input)
		,end_timeGet(input)
		,from_station_nameGet(input)
		,to_station_nameGet(input)
		,input.cancel_flag
		,input.id_mode
		,passengerTicketsGet(input.passengers[0])
		,passengerTicketsGet(input.passengers[1])
		,input.randCode
		,input.tFlag
		);

		refStr = L"/otsweb/order/confirmPassengerAction.do?method=init";
}

void CCheckPage::ParseOutput( OrderInfo& output )
{
	if(status != ERROR_OK)
		return;
	// split results
	CRegex regex;
	CString pattern, restStr, matchStr;

	// get errMsg first
	matchStr = respStr;
	pattern = L"\\\"errMsg\\\":{\\q}";
	regex.patternLoad(pattern);
	if((regex.contextMatch(matchStr, restStr) == TRUE) && (regex.matchCount() == 1))
	{
		regex.matchGet(0, output.errMsg);
	}

	if(output.errMsg == L"\"Y\"")
	{
		// get rest fields
		matchStr = respStr;
		restStr.Empty();
		pattern = L"\\\"checkHuimd\\\":{\\q},\\\"check608\\\":{\\q},\\\"msg\\\":{\\q}";
		regex.patternLoad(pattern);
		if((regex.contextMatch(matchStr, restStr) == TRUE) && (regex.matchCount() == 3))
		{
			regex.matchGet(0, output.checkHuimd);
			regex.matchGet(1, output.check608);
			regex.matchGet(2, output.msg);
		}
		matchStr = restStr;
		restStr.Empty();

		if(output.checkHuimd == L"\"N\"")
		{
			CLog::GetLog().AddLog(L"canceled too many times!");
			status = ERROR_CANCEL_TOO_MANY;
		}
		else if(output.check608 == L"\"N\"")
		{
			CLog::GetLog().AddLog(L"real name applied on this train!");
			status = ERROR_REAL_NAME;
		}
		else
		{
			CLog::GetLog().AddLog(L"check page success!");
			status = ERROR_OK;
		}
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
		CLog::GetLog().AddLog(L"general error!");
		status = ERROR_GENERAL;
	}
}

CString CCheckPage::start_timeGet( TicketInfo& input )
{
	CString str = input.start_time;
	str.Replace(L":", L"%3A");
	return str;
}

CString CCheckPage::end_timeGet( TicketInfo& input )
{
	CString str = input.end_time;
	str.Replace(L":", L"%3A");
	return str;
}

CString CCheckPage::from_station_nameGet( TicketInfo& input )
{
	return GetUTF8Str(input.from_station_name);
}

CString CCheckPage::to_station_nameGet( TicketInfo& input )
{
	return GetUTF8Str(input.to_station_name);
}

CString CCheckPage::passengerTicketsGet( PassInfo& passenger )
{
	CString str(L"");
	str.Format(L"%s%%2C0%%2C1%%2C%s%%2C%s%%2C%s%%2C%s%%2CN",
		passenger.seatTyp,
		GetUTF8Str(passenger.name),
		passenger.passTyp,
		passenger.passNo,
		passenger.mobileNo
		);
	return str;
}
