#pragma once
#include "Page.h"

class CBookPage: public CPage
{
public:
	CBookPage(void);
	~CBookPage(void);
	void BuildRequest( TrainInfo& input );
	void ParseOutput( TicketInfo& output );
	CString studentGet( TrainInfo& input );
	CString stationFromTeNameGet( TrainInfo& input );
	CString stationToTeNameGet( TrainInfo& input );
	CString trainRoundTimeStrGet( TrainInfo& input );
	CString roundTypeGet( TrainInfo& input );
	CString passTypeGet( TrainInfo& input );
	CString trainClassGet( TrainInfo& input );
	CString timeStartStrGet( TrainInfo& input );
	CString durationGet( TrainInfo& input );
	CString trainStartTimeGet( TrainInfo& input );
	CString timeArriveGet( TrainInfo& input );
	CString stationFromNameGet( TrainInfo& input );
	CString stationToNameGet( TrainInfo& input );
};
