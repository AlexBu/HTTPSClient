#pragma once

class CHTTPContent
{
public:
	CHTTPContent();
	virtual ~CHTTPContent(void);
	void ConnectSite(const CString& site);
	BOOL SendDatabyGet(const CString& URL);
	BOOL SendDatabyPost(const CString& URL, const CString& additionalData);
	void GetResponseStr(CString& result);
	void GetResponseRaw(BYTE* result, DWORD& size);
	void SetRefStr(CString& str);
private:
	BYTE* buff;
	CString siteAdr;
	int status;
	HINTERNET	hConnect;
	HINTERNET	hSession;
	DWORD buffUsed;
	CString refStr;
};
