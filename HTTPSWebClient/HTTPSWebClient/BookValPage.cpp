#include "StdAfx.h"
#include "BookValPage.h"
#include "jpg.h"

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
	refStr = L"Referer: https://dynamic.12306.cn/otsweb/order/confirmPassengerAction.do?method=init";
}

void CBookValPage::ParseOutput()
{
	if(status == ERROR_HTTP)
		return;
	// transform into bmp format
	height = bmpHeightGet(buff, size);
	width = bmpWidthGet(buff, size);

	if(height == 0 || width == 0)
	{
		status = ERROR_VALIDATE;
		return;
	}

	if(bmpFromJpeg(buff, size, bmpbuff, &bmpsize) != 0)
	{
		// set data to zero
		bmpsize = 0;
		status = ERROR_VALIDATE;
		return;
	}

	status = ERROR_OK;
}
