#pragma once
#include "OrderInfo.h"
#include "HTTPContent.h"

class CWaitPage
{
public:
	CWaitPage(void);
	~CWaitPage(void);
	void BuildRequest( OrderInfo& input );
	void GetPageData( CHTTPContent& content );
	void ParseOutput( OrderInfo& output );
private:
	CString reqStr;
	CString respStr;
};
