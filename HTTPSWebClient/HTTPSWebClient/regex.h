#pragma once

#include "stdafx.h"
#include "atlrx.h"

// regex interfaces

class CRegex
{
public:
	// wrap regex of atl
	BOOL patternLoad(CString& pattern);
	BOOL contextMatch(CString& context, CString& rest);
	BOOL matchGet(DWORD matchIndex, CString& result);
	BOOL GetStringByPattern(CStringArray& strList, CString& content, CString& pattern);
private:
	CAtlRegExp<> regularExpression;
	CAtlREMatchContext<> matchContext;
	CString patternString;
};