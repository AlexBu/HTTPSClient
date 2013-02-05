#pragma once
#include "defines.h"
#include "Info.h"
#include "HTTPConnection.h"
#include "regex.h"
#include "Log.h"
#include "Utility.h"

#define MAX_DATA_SIZE (1*1024*1024L)

class CPage
{
public:
	CPage(void);
	~CPage(void);
	void GetPageStr( CHTTPConnection& content );
	void GetPageData( CHTTPConnection& content );
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
