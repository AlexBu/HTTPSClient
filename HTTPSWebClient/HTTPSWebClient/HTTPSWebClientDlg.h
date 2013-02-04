
// HTTPSWebClientDlg.h : ͷ�ļ�
//

#pragma once
#include "afxwin.h"

#include "ValidationCtrl.h"

#include "Config.h"
#include "SelectUser.h"
#include "SelectPassenger.h"
#include "EditPassenger.h"
#include "afxdtctl.h"

// CHTTPSWebClientDlg �Ի���
class CHTTPSWebClientDlg : public CDialog
{
// ����
public:
	CHTTPSWebClientDlg(CWnd* pParent = NULL);	// ��׼���캯��

// �Ի�������
	enum { IDD = IDD_HTTPSWEBCLIENT_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV ֧��


// ʵ��
protected:
	HICON m_hIcon;

	// ���ɵ���Ϣӳ�亯��
	virtual BOOL OnInitDialog();

	

	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg LRESULT OnSetStr(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnLogin(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnGetCode(WPARAM wParam, LPARAM lParam);
	afx_msg void OnBook();
	afx_msg void OnSelectUser();
	afx_msg void OnSelectPassenger();
	afx_msg void OnNMDblclkListPassengerBoard(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnAddPassenger();
	afx_msg void OnRemovePassenger();
	DECLARE_MESSAGE_MAP()

public:
	void GetPassengerInfo();
	void GetUserInfo();

	void LoadConfig();

	void UpdatePassengerListCtrl();
	void InitPassengerListCtrl();

	void UpdateUser( UserInfo &userinfo );
	void UpdateOneBoardPassenger( PassInfo& passinfo );

	void UpdateUserListConfig();
	void UpdatePassListConfig();
	void InitDate();
private:
	CString usernameStr;
	CString passwordStr;
	CString trainNo;
	CString stationFrom;
	CString stationTo;

public:
	CEdit outputBox;
	CListCtrl listctrlPassengers;
	UserInfo infoUser;
	CArray<PassInfo> listPassengers;
	CDateTimeCtrl ctrlDate;
};
