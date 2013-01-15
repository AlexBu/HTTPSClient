#pragma once
#include "OrderInfo.h"
#include "HTTPContent.h"

class CConfirmPage
{
public:
	CConfirmPage(void);
	~CConfirmPage(void);
	void BuildRequest( OrderInfo& input );
	void GetPageData( CHTTPContent& content );
	void ParseOutput( OrderInfo& output );
private:
	CString reqStr;
	CString reqData;
	CString respStr;
};
