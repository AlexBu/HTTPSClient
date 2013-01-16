#pragma once
#include "Page.h"

class CQueuePage : public CPage
{
public:
	CQueuePage(void);
	~CQueuePage(void);
	void BuildRequest( TicketInfo& input );
	void ParseOutput( OrderInfo& output );
};
