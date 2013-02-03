#pragma once
#include "Page.h"

class CConfirmPage : public CPage
{
public:
	CConfirmPage(void);
	~CConfirmPage(void);
	void BuildRequest( TicketInfo& input );
	void ParseOutput( OrderInfo& output );
	CString start_timeGet( TicketInfo& input );
	CString end_timeGet( TicketInfo& input );
	CString from_station_nameGet( TicketInfo& input );
	CString to_station_nameGet( TicketInfo& input );
	CString passengerTicketsGet( PassInfo& passenger );
};
