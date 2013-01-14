#pragma once

#include "LoginRandPageIn.h"
#include "LoginRandPageOut.h"
#include "HTTPContent.h"

class CLoginRandPage
{
public:
	CLoginRandPage(void);
	virtual ~CLoginRandPage(void);
	void CollectInput( CLoginRandPageIn& input );
	void GetPageData(CHTTPContent &content);
	void ParseOutput( CLoginRandPageOut& output );
private: 
	void BuildRequestURL();
private:
	CString reqStr;
	CString respStr;
};
