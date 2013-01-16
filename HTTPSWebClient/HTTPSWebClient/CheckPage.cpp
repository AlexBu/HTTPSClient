#include "StdAfx.h"
#include "CheckPage.h"
#include "regex.h"
#include "Utility.h"

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
		L"&checkbox0=0"
		L"&checkbox2=2"
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
		L"&oldPassengers=%s"
		L"&passenger_1_seat=%s"
		L"&passenger_1_ticket=%s"
		L"&passenger_1_name=%s"
		L"&passenger_1_cardtype=%s"
		L"&passenger_1_cardno=%s"
		L"&passenger_1_mobileno=%s"
		L"&passengerTickets=%s"
		L"&oldPassengers=%s"
		L"&passenger_2_seat=%s"
		L"&passenger_2_ticket=%s"
		L"&passenger_2_name=%s"
		L"&passenger_2_cardtype=%s"
		L"&passenger_2_cardno=%s"
		L"&passenger_2_mobileno=%s"
		L"&oldPassengers="
		L"&checkbox9=Y"
		L"&oldPassengers="
		L"&checkbox9=Y"
		L"&oldPassengers="
		L"&checkbox9=Y"
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
		,oldPassengersGet(input.passengers[0])
		,input.passengers[0].seat
		,ticketGet(input.passengers[0])
		,nameGet(input.passengers[0])
		,input.passengers[0].cardtype
		,input.passengers[0].cardno
		,input.passengers[0].mobileno
		,passengerTicketsGet(input.passengers[1])
		,oldPassengersGet(input.passengers[1])
		,input.passengers[1].seat
		,ticketGet(input.passengers[1])
		,nameGet(input.passengers[1])
		,input.passengers[1].cardtype
		,input.passengers[1].cardno
		,input.passengers[1].mobileno
		,input.randCode
		,reserve_flagGet(input)
		,input.tFlag
		);
}

void CCheckPage::ParseOutput( OrderInfo& output )
{
	// split results
	CRegex regex;
	CString pattern, restStr, matchStr;

	matchStr = respStr;

	pattern = L"\\{\\\"checkHuimd\\\":{\\q},\\\"check608\\\":{\\q},\\\"msg\\\":{\\q},\\\"errMsg\\\":{\\q}\\}";
	regex.patternLoad(pattern);
	regex.contextMatch(matchStr, restStr);
	if(regex.matchCount() == 4)
	{
		regex.matchGet(3, output.errMsg);
	}
	matchStr = restStr;
	restStr.Empty();

	if(output.errMsg == L"\"Y\"")
	{
		// remove last part of str
		pattern = L"{.*}&tFlag=.*$";
		matchStr = reqData;
		regex.patternLoad(pattern);
		regex.contextMatch(matchStr, restStr);
		regex.matchGet(0, output.orderInfo);
		status = 0;
	}
	else
	{
		status = -1;
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
	str.Format(L"%s%%2Cundefined%%2C%s%%2C%s%%2C%s%%2C%s%%2C%s%%2CN",
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

