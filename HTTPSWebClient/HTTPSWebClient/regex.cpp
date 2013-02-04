#include "stdafx.h"
#include "regex.h"

BOOL CRegex::patternLoad(CString& pattern)
{
	patternString = pattern;
	return ( regularExpression.Parse( pattern ) != REPARSE_ERROR_OK ) ? FALSE : TRUE;
}

BOOL CRegex::contextMatch(CString& context, CString& rest)
{
	LPCTSTR afterMatch = NULL;
	if(regularExpression.Match( context, &matchContext, &afterMatch))
	{
		rest = afterMatch;
		return TRUE;
	}
	return FALSE;
}

BOOL CRegex::matchGet( DWORD matchIndex, CString& result )
{
	if(matchIndex >= matchContext.m_uNumGroups)
	{
		result.Empty();
		return FALSE;
	}

	const CAtlREMatchContext<>::RECHAR* szStart = 0;
	const CAtlREMatchContext<>::RECHAR* szEnd = 0;
	matchContext.GetMatch(matchIndex, &szStart, &szEnd);

	DWORD strSize = szEnd - szStart;
	LPTSTR strBuf = result.GetBuffer(strSize + 1);
	memcpy(strBuf, szStart, strSize*sizeof(TCHAR));
	result.ReleaseBuffer(strSize);

	return TRUE;
}
