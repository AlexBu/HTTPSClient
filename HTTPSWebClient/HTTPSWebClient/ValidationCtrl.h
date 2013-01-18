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
public:
	void imageAttrSet(DWORD h, DWORD w);
	void imageBuffSet(BYTE* buff, DWORD size);
};


