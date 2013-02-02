#include "StdAfx.h"
#include "Config.h"

CConfig::CConfig(void)
:fileName(L"config.bin")
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
	LoadString(configFile, cfgData.version);

	if(cfgData.version.IsEmpty())
		cfgData.version = L"0.1";

	// user list
	LoadUserList(configFile, cfgData.userlist);

	// pass list
	LoadPassList(configFile, cfgData.passlist);
	
	// cdn list
	LoadCDNList(configFile, cfgData.cdnlist);

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
	WriteString(configFile, cfgData.version);

	// user list
	WriteUserList(configFile, cfgData.userlist);

	// pass list
	WritePassList(configFile, cfgData.passlist);

	// cdn list
	WriteCDNList(configFile, cfgData.cdnlist);

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

CArray<PassInfo>& CConfig::GetPassenger()
{
	return cfgData.passlist.pass;
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

void CConfig::LoadPassInfo( CFile &file, PassInfo& passinfo )
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

void CConfig::LoadPassList( CFile &file, PassList& passlist )
{
	LoadDword(file, passlist.count);

	for(DWORD i = 0; i < passlist.count; i++)
	{
		PassInfo passinfo;
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

void CConfig::WriteDword( CFile &file, DWORD n )
{
	file.Write(&n, sizeof(DWORD));
}

void CConfig::WriteString( CFile &file, CString& str )
{
	DWORD count = str.GetLength() + 1;
	file.Write(&count, sizeof(DWORD));
	TCHAR* buf = new TCHAR[count];
	memcpy(buf, str, count * sizeof(TCHAR));
	file.Write(buf, sizeof(TCHAR)*count);
	delete []buf;
}

void CConfig::WriteUserInfo( CFile &file, UserInfo& userinfo )
{
	WriteString(file, userinfo.name);
	WriteString(file, userinfo.pass);
}

void CConfig::WritePassInfo( CFile &file, PassInfo& passinfo )
{
	WriteString(file, passinfo.name);
	WriteString(file, passinfo.passTyp);
	WriteString(file, passinfo.passNo);
	WriteString(file, passinfo.mobileNo);
	WriteString(file, passinfo.seatTyp);
}

void CConfig::WriteUserList( CFile &file, UserList& userlist )
{
	userlist.count = userlist.user.GetCount();

	WriteDword(file, userlist.count);

	for(DWORD i = 0; i < userlist.count; i++)
	{
		UserInfo& userinfo = userlist.user[i];
		WriteUserInfo(file, userinfo);
	}
}

void CConfig::WritePassList( CFile &file, PassList& passlist )
{
	passlist.count = passlist.pass.GetCount();

	WriteDword(file, passlist.count);

	for(DWORD i = 0; i < passlist.count; i++)
	{
		PassInfo& passinfo = passlist.pass[i];
		WritePassInfo(file, passinfo);
	}
}

void CConfig::WriteCDNList( CFile &file, CDNList& cdnlist )
{
	cdnlist.count = cdnlist.ip.GetCount();

	WriteDword(file, cdnlist.count);

	for(DWORD i = 0; i < cdnlist.count; i++)
	{
		CString& str = cdnlist.ip[i];
		WriteString(file, str);
	}
}
