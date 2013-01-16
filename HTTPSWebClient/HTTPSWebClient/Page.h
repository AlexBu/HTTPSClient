#pragma once
#include "Info.h"
#include "HTTPContent.h"

#define ERROR_OK		(0)
#define ERROR_GENERAL	(-1)
#define ERROR_HTTP		(-2)
#define ERROR_LOGIC		(-3)

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
