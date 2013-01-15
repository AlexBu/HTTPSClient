#include "StdAfx.h"
#include "LoginPage.h"
#include "regex.h"

CLoginPage::CLoginPage(void)
{
	isGet = FALSE;
}

CLoginPage::~CLoginPage(void)
{
}

void CLoginPage::BuildRequest( LoginInfo& input )
{
	reqStr = _T("/otsweb/loginAction.do?method=login");
	reqData.Format(L"loginRand=%s"
		L"&refundLogin=N"
		L"&refundFlag=Y"
		L"&loginUser.user_name=%s"
		L"&nameErrorFocus="
		L"&user.password=%s"
		L"&passwordErrorFocus="
		L"&randCode=%s"
		L"&randErrorFocus=", 
		input.rand, 
		input.username, 
		input.password, 
		input.validate);
}

void CLoginPage::ParseOutput( )
{
	CRegex regex;
	CString patternTitle = L"<[tT][iI][tT][lL][eE]>{[^</>]+}</[tT][iI][tT][lL][eE]>";
	regex.patternLoad(patternTitle);
	CString restStr, titleStr;
	regex.contextMatch(respStr, restStr);
	regex.matchGet(0, titleStr);

	if(titleStr == L"系统消息")
	{
		status = 0;
	}
	else
	{
		status = -1;
	}
}
