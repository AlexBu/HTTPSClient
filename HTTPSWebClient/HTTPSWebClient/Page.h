#pragma once
#include "Info.h"
#include "HTTPContent.h"

class CPage
{
public:
	CPage(void);
	~CPage(void);
	void GetPageData( CHTTPContent& content );
	int GetStatus();
	CString GetResponse();
protected:
	CString reqStr;
	CString reqData;
	CString respStr;
	int status;
	BOOL isGet;
};
