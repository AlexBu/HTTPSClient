
// HTTPSWebClientDlg.h : ͷ�ļ�
//

#pragma once
#include "afxwin.h"

#include "ValidatePicture.h"

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
