#pragma once
#include "Page.h"

class CQueryPage : public CPage
{
public:
	CQueryPage(void);
	~CQueryPage(void);
	void BuildRequest( QueryInfo& input );
	void ParseOutput( TrainInfo& output );
	CString from_station_telecodeGet( QueryInfo& input );
	CString to_station_telecodeGet( QueryInfo& input );
};
