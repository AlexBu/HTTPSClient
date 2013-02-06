#include "StdAfx.h"
#include "Config.h"

CConfig::CConfig(void)
:fileName(L"config.bin")
{
	LoadConfig();
}

CConfig::~CConfig( void )
{

}

CConfig& CConfig::GetConfig()
{
	static CConfig config;
	return config;
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
	LoadString(configFile, cfgData.version);

	if(cfgData.version.IsEmpty())
		cfgData.version = L"0.1";

	// user list
	LoadUserList(configFile, cfgData.userlist);

	// pass list
	LoadPassList(configFile, cfgData.passlist);
	
	// cdn list
	LoadCDNList(configFile, cfgData.cdnlist);

	// input info
	LoadInputInfo(configFile, cfgData.lastClostInfo);

	configFile.Close();
	return TRUE;
}

BOOL CConfig::SaveConfig()
{
	// open file for loading
	CFile configFile;
	if( FALSE == configFile.Open(fileName, 
		CFile::modeCreate|CFile::modeWrite|CFile::shareDenyWrite) )
	{
		configFile.Close();
		return FALSE;
	}

	// version
	SaveString(configFile, cfgData.version);

	// user list
	SaveUserList(configFile, cfgData.userlist);

	// pass list
	SavePassList(configFile, cfgData.passlist);

	// cdn list
	SaveCDNList(configFile, cfgData.cdnlist);

	// input info
	SaveInputInfo(configFile, cfgData.lastClostInfo);

	configFile.Close();
	return TRUE;
}

void CConfig::ReleaseConfig()
{
	GetConfig().cfgData.userlist.user.RemoveAll();
	GetConfig().cfgData.passlist.pass.RemoveAll();
	GetConfig().cfgData.cdnlist.ip.RemoveAll();
}

void CConfig::SetUpdate()
{
	SaveConfig();
}

CArray<UserInfo>& CConfig::GetUser()
{
	return cfgData.userlist.user;
}

CArray<PassengerInfo>& CConfig::GetPassenger()
{
	return cfgData.passlist.pass;
}

InputInfo& CConfig::GetInput()
{
	return cfgData.lastClostInfo;
}

void CConfig::LoadDword( CFile &file, DWORD& n )
{
	file.Read(&n, sizeof(DWORD));
}

void CConfig::LoadString( CFile &file, CString& str )
{
	DWORD count = 0;
	if(file.Read(&count, sizeof(DWORD)) == FALSE)
	{
		str.Empty();
		return;
	}
	TCHAR* buf = new TCHAR[count];
	if(file.Read(buf, sizeof(TCHAR)*count) == FALSE)
	{
		str.Empty();
		delete []buf;
		return;
	}
	str = buf;
	delete []buf;
}

void CConfig::LoadUserInfo( CFile &file, UserInfo& userinfo )
{
	LoadString(file, userinfo.name);
	LoadString(file, userinfo.pass);
}

void CConfig::LoadPassInfo( CFile &file, PassengerInfo& passinfo )
{
	LoadString(file, passinfo.name);
	LoadString(file, passinfo.passTyp);
	LoadString(file, passinfo.passNo);
	LoadString(file, passinfo.mobileNo);
	LoadString(file, passinfo.seatTyp);
}

void CConfig::LoadUserList( CFile &file, UserList& userlist )
{
	LoadDword(file, userlist.count);

	for(DWORD i = 0; i < userlist.count; i++)
	{
		UserInfo userinfo;
		LoadUserInfo(file, userinfo);
		userlist.user.Add(userinfo);
	}
}

void CConfig::LoadPassList( CFile &file, PassengerList& passlist )
{
	LoadDword(file, passlist.count);

	for(DWORD i = 0; i < passlist.count; i++)
	{
		PassengerInfo passinfo;
		LoadPassInfo(file, passinfo);
		passlist.pass.Add(passinfo);
	}
}

void CConfig::LoadCDNList( CFile &file, CDNList& cdnlist )
{
	LoadDword(file, cdnlist.count);

	for(DWORD i = 0; i < cdnlist.count; i++)
	{
		CString str;
		LoadString(file, str);
		cdnlist.ip.Add(str);
	}
}

void CConfig::LoadInputInfo( CFile &file, InputInfo& inputinfo )
{
	LoadUserInfo(file, inputinfo.userinfo);
	LoadPassList(file, inputinfo.passengerlist);
	LoadString(file, inputinfo.date);
	LoadString(file, inputinfo.fromStation);
	LoadString(file, inputinfo.toStation);
	LoadString(file, inputinfo.trainNo);
}

void CConfig::SaveDword( CFile &file, DWORD n )
{
	file.Write(&n, sizeof(DWORD));
}

void CConfig::SaveString( CFile &file, CString& str )
{
	DWORD count = str.GetLength() + 1;
	file.Write(&count, sizeof(DWORD));
	TCHAR* buf = new TCHAR[count];
	memcpy(buf, str, count * sizeof(TCHAR));
	file.Write(buf, sizeof(TCHAR)*count);
	delete []buf;
}

void CConfig::SaveUserInfo( CFile &file, UserInfo& userinfo )
{
	SaveString(file, userinfo.name);
	SaveString(file, userinfo.pass);
}

void CConfig::SavePassInfo( CFile &file, PassengerInfo& passinfo )
{
	SaveString(file, passinfo.name);
	SaveString(file, passinfo.passTyp);
	SaveString(file, passinfo.passNo);
	SaveString(file, passinfo.mobileNo);
	SaveString(file, passinfo.seatTyp);
}

void CConfig::SaveUserList( CFile &file, UserList& userlist )
{
	userlist.count = userlist.user.GetCount();

	SaveDword(file, userlist.count);

	for(DWORD i = 0; i < userlist.count; i++)
	{
		UserInfo& userinfo = userlist.user[i];
		SaveUserInfo(file, userinfo);
	}
}

void CConfig::SavePassList( CFile &file, PassengerList& passlist )
{
	passlist.count = passlist.pass.GetCount();

	SaveDword(file, passlist.count);

	for(DWORD i = 0; i < passlist.count; i++)
	{
		PassengerInfo& passinfo = passlist.pass[i];
		SavePassInfo(file, passinfo);
	}
}

void CConfig::SaveCDNList( CFile &file, CDNList& cdnlist )
{
	cdnlist.count = cdnlist.ip.GetCount();

	SaveDword(file, cdnlist.count);

	for(DWORD i = 0; i < cdnlist.count; i++)
	{
		CString& str = cdnlist.ip[i];
		SaveString(file, str);
	}
}

void CConfig::SaveInputInfo( CFile &file, InputInfo& inputinfo )
{
	SaveUserInfo(file, inputinfo.userinfo);
	SavePassList(file, inputinfo.passengerlist);
	SaveString(file, inputinfo.date);
	SaveString(file, inputinfo.fromStation);
	SaveString(file, inputinfo.toStation);
	SaveString(file, inputinfo.trainNo);
}
