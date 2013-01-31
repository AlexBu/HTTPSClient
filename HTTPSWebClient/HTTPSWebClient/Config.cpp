#include "StdAfx.h"
#include "Config.h"

CConfig::CConfig(void)
:fileName(L"config.bin")
,needUpdate(FALSE)
{
	LoadConfig();
}

CConfig::~CConfig(void)
{
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

BOOL CConfig::LoadConfig()
{
	// open file for loading
	CFile configFile;
	if(FALSE == configFile.Open(fileName, 
		CFile::modeCreate|CFile::modeNoTruncate|CFile::modeRead|CFile::shareDenyWrite))
	{
		configFile.Close();
		return FALSE;
	}
	
	// version
	configFile.Read(&cfgData.version, 16 * sizeof(TCHAR));

	// user info
	configFile.Read(&cfgData.userlist.count, sizeof(DWORD));

	if(cfgData.userlist.count < 256)
	{
		for(DWORD i = 0; i < cfgData.userlist.count; i++)
		{
			UserInfo user;
			configFile.Read(&user, sizeof(UserInfo));
			cfgData.userlist.user.Add(user);
		}
	}
	else
	{
		goto LOAD_FAIL;
	}
	
	// pass info
	configFile.Read(&cfgData.passlist.count, sizeof(DWORD));

	if(cfgData.passlist.count < 256)
	{
		for(DWORD i = 0; i < cfgData.userlist.count; i++)
		{
			PassInfo passenger;
			configFile.Read(&passenger, sizeof(PassInfo));
			cfgData.passlist.pass.Add(passenger);
		}
	}
	else
	{
		goto LOAD_FAIL;
	}

	// cdn list
	configFile.Read(&cfgData.cdnlist.count, sizeof(DWORD));

	if(cfgData.cdnlist.count < 256)
	{
		for(DWORD i = 0; i < cfgData.cdnlist.count; i++)
		{
			CDNIP cdn;
			configFile.Read(&cdn, sizeof(CDNIP));
			cfgData.cdnlist.ip.Add(cdn);
		}
	}
	else
	{
		goto LOAD_FAIL;
	}

	configFile.Close();
	return TRUE;
LOAD_FAIL:
	configFile.Close();
	return FALSE;
}

BOOL CConfig::SaveConfig()
{
	// open file for loading
	if(needUpdate)
		return TRUE;
	CFile configFile;
	configFile.Open(fileName, 
		CFile::modeCreate|CFile::modeWrite|CFile::shareDenyWrite);
	{
		configFile.Close();
		return FALSE;
	}

	// version
	configFile.Write(&cfgData.version, 16 * sizeof(TCHAR));

	// user info
	configFile.Write(&cfgData.userlist.count, sizeof(DWORD));


	for(DWORD i = 0; i < cfgData.userlist.count; i++)
	{
		configFile.Write(&cfgData.userlist.user[0], sizeof(UserInfo));
		cfgData.userlist.user.RemoveAt(0);
	}

	// pass info
	configFile.Write(&cfgData.passlist.count, sizeof(DWORD));


	for(DWORD i = 0; i < cfgData.userlist.count; i++)
	{
		configFile.Write(&cfgData.passlist.pass[0], sizeof(PassInfo));
		cfgData.passlist.pass.RemoveAt(0);
	}

	// cdn list
	configFile.Write(&cfgData.cdnlist.count, sizeof(DWORD));


	for(DWORD i = 0; i < cfgData.cdnlist.count; i++)
	{
		configFile.Write(&cfgData.cdnlist.ip[0], sizeof(CDNIP));
		cfgData.cdnlist.ip.RemoveAt(0);
	}

	configFile.Close();
	return TRUE;
}

void CConfig::ReleaseConfig()
{
	GetConfig().SaveConfig();
}

void CConfig::SetUpdate()
{
	needUpdate = TRUE;
}

DWORD CConfig::GetUserCount()
{
	GetConfig();
	return cfgData.userlist.count;
}

DWORD CConfig::GetPassengerCount()
{
	GetConfig();
	return cfgData.passlist.count;
}

UserInfo CConfig::GetUser( DWORD index)
{
	UserInfo userinfo;
	memset(&userinfo, 0, sizeof(UserInfo));

	GetConfig();
	if(index < cfgData.userlist.count)
		userinfo = cfgData.userlist.user[index];

	return userinfo;
}

PassInfo CConfig::GetPassenger( DWORD index)
{
	PassInfo passengerinfo;
	memset(&passengerinfo, 0, sizeof(PassInfo));

	GetConfig();
	if(index < cfgData.passlist.count)
		passengerinfo = cfgData.passlist.pass[index];

	return passengerinfo;
}
