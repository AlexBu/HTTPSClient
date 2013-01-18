#pragma once
#include "afxwin.h"
#include "ValidationCtrl.h"

// CValidationDialog dialog

class CValidationDialog : public CDialog
{
	DECLARE_DYNAMIC(CValidationDialog)

public:
	CValidationDialog(CWnd* pParent = NULL);   // standard constructor
	virtual ~CValidationDialog();

// Dialog Data
	enum { IDD = IDD_DIALOG_VALIDATE_CODE };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	CValPicCtrl pic;
	CString code;
};
