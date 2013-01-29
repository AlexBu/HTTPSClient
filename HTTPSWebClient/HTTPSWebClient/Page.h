#pragma once
#include "defines.h"
#include "Info.h"
#include "HTTPContent.h"
#include "regex.h"
#include "Log.h"
#include "Utility.h"

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
