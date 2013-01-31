#pragma once

struct UserInfo
{
	TCHAR name[8];
	TCHAR pass[256];
};

struct UserList
{
	DWORD count;
	CArray<UserInfo> user;
};

struct PassInfo
{
	TCHAR name[8];
	DWORD passTyp;
	TCHAR passNo[256];
	TCHAR mobileNo[16];
	DWORD seatTyp;
};

struct PassList
{
	DWORD count;
	CArray<PassInfo> pass;
};

struct CDNIP
{
	TCHAR IP[16];
};
struct CDNList
{
	DWORD count;
	CArray<CDNIP> ip;
};

struct CfgData
{
	TCHAR version[16];
	UserList userlist;
	PassList passlist;
	CDNList cdnlist;
};

class CConfig
{
public:
	DWORD GetUserCount();
	DWORD GetPassengerCount();
	UserInfo GetUser(DWORD);
	PassInfo GetPassenger(DWORD);
	void SetUpdate();
private:
	BOOL LoadConfig();
	BOOL SaveConfig();
	CConfig(void);
	CConfig(const CConfig& config);
	~CConfig(void);
	CConfig& operator=(CConfig const& config);
private:
	CfgData cfgData;
	CString fileName;
	BOOL	needUpdate;
public:
	static CConfig& GetConfig();
	static void ReleaseConfig();
};
