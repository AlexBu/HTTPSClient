#include "StdAfx.h"
#include "LoginPage.h"
#include "regex.h"

CLoginPage::CLoginPage(void)
:rand(L"")
{
}

CLoginPage::~CLoginPage(void)
{
}

void CLoginPage::CollectInfoFromResp( CString& loginPageResStr )
{
	// get rand number from response
	CRegex	regex;
	CString patternRand = L"{\\d+}";
	regex.patternLoad(patternRand);
	CString restStr;
	regex.contextMatch(loginPageResStr, restStr);
	CString rand;
	regex.matchGet(0, rand);
}

void CLoginPage::BuildRequestString( const CString& usernameStr, const CString& passwordStr, const CString& validateStr )
{
	/*throw std::exception("The method or operation is not implemented.");*/
}
