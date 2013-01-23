#pragma once
#include "page.h"

class CBookValPage : public CPage
{
public:
	CBookValPage(void);
	~CBookValPage(void);
	void BuildRequest();
	void ParseOutput();
public:
	unsigned height, width;
	DWORD bmpsize;
	BYTE* bmpbuff;
};
