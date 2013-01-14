#pragma once

class CLoginRandPageOut
{
public:
	CLoginRandPageOut(void);
	~CLoginRandPageOut(void);
	void randSet( CString& str );
private:
	CString rand;
};
