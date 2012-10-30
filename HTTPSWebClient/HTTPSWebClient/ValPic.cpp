// ValPic.cpp : implementation file
//

#include "stdafx.h"
#include "ValPic.h"


// CValPic

IMPLEMENT_DYNAMIC(CValPic, CStatic)

CValPic::CValPic()
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
	CPaintDC dc(this); // device context for painting
	// TODO: Add your message handler code here
	// Do not call CStatic::OnPaint() for painting messages

	CBitmap bitmap;
	//bitmap.CreateBitmap(240,128,1,1,data);

	CDC memdc;
	memdc.CreateCompatibleDC(&dc);
	memdc.SelectObject(&bitmap);
	dc.StretchBlt(0,0,240,128,&memdc,0,0,240,128,NOTSRCCOPY);
	bitmap.DeleteObject();
	this->ReleaseDC(&dc);
	memdc.DeleteDC();
}
