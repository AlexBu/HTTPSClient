
// HTTPSWebClientDlg.h : ͷ�ļ�
//

#pragma once


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
};
