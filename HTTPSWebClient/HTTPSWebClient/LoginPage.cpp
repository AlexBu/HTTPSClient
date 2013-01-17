#include "StdAfx.h"
#include "LoginPage.h"

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
	if(status == ERROR_HTTP)
		return;
	CRegex regex;
	CString patternTitle = L"<[tT][iI][tT][lL][eE]>{[^</>]+}</[tT][iI][tT][lL][eE]>";
	regex.patternLoad(patternTitle);
	CString restStr, titleStr;
	regex.contextMatch(respStr, restStr);
	regex.matchGet(0, titleStr);

	if(titleStr == L"ϵͳ��Ϣ")
	{
		CLog::GetLog().AddLog(L"login page success!");
		status = ERROR_OK;
	}
	else if(respStr.Find(L"��������ȷ����֤��") != -1)
	{
		CLog::GetLog().AddLog(L"wrong validate code!");
		status = ERROR_VALIDATE;
	}
	else if(respStr.Find(L"��¼��������") != -1)
	{
		CLog::GetLog().AddLog(L"username not exist!");
		status = ERROR_USERNAME;
	}
	else if(respStr.Find(L"�����������") != -1)
	{
		CLog::GetLog().AddLog(L"wrong password!");
		status = ERROR_PASSWORD;
	}
	else if(respStr.Find(L"��ǰ�����û�����") != -1)
	{
		CLog::GetLog().AddLog(L"too many people!");
		status = ERROR_OVERLOAD;
	}
	else
	{
		CLog::GetLog().AddLog(L"general error!");
		status = ERROR_GENERAL;
	}
}
