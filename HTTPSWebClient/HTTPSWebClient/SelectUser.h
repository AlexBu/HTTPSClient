#pragma once
#include "afxwin.h"

#include "Config.h"

// CSelectUser dialog

class CSelectUser : public CDialog
{
	DECLARE_DYNAMIC(CSelectUser)

public:
	CSelectUser(CWnd* pParent = NULL);   // standard constructor
	virtual ~CSelectUser();

// Dialog Data
	enum { IDD = IDD_DIALOG_SELECT_USER };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	afx_msg void OnDblClkUserList();
	DECLARE_MESSAGE_MAP()

public:
	void OnSelectUser();

public:
	CListBox listboxUser;
	UserInfo selecteduser;
};
