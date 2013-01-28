#pragma once


// CValPic

class CValPicCtrl : public CStatic
{
	DECLARE_DYNAMIC(CValPicCtrl)

public:
	CValPicCtrl();
	virtual ~CValPicCtrl();

protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnPaint();
private:
	DWORD height, width;
	BYTE* picBuff;
	BOOL hasData;
	BITMAPINFO bminfo;
	DWORD size;
	DWORD LabConst[256][9];
	BYTE* bakBuff;
public:
	void LoadBmp();
	void LoadLabConst();
	float ApplyLValue(DWORD l);
};


