#include "stdafx.h"
#include "regex.h"

BOOL CRegex::patternLoad(CString& pattern)
{
	patternString = pattern;
	return ( regularExpression.Parse( pattern ) != REPARSE_ERROR_OK ) ? FALSE : TRUE;
}

CString CRegex::patternGet()
{
	return patternString;
}

BOOL CRegex::contextMatch(CString& context, CString& rest)
{
	LPCTSTR afterMatch = NULL;
	if(regularExpression.Match( context, &matchContext, &afterMatch))
	{
		rest = afterMatch;
		return TRUE;
	}
	else
	{
		rest.Empty();
		return FALSE;
	}
}

DWORD CRegex::matchCount()
{
	return matchContext.m_uNumGroups;
}

BOOL CRegex::matchGet( DWORD matchIndex, CString& result )
{
	if(matchIndex >= matchContext.m_uNumGroups)
		return FALSE;

	const CAtlREMatchContext<>::RECHAR* szStart = 0;
	const CAtlREMatchContext<>::RECHAR* szEnd = 0;
	matchContext.GetMatch(matchIndex, &szStart, &szEnd);

	DWORD strSize = szEnd - szStart;
	LPTSTR strBuf = result.GetBuffer(strSize + 1);
	memcpy(strBuf, szStart, strSize*sizeof(TCHAR));
	result.ReleaseBuffer(strSize);

	return TRUE;
}

CRegex::CRegex()
{

}

CRegex::~CRegex()
{

}
