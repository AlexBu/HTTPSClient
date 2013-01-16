#include "StdAfx.h"
#include "LoginRandPage.h"
#include "regex.h"

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
		status = 0;
	}
	else
	{
		status = -1;
	}	
}