#include "StdAfx.h"
#include "ConfirmPage.h"

CConfirmPage::CConfirmPage(void)
{
	isGet = FALSE;
}

CConfirmPage::~CConfirmPage(void)
{
}

void CConfirmPage::BuildRequest( TicketInfo& input )
{
	reqStr = L"/otsweb/order/confirmPassengerAction.do?method=confirmSingleForQueue";

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
		L"=%s"
		L"&randCode=%s"
		L"&orderRequest.reserve_flag=A"
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
		,passengerStrBuild(input.passengers)
		,input.randCode
		);

	refStr = L"/otsweb/order/confirmPassengerAction.do?method=init";
}

void CConfirmPage::ParseOutput( OrderInfo& output )
{
	if(status != ERROR_OK)
		return;
	// split results
	CRegex regex;
	CString pattern, restStr, matchStr;

	matchStr = respStr;

	pattern = L"\\{\\\"errMsg\\\":{\\q}\\}";
	regex.patternLoad(pattern);
	if(regex.contextMatch(matchStr, restStr) == TRUE)
	{
		regex.matchGet(0, output.errMsg);

		if(output.errMsg == L"\"Y\"")
		{
			CLog::GetLog().AddLog(L"confirm page success!");
			status = ERROR_OK;
		}
		else if(output.errMsg.Find(L"网络可能存在问题") != -1)
		{
			// logic error
			status = ERROR_SERVER;
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
			CLog::GetLog().AddLog(output.errMsg);
			status = ERROR_GENERAL;
		}
	}
	else
	{
		CLog::GetLog().AddLog(output.errMsg);
		status = ERROR_GENERAL;
	}
	matchStr = restStr;



}

CString CConfirmPage::start_timeGet( TicketInfo& input )
{
	CString str = input.start_time;
	str.Replace(L":", L"%3A");
	return str;
}

CString CConfirmPage::end_timeGet( TicketInfo& input )
{
	CString str = input.end_time;
	str.Replace(L":", L"%3A");
	return str;
}

CString CConfirmPage::from_station_nameGet( TicketInfo& input )
{
	return GetUTF8Str(input.from_station_name);
}

CString CConfirmPage::to_station_nameGet( TicketInfo& input )
{
	return GetUTF8Str(input.to_station_name);
}

CString CConfirmPage::passengerTicketsGet( PassengerInfo& passenger )
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

CString CConfirmPage::passengerStrBuild(CArray<PassengerInfo>& passengers)
{
	CString str;
	for(int i = 0; i < passengers.GetCount(); i++)
	{
		str.AppendFormat(L"&passengerTickets=%s", passengerTicketsGet(passengers[i]));
	}
	return str;
}