#pragma once
#include "QueryInfo.h"
#include "TrainInfo.h"
#include "HTTPContent.h"

class CQueryPage
{
public:
	CQueryPage(void);
	~CQueryPage(void);
	void BuildRequest( QueryInfo& input );
	void GetPageData( CHTTPContent& content );
	void ParseOutput( TrainInfo& output );
private:
	CString reqStr;
	CString respStr;
};
