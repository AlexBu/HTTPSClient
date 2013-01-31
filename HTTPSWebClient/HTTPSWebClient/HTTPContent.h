#pragma once

class CHTTPContent
{
public:
	CHTTPContent();
	virtual ~CHTTPContent(void);
	void ConnectInit(const CString& site);
	void ConnectSite(const CString& site);
	void SwitchSite();
	BOOL SendDatabyGet(const CString& URL);

	void GetCookie( HINTERNET hRequest );

	BOOL SendDatabyPost(const CString& URL, const CString& additionalData);
	void GetResponseStr(CString& result);
	void GetResponseRaw(BYTE* result, DWORD& size);
	void SetRefStr(CString& str);
	void CleanCookie();
private:
	BYTE* buff;
	CString siteAdr;
	int status;
	HINTERNET	hConnect;
	HINTERNET	hSession;
	DWORD buffUsed;
	CString refStr;
	CString cookieStr;
	CStringArray cdnlist;
};
