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

	BOOL patternLoad();
	CString patternGet();	// maybe no use
	void contextMatch();
	DWORD matchCount();
	CString matchGet(DWORD matchIndex);

private:
	CAtlRegExp<> regularExpression;
	CAtlREMatchContext<> matchContext;
	CStringArray matchStrings;
	CString patternString;
};