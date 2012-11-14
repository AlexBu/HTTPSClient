// ValPic.cpp : implementation file
//

#include "stdafx.h"
#include "ValPic.h"


// CValPic

IMPLEMENT_DYNAMIC(CValPic, CStatic)

CValPic::CValPic():width(0),height(0),picBuff(NULL)
{

}

CValPic::~CValPic()
{
}


BEGIN_MESSAGE_MAP(CValPic, CStatic)
	ON_WM_PAINT()
END_MESSAGE_MAP()



// CValPic message handlers



void CValPic::OnPaint()
{
	if(picBuff == NULL)
		return;
	CPaintDC dc(this); // device context for painting
	// TODO: Add your message handler code here
	// Do not call CStatic::OnPaint() for painting messages

	CBitmap bitmap;

	bitmap.CreateBitmap(width, height, 1, 24, picBuff);

	CDC memdc, *staticdc;
	staticdc = GetDC();
	memdc.CreateCompatibleDC(staticdc);
	CBitmap* old_bitmap = memdc.SelectObject(&bitmap);
	staticdc->BitBlt(0,0,width,height,&memdc,0,0,SRCCOPY);
	memdc.SelectObject(old_bitmap);
	bitmap.DeleteObject();
	memdc.DeleteDC();
	ReleaseDC(staticdc);
}

void CValPic::imageAttrSet( DWORD h, DWORD w )
{
	if(h > 0)	height = h;
	if(w > 0)	width = w;
}

void CValPic::imageBuffSet( BYTE* buff )
{
	if(buff)	picBuff = buff;
}
