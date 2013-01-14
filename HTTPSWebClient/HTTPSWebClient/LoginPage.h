#pragma once

class CLoginPage
{
public:
	CLoginPage(void);
	~CLoginPage(void);
	void CollectInfoFromResp( CString& loginPageResStr );
	void BuildRequestString( const CString& usernameStr, const CString& passwordStr, const CString& validateStr );
private:
	CString rand;
};
