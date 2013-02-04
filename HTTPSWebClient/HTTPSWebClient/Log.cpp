#include "StdAfx.h"
#include "Log.h"

CLog::CLog(void)
{
#ifdef LOG_TO_FILE
	CString fileName;
	fileName.Format(L"http_%x.log", GetCurrentProcessId());
	//fileName.Format(L"http.log");
	logFile.Open(fileName, 
	CFile::modeCreate|CFile::modeNoTruncate|CFile::modeWrite|CFile::shareDenyWrite);
	logFile.SeekToEnd();
#endif
}

CLog::CLog( const CLog& log )
{

}

CLog::~CLog(void)
{
#ifdef LOG_TO_FILE
	logFile.Close();
#endif
}

void CLog::AddLog( CString content )
{
#ifdef LOG_TO_FILE
	// format
	// [2013.01.01 12:59:59] <log content>
	CTime currentTime = CTime::GetCurrentTime();
	CString str = currentTime.Format(L"[%Y.%m.%d %H:%M:%S] ");
	logFile.Write(str.GetString(), str.GetLength()*sizeof(TCHAR));
	logFile.Write(content.GetString(), content.GetLength()*sizeof(TCHAR));
	str = L"\r\n";
	logFile.Write(str.GetString(), str.GetLength()*sizeof(TCHAR));
#endif
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
