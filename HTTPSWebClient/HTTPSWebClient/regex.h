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

private:
	CAtlRegExp<> regularExpression;
	CAtlREMatchContext<> matchContext;
	CString patternString;
};