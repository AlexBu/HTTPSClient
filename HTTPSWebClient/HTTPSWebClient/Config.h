#pragma once

class CConfig
{
public:
	int GetUserNum();
	int GetPassengerNum();
	int GetUser(int);
	int GetPassenger(int);
	int AddUser();
	int AddPassenger();
	static CConfig& GetConfig();
private:
	CConfig(void);
	CConfig(const CConfig& config);
	~CConfig(void);
	CConfig& operator=(CConfig const& config);
	CFile configFile;
};
