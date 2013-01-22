#pragma once
#include "Info.h"
#include "HTTPContent.h"
#include "regex.h"
#include "Log.h"
#include "Utility.h"

#define ERROR_OK				(0)
#define ERROR_GENERAL			(-1)
#define ERROR_HTTP				(-2)
#define ERROR_LOGIC				(-3)
#define ERROR_VALIDATE			(-4)
#define ERROR_USERNAME			(-5)
#define ERROR_PASSWORD			(-6)
#define ERROR_OVERLOAD			(-7)
#define ERROR_NOSEAT			(-8)
#define ERROR_ORDER				(-9)
#define ERROR_CANCEL_TOO_MANY	(-10)
#define ERROR_REAL_NAME			(-11)

#define MAX_DATA_SIZE (1*1024*1024L)

class CPage
{
public:
	CPage(void);
	~CPage(void);
	void GetPageStr( CHTTPContent& content );
	void GetPageData( CHTTPContent& content );
	int GetStatus();
	CString GetResponse();
protected:
	CString reqStr;
	CString reqData;
	CString respStr;
	CString refStr;
	int status;
	BOOL isGet;
	BYTE* buff;
	DWORD size;
};
