#pragma once
#include "Page.h"

class CRandPage : public CPage
{
public:
	CRandPage(void);
	~CRandPage(void);
	void BuildRequest();
	void ParseOutput( LoginInfo& output );
};
