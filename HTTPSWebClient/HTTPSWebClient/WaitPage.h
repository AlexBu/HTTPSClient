#pragma once
#include "Page.h"

class CWaitPage : public CPage
{
public:
	CWaitPage(void);
	~CWaitPage(void);
	void BuildRequest( OrderInfo& input );
	void ParseOutput( OrderInfo& output );
};
