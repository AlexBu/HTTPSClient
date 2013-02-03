#pragma once
#include "Page.h"

class CBookPage: public CPage
{
public:
	CBookPage(void);
	~CBookPage(void);
	void BuildRequest( TrainInfo& input );
	void ParseOutput( TicketInfo& output );
	CString stationFromTeNameGet( TrainInfo& input );
	CString stationToTeNameGet( TrainInfo& input );
	CString durationGet( TrainInfo& input );
	CString trainStartTimeGet( TrainInfo& input );
	CString timeArriveGet( TrainInfo& input );
	CString stationFromNameGet( TrainInfo& input );
	CString stationToNameGet( TrainInfo& input );
};
