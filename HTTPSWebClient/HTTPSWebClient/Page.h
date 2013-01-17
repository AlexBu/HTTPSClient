#pragma once
#include "Info.h"
#include "HTTPContent.h"
#include "regex.h"
#include "Log.h"
#include "Utility.h"

#define ERROR_OK		(0)
#define ERROR_GENERAL	(-1)
#define ERROR_HTTP		(-2)
#define ERROR_LOGIC		(-3)
#define ERROR_VALIDATE	(-4)
#define ERROR_USERNAME	(-5)
#define ERROR_PASSWORD	(-6)
#define ERROR_OVERLOAD	(-7)
#define ERROR_NOSEAT	(-8)

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
