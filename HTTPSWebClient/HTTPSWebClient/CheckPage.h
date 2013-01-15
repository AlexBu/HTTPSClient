#pragma once
#include "TicketInfo.h"
#include "OrderInfo.h"
#include "HTTPContent.h"

class CCheckPage
{
public:
	CCheckPage(void);
	~CCheckPage(void);
	void BuildRequest( TicketInfo& input );
	void GetPageData( CHTTPContent& content );
	void ParseOutput( OrderInfo& output );
	CString textfieldGet( TicketInfo& input );
	CString start_timeGet( TicketInfo& input );
	CString end_timeGet( TicketInfo& input );
	CString from_station_nameGet( TicketInfo& input );
	CString to_station_nameGet( TicketInfo& input );
	CString reserve_flagGet( TicketInfo& input );
	CString passengerTicketsGet( PassengerInfo& passenger );
	CString oldPassengersGet( PassengerInfo& passenger );
	CString seat_detailGet( PassengerInfo& passenger );
	CString ticketGet( PassengerInfo& passenger );
	CString nameGet( PassengerInfo& passenger );

private:
	CString reqStr;
	CString reqData;
	CString respStr;
};
