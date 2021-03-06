#include "StdAfx.h"
#include "BookValPage.h"
#include "pngdecode.h"

CBookValPage::CBookValPage(void)
:height(0)
,width(0)
,bmpsize(0)
{
	bmpbuff = new BYTE[MAX_DATA_SIZE];
}

CBookValPage::~CBookValPage(void)
{
	if(bmpbuff)
	{
		delete []bmpbuff;
		bmpbuff = NULL;
	}
}

void CBookValPage::BuildRequest()
{
	reqStr = L"/otsweb/passCodeAction.do?rand=randp";
	refStr = L"/otsweb/order/confirmPassengerAction.do?method=init";
}

void CBookValPage::ParseOutput()
{
	if(status != ERROR_OK)
		return;
	// transform into bmp format
	if(bmpFromPng(buff, size, bmpbuff, &bmpsize, &height, &width) != 0)
	{
		// set data to zero
		bmpsize = 0;
		status = ERROR_VALIDATE;
		return;
	}

	status = ERROR_OK;
}
