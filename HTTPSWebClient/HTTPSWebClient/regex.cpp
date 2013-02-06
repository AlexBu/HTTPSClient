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

BOOL CRegex::GetStringByPattern(CStringArray& strList, CString& content, CString& pattern)
{
	if( regularExpression.Parse( pattern ) != REPARSE_ERROR_OK )
		return FALSE;
	LPCTSTR afterMatch = NULL;
	if(!regularExpression.Match( content, &matchContext, &afterMatch))
		return FALSE;

	for(UINT i = 0; i < matchContext.m_uNumGroups; i++)
	{
		const CAtlREMatchContext<>::RECHAR* szStart = 0;
		const CAtlREMatchContext<>::RECHAR* szEnd = 0;
		matchContext.GetMatch(i, &szStart, &szEnd);

		DWORD strSize = szEnd - szStart;
		CString str;
		LPTSTR strBuf = str.GetBuffer(strSize + 1);
		memcpy(strBuf, szStart, strSize*sizeof(TCHAR));
		str.ReleaseBuffer(strSize);
		strList.Add(str);
	}
	return TRUE;
}