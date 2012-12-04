#pragma once

#include "stdafx.h"
#include "atlrx.h"

// regex interfaces

class CRegex
{
public:
	// wrap regex of atl
	CRegex();
	virtual ~CRegex();

	BOOL patternLoad(CString pattern);
	CString patternGet();	// maybe no use
	BOOL contextMatch(CString context);
	DWORD matchCount();
	BOOL matchGet(DWORD matchIndex, CString& result);

private:
	CAtlRegExp<> regularExpression;
	CAtlREMatchContext<> matchContext;
	CString patternString;
};