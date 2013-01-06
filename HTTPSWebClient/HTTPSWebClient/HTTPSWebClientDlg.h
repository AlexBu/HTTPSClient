
// HTTPSWebClientDlg.h : 头文件
//

#pragma once
#include "afxwin.h"

#include "ValidatePicture.h"

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
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnGet();

	void Test_SetURL();

	void CheckURL();
private:
	CString WebResString;
	CString dateString;
public:
	CString POSTString;
	CString RespondString;
	CString AddrString;
	void ConnectToSite();
	afx_msg void OnPost();
	afx_msg void OnGetValLoginPic();
	CValPic valPicLoginCtrl;
	afx_msg void OnLogin();
	CString usernameStr;
	CString passwordStr;
	CString validateStr;
	CValPic valPicBookCtrl;
	afx_msg void OnGetValBookPic();
	afx_msg void OnQuery();

	afx_msg void OnBook();
};
