#pragma once
#include "TrainInfo.h"
#include "TicketInfo.h"
#include "HTTPContent.h"

class CBookPage
{
public:
	CBookPage(void);
	~CBookPage(void);
	void BuildRequest( TrainInfo& input );
	void GetPageData( CHTTPContent& content );
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
private:
	CString reqStr;
	CString reqData;
	CString respStr;
};
