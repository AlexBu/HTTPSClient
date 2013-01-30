// ValPic.cpp : implementation file
//

#include "stdafx.h"
#include "ValidationCtrl.h"

// CValPic

IMPLEMENT_DYNAMIC(CValPicCtrl, CStatic)

CValPicCtrl::CValPicCtrl():width(0),height(0),hasData(FALSE),picBuff(NULL),bakBuff(NULL)
{
	LoadLabConst();
	LoadBmp();
}

CValPicCtrl::~CValPicCtrl()
{
	hasData = FALSE;
	if(picBuff)
	{
		delete []picBuff;
		picBuff = NULL;
	}
	if(bakBuff)
	{
		delete []bakBuff;
		bakBuff = NULL;
	}
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
	memdc.CreateCompatibleDC(&dc);

	void* dibBuff = NULL;
	dibHandle = CreateDIBSection(memdc, &bminfo, DIB_RGB_COLORS, &dibBuff, 0, 0);
	memcpy(dibBuff, bakBuff, size);

	HBITMAP old_bitmap = (HBITMAP)SelectObject(memdc, dibHandle);

	//dc.BitBlt(0,0,width,height,&memdc,0,0,SRCCOPY);
	dc.StretchBlt(0,0,width*4,height*4, &memdc, 0, 0, width, height, SRCCOPY);

	memdc.SelectObject(old_bitmap);
	DeleteObject(dibHandle);
	memdc.DeleteDC();

}

void CValPicCtrl::LoadBmp()
{
	memset(bminfo.bmiColors, 0, sizeof(RGBQUAD));

	CFile bmpFile(L"test.bmp", CFile::modeRead);
	BITMAPFILEHEADER fileheader;
	bmpFile.Read(&fileheader, sizeof(BITMAPFILEHEADER));
	bmpFile.Read(&bminfo.bmiHeader, sizeof(BITMAPINFOHEADER));
	height = (bminfo.bmiHeader.biHeight > 0) ? bminfo.bmiHeader.biHeight : (-bminfo.bmiHeader.biHeight);
	width = bminfo.bmiHeader.biWidth;
	size = height*width*bminfo.bmiHeader.biBitCount / 8;
	if(picBuff)
		delete []picBuff;
	picBuff = new BYTE[size];
	bmpFile.Read(picBuff, size);
	if(bakBuff)
		delete []bakBuff;
	bakBuff = new BYTE[size];
	memcpy(bakBuff, picBuff, size);
	bmpFile.Close();

	hasData = TRUE;
}

void CValPicCtrl::LoadLabConst()
{
	// calc only once
	memset(LabConst[0], 0, sizeof(DWORD)*9);
	for(int i = 1; i < 256; i++)
	{
		LabConst[i][0] = LabConst[i-1][0] + 13933;
		LabConst[i][1] = LabConst[i-1][1] + 46871;
		LabConst[i][2] = LabConst[i-1][2] + 4732;

		LabConst[i][3] = LabConst[i-1][3] + 5467631;
		LabConst[i][4] = LabConst[i-1][4] + 8376186;
		LabConst[i][5] = LabConst[i-1][5] + 2908178;

		LabConst[i][6] = LabConst[i-1][6] + 2043680;
		LabConst[i][7] = LabConst[i-1][7] + 6351200;
		LabConst[i][8] = LabConst[i-1][8] + 8394880;
	}
}

float CValPicCtrl::ApplyLValue( DWORD l )
{
	DWORD count = 0;
	float total = (float)width * height;
	for(DWORD i = 0; i < width * height; i++)
	{
		BYTE b = picBuff[3*i];
		BYTE g = picBuff[3*i + 1];
		BYTE r = picBuff[3*i + 2];

		DWORD lab = (LabConst[r][0] + LabConst[g][1] + LabConst[b][2]) >> 16;

		if(lab < l)
		{
			// reserve
			bakBuff[3*i]		=	r;
			bakBuff[3*i + 1]	=	g;
			bakBuff[3*i + 2]	=	b;
		}
		else
		{
			count ++;
			// set to white
			bakBuff[3*i]		=	0xff;
			bakBuff[3*i + 1]	=	0xff;
			bakBuff[3*i + 2]	=	0xff;
		}
	}

	return( count / total);
}
