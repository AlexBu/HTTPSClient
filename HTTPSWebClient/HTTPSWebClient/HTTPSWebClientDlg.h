
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

protected:
	afx_msg void OnGetValLoginPic();
	afx_msg void OnLogin();
	afx_msg void OnGetValBookPic();
	afx_msg void OnQuery();
	afx_msg void OnBook();

public:
	void ConnectToSite();
	void InitUserInfo();
	void InitPassengerInfo();
	void GetPassengerInfo();
	void GetUserInfo();

private:
	CString dateString;	
	CString AddrString;
	CString usernameStr;
	CString passwordStr;
	CString validateStr;
	CString validateBookStr;

	CString nameP1;
	CString identityTypeP1;
	CString identityNoP1;
	CString mobileP1;
	CString seatTypeP1;

	CString nameP2;
	CString identityTypeP2;
	CString identityNoP2;
	CString mobileP2;
	CString seatTypeP2;

	CString trainNo;

	CString stationFrom;
	CString stationTo;

public:
	CValPic valPicLoginCtrl;
	CValPic valPicBookCtrl;
	CString RespondString;

};
