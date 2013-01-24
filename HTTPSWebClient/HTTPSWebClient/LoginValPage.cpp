#include "StdAfx.h"
#include "LoginValPage.h"
#include "jpg.h"

CLoginValPage::CLoginValPage(void)
:height(0)
,width(0)
,bmpsize(0)
{
	bmpbuff = new BYTE[MAX_DATA_SIZE];
}

CLoginValPage::~CLoginValPage(void)
{
	if(bmpbuff)
	{
		delete []bmpbuff;
		bmpbuff = NULL;
	}
}

void CLoginValPage::BuildRequest()
{
	reqStr = L"/otsweb/passCodeAction.do?rand=sjrand";
	refStr = L"Referer: https://dynamic.12306.cn/otsweb/loginAction.do?method=init";
}

void CLoginValPage::ParseOutput()
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
