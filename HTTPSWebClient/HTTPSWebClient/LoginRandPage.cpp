#include "StdAfx.h"
#include "LoginRandPage.h"
#include "regex.h"

CLoginRandPage::CLoginRandPage(void)
:reqStr(L"")
,respStr(L"")
{
}

CLoginRandPage::~CLoginRandPage(void)
{
}

void CLoginRandPage::BuildRequestURL()
{
	reqStr = L"/otsweb/loginAction.do?method=loginAysnSuggest";
}

void CLoginRandPage::GetPageData(CHTTPContent& httpContent)
{
	BuildRequestURL();

	httpContent.SendDatabyGet(reqStr);
	httpContent.GetResponseStr(respStr);
}

void CLoginRandPage::ParseOutput( CLoginRandPageOut& output )
{
	// get rand number from response
	CRegex regex;
	CString patternRand = L"{\\d+}";
	regex.patternLoad(patternRand);
	CString restStr;
	regex.contextMatch(respStr, restStr);
	CString rand;
	regex.matchGet(0, rand);

	output.randSet(rand);
}

void CLoginRandPage::CollectInput( CLoginRandPageIn& input )
{
	// do nothing for this page
}