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
		L"&textfield=%s"
		//L"&checkbox0=0"
		//L"&checkbox2=2"
		//L"&checkbox4=4"
		//L"&checkbox5=5"
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
		//L"&oldPassengers=%s"
		//L"&passenger_1_seat=%s"
		//L"&passenger_1_ticket=%s"
		//L"&passenger_1_name=%s"
		//L"&passenger_1_cardtype=%s"
		//L"&passenger_1_cardno=%s"
		//L"&passenger_1_mobileno=%s"

		L"&passengerTickets=%s"
		//L"&oldPassengers=%s"
		//L"&passenger_2_seat=%s"
		//L"&passenger_2_ticket=%s"
		//L"&passenger_2_name=%s"
		//L"&passenger_2_cardtype=%s"
		//L"&passenger_2_cardno=%s"
		//L"&passenger_2_mobileno=%s"

		//L"&passengerTickets=%s"
		//L"&oldPassengers=%s"
		//L"&passenger_3_seat=%s"
		//L"&passenger_3_ticket=%s"
		//L"&passenger_3_name=%s"
		//L"&passenger_3_cardtype=%s"
		//L"&passenger_3_cardno=%s"
		//L"&passenger_3_mobileno=%s"
		
		//L"&passengerTickets=%s"
		//L"&oldPassengers=%s"
		//L"&passenger_4_seat=%s"
		//L"&passenger_4_ticket=%s"
		//L"&passenger_4_name=%s"
		//L"&passenger_4_cardtype=%s"
		//L"&passenger_4_cardno=%s"
		//L"&passenger_4_mobileno=%s"
		//L"&oldPassengers="
		//L"&checkbox9=Y"

		L"&randCode=%s"
		L"&orderRequest.reserve_flag=%s"
		L"&tFlag=%s"
		,input.token
		,input.leftTicketStr
		,textfieldGet(input)
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
		//,oldPassengersGet(input.passengers[0])
		//,input.passengers[0].seat
		//,ticketGet(input.passengers[0])
		//,nameGet(input.passengers[0])
		//,input.passengers[0].cardtype
		//,input.passengers[0].cardno
		//,input.passengers[0].mobileno
		,passengerTicketsGet(input.passengers[1])
		//,oldPassengersGet(input.passengers[1])
		//,input.passengers[1].seat
		//,ticketGet(input.passengers[1])
		//,nameGet(input.passengers[1])
		//,input.passengers[1].cardtype
		//,input.passengers[1].cardno
		//,input.passengers[1].mobileno
		//,passengerTicketsGet(input.passengers[2])
		//,oldPassengersGet(input.passengers[2])
		//,input.passengers[2].seat
		//,ticketGet(input.passengers[2])
		//,nameGet(input.passengers[2])
		//,input.passengers[2].cardtype
		//,input.passengers[2].cardno
		//,input.passengers[2].mobileno
		//,passengerTicketsGet(input.passengers[3])
		//,oldPassengersGet(input.passengers[3])
		//,input.passengers[3].seat
		//,ticketGet(input.passengers[3])
		//,nameGet(input.passengers[3])
		//,input.passengers[3].cardtype
		//,input.passengers[3].cardno
		//,input.passengers[3].mobileno
		,input.randCode
		,reserve_flagGet(input)
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
			//// remove last part of str
			//pattern = L"{.*}&tFlag=.*$";
			//matchStr = reqData;
			//regex.patternLoad(pattern);
			//if(regex.contextMatch(matchStr, restStr) == TRUE)
			//{
			//	regex.matchGet(0, output.orderInfo);
			//	CLog::GetLog().AddLog(L"check page success!");
			//	status = ERROR_OK;
			//}
			//else
			//{
			//	CLog::GetLog().AddLog(L"parse confirm string failed!");
			//	status = ERROR_GENERAL;
			//}

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

CString CCheckPage::textfieldGet( TicketInfo& input )
{
	return L"%E4%B8%AD%E6%96%87%E6%88%96%E6%8B%BC%E9%9F%B3%E9%A6%96%E5%AD%97%E6%AF%8D";
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

CString CCheckPage::reserve_flagGet( TicketInfo& input )
{
	return L"A";
}

CString CCheckPage::passengerTicketsGet( PassengerInfo& passenger )
{
	CString str(L"");
	str.Format(L"%s%%2C0%%2C%s%%2C%s%%2C%s%%2C%s%%2C%s%%2CN",
		passenger.seat,
		ticketGet(passenger),
		GetUTF8Str(passenger.name),
		passenger.cardtype,
		passenger.cardno,
		passenger.mobileno
		);
	return str;
}

CString CCheckPage::oldPassengersGet( PassengerInfo& passenger )
{
	CString str(L"");
	str.Format(L"%s%%2C%s%%2C%s",
		GetUTF8Str(passenger.name),
		passenger.cardtype,
		passenger.cardno
		);
	return str;
}

CString CCheckPage::ticketGet( PassengerInfo& passenger )
{
	return L"1";
}

CString CCheckPage::nameGet( PassengerInfo& passenger )
{
	return GetUTF8Str(passenger.name);
}

