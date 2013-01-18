#pragma once
#include "Page.h"

#define QUERY_DELAY		(5)
class CQueuePage : public CPage
{
public:
	CQueuePage(void);
	~CQueuePage(void);
	void BuildRequest( TicketInfo& input );
	void ParseOutput( OrderInfo& output );
};
