#include "StdAfx.h"
#include "RandPage.h"

CRandPage::CRandPage(void)
{
}

CRandPage::~CRandPage(void)
{
}

void CRandPage::BuildRequest()
{
	reqStr = L"/otsweb/loginAction.do?method=loginAysnSuggest";
	refStr = L"/otsweb/loginAction.do?method=init";
}

void CRandPage::ParseOutput( LoginInfo& output )
{
	if(status != ERROR_OK)
		return;
	// get rand number from response
	CRegex regex;
	CString patternRand = L"{\\d+}";
	regex.patternLoad(patternRand);
	CString restStr;

	if(regex.contextMatch(respStr, restStr) == TRUE)
	{
		regex.matchGet(0, output.rand);
		CLog::GetLog().AddLog(L"login rand page success!");
		status = ERROR_OK;
	}
	else
	{
		CLog::GetLog().AddLog(L"general error!");
		status = ERROR_GENERAL;
	}	
}