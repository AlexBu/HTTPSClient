
// HTTPSWebClientDlg.h : 头文件
//

#pragma once
#include "afxwin.h"

#include "ValidationCtrl.h"

#include "Config.h"
#include "SelectUser.h"
#include "SelectPassenger.h"
#include "EditPassenger.h"
#include "afxdtctl.h"

// CHTTPSWebClientDlg 对话框
class CHTTPSWebClientDlg : public CDialog
{
// 构造
public:
	CHTTPSWebClientDlg(CWnd* pParent = NULL);	// 标准构造函数

// 对话框数据
	enum { IDD = IDD_HTTPSWEBCLIENT_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持


// 实现
protected:
	HICON m_hIcon;

	// 生成的消息映射函数
	virtual BOOL OnInitDialog();

	

	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg LRESULT OnSetStr(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnLogin(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnGetCode(WPARAM wParam, LPARAM lParam);
	afx_msg void OnBook();
	afx_msg void OnSelectUser();
	afx_msg void OnSelectPassenger();
	afx_msg void OnNMDblclkListBoardPassenger(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnAddBoardPassenger();
	afx_msg void OnRemoveBoardPassenger();
	afx_msg void OnClose();
	DECLARE_MESSAGE_MAP()

public:
	void CollectInputInfo();

	void LoadConfig();

	void UpdateBoardPassengerListCtrl();
	void InitPassengerListCtrl();

	void UpdateUser( UserInfo &userinfo );
	void UpdateOneBoardPassenger( PassengerInfo& passinfo );

	void UpdateUserListConfig();
	void UpdatePassengerListConfig();
	void InitDate();
	void GetLastCloseInput();
	void SetLastCloseInput();
private:
	CString usernameStr;
	CString passwordStr;
	CString trainNo;
	CString fromStation;
	CString toStation;

public:
	CEdit outputBox;
	CListCtrl ctrlPassengerList;
	UserInfo dataUserInfo;
	CArray<PassengerInfo> dataPassengerList;
	CDateTimeCtrl ctrlDate;
};
