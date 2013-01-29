#pragma once
#include "page.h"

class CLoginValPage : public CPage
{
public:
	CLoginValPage(void);
	~CLoginValPage(void);
	void BuildRequest();
	void ParseOutput();
	void ParseOutputPng();
public:
	unsigned int height, width;
	DWORD bmpsize;
	BYTE* bmpbuff;
};
