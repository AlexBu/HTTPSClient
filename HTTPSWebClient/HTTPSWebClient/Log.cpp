#include "StdAfx.h"
#include "Log.h"

CLog::CLog(void)
{
	CString fileName;
	fileName.Format(L"http_%x.log", GetCurrentProcessId());
	logFile.Open(fileName, 
	CFile::modeCreate|CFile::modeNoTruncate|CFile::modeWrite|CFile::shareDenyWrite);
	logFile.SeekToEnd();
}

CLog::CLog( const CLog& log )
{

}

CLog::~CLog(void)
{
	logFile.Close();
}

void CLog::AddLog( CString content )
{
	// format
	// <timestamp>
	// [process id]:[date]:[time]
	// </timestamp>
	// <content>
	// log content
	// </content>
	CString str;
	str.Format(L"<timestamp>\r\n[%x]:",GetCurrentProcessId());
	CTime currentTime = CTime::GetCurrentTime();
	str += currentTime.Format(L"[%Y-%m-%d]:[%H-%M-%S]\r\n");
	str += L"</timestamp>\r\n<content>\r\n";
	logFile.Write(str.GetString(), str.GetLength()*sizeof(TCHAR));
	logFile.Write(content.GetString(), content.GetLength()*sizeof(TCHAR));
	str = L"\r\n</content>\r\n\r\n";
	logFile.Write(str.GetString(), str.GetLength()*sizeof(TCHAR));
}

CLog& CLog::GetLog()
{
	static CLog log;
	return log;
}

CLog& CLog::operator=( CLog const& log )
{
	return GetLog();
}
