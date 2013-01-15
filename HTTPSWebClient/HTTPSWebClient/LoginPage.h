#pragma once
#include "Page.h"

class CLoginPage : public CPage
{
public:
	CLoginPage(void);
	~CLoginPage(void);
	void BuildRequest( LoginInfo& input );
	void ParseOutput();
};
