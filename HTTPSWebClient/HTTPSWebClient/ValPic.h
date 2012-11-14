#pragma once


// CValPic

class CValPic : public CStatic
{
	DECLARE_DYNAMIC(CValPic)

public:
	CValPic();
	virtual ~CValPic();

protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnPaint();
private:
	DWORD height, width;
	BYTE* picBuff;
public:
	void imageAttrSet(DWORD h, DWORD w);
	void imageBuffSet(BYTE* buff);
};


