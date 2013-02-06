#pragma once

#include "Info.h"

struct UserList
{
	DWORD count;
	CArray<UserInfo> user;
};

struct PassengerList
{
	DWORD count;
	CArray<PassengerInfo> pass;
};

struct CDNList
{
	DWORD count;
	CStringArray ip;
};

struct InputInfo
{
	UserInfo userinfo;
	PassengerList passengerlist;
	CString date;
	CString fromStation;
	CString toStation;
	CString trainNo;
};

struct CfgData
{
	CString version;
	UserList userlist;
	PassengerList passlist;
	CDNList cdnlist;
	InputInfo lastClostInfo;
};

class CConfig
{
public:
	CArray<UserInfo>& GetUser();
	CArray<PassengerInfo>& GetPassenger();
	InputInfo& GetInput();
	void SetUpdate();
private:
	BOOL LoadConfig();
	BOOL SaveConfig();

	void LoadDword( CFile &file, DWORD& n );
	void LoadString( CFile &file, CString& str );
	void LoadUserInfo( CFile &file, UserInfo& userinfo );
	void LoadPassInfo( CFile &file, PassengerInfo& passinfo );
	void LoadUserList( CFile &file, UserList& userlist );
	void LoadPassList( CFile &file, PassengerList& passlist );
	void LoadCDNList( CFile &file, CDNList& cdnlist );
	void LoadInputInfo( CFile &file, InputInfo& inputinfo );

	void SaveDword( CFile &file, DWORD n );
	void SaveString( CFile &file, CString& str );
	void SaveUserInfo( CFile &file, UserInfo& userinfo );
	void SavePassInfo( CFile &file, PassengerInfo& passinfo );
	void SaveUserList( CFile &file, UserList& userlist );
	void SavePassList( CFile &file, PassengerList& passlist );
	void SaveCDNList( CFile &file, CDNList& cdnlist );
	void SaveInputInfo( CFile &file, InputInfo& inputinfo );

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
