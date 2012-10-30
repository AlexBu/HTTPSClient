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
};


