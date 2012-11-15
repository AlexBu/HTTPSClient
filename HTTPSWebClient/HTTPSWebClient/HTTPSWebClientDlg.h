
// HTTPSWebClientDlg.h : ͷ�ļ�
//

#pragma once
#include "afxwin.h"

#include "ValPic.h"

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
public:
	CString POSTString;
	CString RespondString;
	CString AddrString;
	afx_msg void OnConnect();
	afx_msg void OnPost();
	afx_msg void OnGetValpic();
	CValPic picFrame;
	afx_msg void OnLogin();
};
