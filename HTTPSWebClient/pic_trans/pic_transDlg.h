
// pic_transDlg.h : 头文件
//

#pragma once
#include "validationctrl.h"
#include "afxcmn.h"
#include "afxwin.h"


// Cpic_transDlg 对话框
class Cpic_transDlg : public CDialog
{
// 构造
public:
	Cpic_transDlg(CWnd* pParent = NULL);	// 标准构造函数

// 对话框数据
	enum { IDD = IDD_PIC_TRANS_DIALOG };

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
	CValPicCtrl picture;
	CSliderCtrl slider_lab;
	CStatic text_lab;
	afx_msg void OnNMCustomdrawSliderLab(NMHDR *pNMHDR, LRESULT *pResult);
	CStatic text_count;
};
