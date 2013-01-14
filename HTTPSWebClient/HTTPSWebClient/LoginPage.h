#pragma once
#include "LoginPageIn.h"
#include "LoginPageOut.h"
#include "HTTPContent.h"

class CLoginPage
{
public:
	CLoginPage(void);
	~CLoginPage(void);
	void BuildRequestURL();
	void CollectInput( CLoginPageIn& loginPageIn );
	void GetPageData( CHTTPContent& httpContent );
	void ParseOutput( CLoginPageOut& loginPageOut );
private:
	CString reqStr;
	CString respStr;
	CString reqData;

	CString username;
	CString password;
	CString rand;
	CString validate;
};
