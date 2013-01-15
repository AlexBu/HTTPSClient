#pragma once
#include "Page.h"

class CConfirmPage : public CPage
{
public:
	CConfirmPage(void);
	~CConfirmPage(void);
	void BuildRequest( OrderInfo& input );
	void ParseOutput( OrderInfo& output );
};
