#include "StdAfx.h"
#include "Config.h"

CConfig::CConfig(void)
{
	CString fileName;
	fileName.Format(L"config.bin", GetCurrentProcessId());
	//configFile.Open(fileName, 
	//	CFile::modeCreate|CFile::modeNoTruncate|CFile::modeWrite|CFile::shareDenyWrite);
	
}

CConfig::~CConfig(void)
{
	configFile.Close();
}

CConfig& CConfig::GetConfig()
{
	static CConfig config;
	return config;
}

CConfig& CConfig::operator=( CConfig const& config )
{
	return GetConfig();
}
