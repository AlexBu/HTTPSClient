#pragma once

class CLog
{
public:
	void AddLog(CString content);
	static CLog& GetLog();
private:
	CLog(void);
	CLog(const CLog& log);
	CLog& operator=(CLog const& log);
	~CLog(void);
	CFile logFile;
};
