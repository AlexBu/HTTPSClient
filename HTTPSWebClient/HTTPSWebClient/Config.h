#pragma once

#include "Info.h"

struct UserList
{
	DWORD count;
	CArray<UserInfo> user;
};

struct PassList
{
	DWORD count;
	CArray<PassInfo> pass;
};

struct CDNList
{
	DWORD count;
	CStringArray ip;
};

struct CfgData
{
	CString version;
	UserList userlist;
	PassList passlist;
	CDNList cdnlist;
};

class CConfig
{
public:
	CArray<UserInfo>& GetUser();
	CArray<PassInfo>& GetPassenger();
	void SetUpdate();
private:
	BOOL LoadConfig();
	BOOL SaveConfig();

	void LoadDword( CFile &file, DWORD& n );
	void LoadString( CFile &file, CString& str );
	void LoadUserInfo( CFile &file, UserInfo& userinfo );
	void LoadPassInfo( CFile &file, PassInfo& passinfo );
	void LoadUserList( CFile &file, UserList& userlist );
	void LoadPassList( CFile &file, PassList& passlist );
	void LoadCDNList( CFile &file, CDNList& cdnlist );

	void WriteDword( CFile &file, DWORD n );
	void WriteString( CFile &file, CString& str );
	void WriteUserInfo( CFile &file, UserInfo& userinfo );
	void WritePassInfo( CFile &file, PassInfo& passinfo );
	void WriteUserList( CFile &file, UserList& userlist );
	void WritePassList( CFile &file, PassList& passlist );
	void WriteCDNList( CFile &file, CDNList& cdnlist );

	CConfig(void);
	CConfig(const CConfig& config);
	~CConfig(void);
	CConfig& operator=(CConfig const& config);
private:
	CfgData cfgData;
	CString fileName;
public:
	static CConfig& GetConfig();
	static void ReleaseConfig();
};
