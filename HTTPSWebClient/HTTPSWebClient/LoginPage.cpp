#include "StdAfx.h"
#include "LoginPage.h"
#include "regex.h"

CLoginPage::CLoginPage(void)
:reqStr(L"")
,respStr(L"")
,reqData(L"")
,username(L"")
,password(L"")
,rand(L"")
,validate(L"")
{
}

CLoginPage::~CLoginPage(void)
{
}

void CLoginPage::BuildRequestURL()
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
		rand, username, password, validate);
}

void CLoginPage::CollectInput( CLoginPageIn& loginPageIn )
{
	username = loginPageIn.usernameGet();
	password = loginPageIn.passwordGet();
	rand = loginPageIn.randGet();
	validate = loginPageIn.validateGet();
}

void CLoginPage::GetPageData( CHTTPContent& httpContent )
{
	BuildRequestURL();

	httpContent.SendDatabyPost(reqStr, reqData);
	httpContent.GetResponseStr(respStr);
}

void CLoginPage::ParseOutput( CLoginPageOut& loginPageOut )
{
	CRegex regex;
	CString patternTitle = L"<[tT][iI][tT][lL][eE]>{[^</>]+}</[tT][iI][tT][lL][eE]>";
	regex.patternLoad(patternTitle);
	CString restStr;
	regex.contextMatch(respStr, restStr);
	CString titleStr;
	regex.matchGet(0, titleStr);

	if(titleStr == L"系统消息")
	{
		loginPageOut.loginSet(TRUE);
	}
}
