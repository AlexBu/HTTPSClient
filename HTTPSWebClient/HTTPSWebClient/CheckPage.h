#pragma once
#include "Page.h"

class CCheckPage: public CPage
{
public:
	CCheckPage(void);
	~CCheckPage(void);
	void BuildRequest( TicketInfo& input );
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
};
