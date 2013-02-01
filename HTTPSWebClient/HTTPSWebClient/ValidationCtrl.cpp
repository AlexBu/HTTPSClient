// ValPic.cpp : implementation file
//

#include "stdafx.h"
#include "ValidationCtrl.h"

// CValPic

IMPLEMENT_DYNAMIC(CValPicCtrl, CStatic)

CValPicCtrl::CValPicCtrl():width(0),height(0),hasData(FALSE)
{
	picBuff = new BYTE[SMALL_PIC_SIZE];
}

CValPicCtrl::~CValPicCtrl()
{
	hasData = FALSE;
	delete []picBuff;
}


BEGIN_MESSAGE_MAP(CValPicCtrl, CStatic)
	ON_WM_PAINT()
END_MESSAGE_MAP()



// CValPic message handlers



void CValPicCtrl::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	// TODO: Add your message handler code here
	// Do not call CStatic::OnPaint() for painting messages

	if(hasData == FALSE)
		return;

	CDC memdc;
	HBITMAP dibHandle;

	BITMAPINFO bminfo;
	bminfo.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
	bminfo.bmiHeader.biWidth = width;
	bminfo.bmiHeader.biHeight = height;	// top-bottom picture
	bminfo.bmiHeader.biPlanes = 1;
	bminfo.bmiHeader.biBitCount = 24;
	bminfo.bmiHeader.biCompression = BI_RGB;
	bminfo.bmiHeader.biSizeImage = 0;	// can be 0 if RGB mode	
	bminfo.bmiHeader.biXPelsPerMeter = 72;	// not used
	bminfo.bmiHeader.biYPelsPerMeter = 72;	// not used
	bminfo.bmiHeader.biClrUsed = 0;
	bminfo.bmiHeader.biClrImportant = 0;
	memset(bminfo.bmiColors, 0, sizeof(RGBQUAD));

	memdc.CreateCompatibleDC(&dc);

	void* dibBuff = NULL;
	dibHandle = CreateDIBSection(memdc, &bminfo, DIB_RGB_COLORS, &dibBuff, 0, 0);
	memcpy(dibBuff, picBuff, width*height*3);

	HBITMAP old_bitmap = (HBITMAP)SelectObject(memdc, dibHandle);

	//dc.BitBlt(0,0,width,height,&memdc,0,0,SRCCOPY);
	// enlarge 2x for better view
	dc.StretchBlt(0,0,width*2,height*2, &memdc, 0, 0, width, height, SRCCOPY);

	memdc.SelectObject(old_bitmap);
	DeleteObject(dibHandle);
	memdc.DeleteDC();

}

void CValPicCtrl::imageAttrSet( DWORD h, DWORD w )
{
	if(h > 0)	height = h;
	if(w > 0)	width = w;
}

void CValPicCtrl::imageBuffSet( BYTE* buff, DWORD size )
{
	if(buff && size < SMALL_PIC_SIZE)
	{
		memcpy(picBuff, buff, size);
		hasData = TRUE;
	}
}
