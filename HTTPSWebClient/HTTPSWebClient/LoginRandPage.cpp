#include "StdAfx.h"
#include "LoginRandPage.h"

CLoginRandPage::CLoginRandPage(void)
{
}

CLoginRandPage::~CLoginRandPage(void)
{
}

void CLoginRandPage::BuildRequest()
{
	reqStr = L"/otsweb/loginAction.do?method=loginAysnSuggest";
}

void CLoginRandPage::ParseOutput( LoginInfo& output )
{
	if(status == ERROR_HTTP)
		return;
	// get rand number from response
	CRegex regex;
	CString patternRand = L"{\\d+}";
	regex.patternLoad(patternRand);
	CString restStr;
	regex.contextMatch(respStr, restStr);
	if(regex.matchCount() == 1)
	{
		regex.matchGet(0, output.rand);
		CLog::GetLog().AddLog(L"login rand page success!");
		status = 0;
	}
	else
	{
		CLog::GetLog().AddLog(L"general error!");
		status = -1;
	}	
}