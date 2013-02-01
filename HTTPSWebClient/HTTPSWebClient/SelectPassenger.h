#pragma once
#include "Config.h"

// CSelectPassenger dialog

class CSelectPassenger : public CDialog
{
	DECLARE_DYNAMIC(CSelectPassenger)

public:
	CSelectPassenger(CWnd* pParent = NULL);   // standard constructor
	virtual ~CSelectPassenger();

// Dialog Data
	enum { IDD = IDD_DIALOG_SELECT_PASS };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	DECLARE_MESSAGE_MAP()

public:
	void OnSelectPassenger();

public:
	CListBox listboxPassenger;
	CStringArray passOutside;
	CArray<PassInfo> selectedpass;
};
