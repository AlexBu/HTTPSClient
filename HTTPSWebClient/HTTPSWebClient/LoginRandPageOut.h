#pragma once

class CLoginRandPageOut
{
public:
	CLoginRandPageOut(void);
	~CLoginRandPageOut(void);
	void randSet( CString& str );
	CString randGet();
private:
	CString rand;
};
