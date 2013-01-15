#pragma once
#include "Page.h"

class CLoginRandPage : public CPage
{
public:
	CLoginRandPage(void);
	~CLoginRandPage(void);
	void BuildRequest();
	void ParseOutput( LoginInfo& output );
};
