#pragma once
#include "Config.h"
#include "afxwin.h"

// CEditPassenger dialog

class CEditPassenger : public CDialog
{
	DECLARE_DYNAMIC(CEditPassenger)

public:
	CEditPassenger(CWnd* pParent = NULL);   // standard constructor
	virtual ~CEditPassenger();

// Dialog Data
	enum { IDD = IDD_DIALOG_EDIT_PASSENGER };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	PassengerInfo passenger;
	virtual BOOL OnInitDialog();
protected:
	virtual void OnOK();
	void InitList();

	void LoadPassenger();
	void SavePassenger();
public:
	CString passname;
	CString passmobile;
	CString passidno;
	CComboBox cboxSeatType;
	CComboBox cboxIdTyp;
};
