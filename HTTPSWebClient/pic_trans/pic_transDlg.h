
// pic_transDlg.h : ͷ�ļ�
//

#pragma once
#include "validationctrl.h"
#include "afxcmn.h"
#include "afxwin.h"


// Cpic_transDlg �Ի���
class Cpic_transDlg : public CDialog
{
// ����
public:
	Cpic_transDlg(CWnd* pParent = NULL);	// ��׼���캯��

// �Ի�������
	enum { IDD = IDD_PIC_TRANS_DIALOG };

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
	CValPicCtrl picture;
	CSliderCtrl slider_lab;
	CStatic text_lab;
	afx_msg void OnNMCustomdrawSliderLab(NMHDR *pNMHDR, LRESULT *pResult);
	CStatic text_count;
};
