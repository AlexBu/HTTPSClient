
// pic_transDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "pic_trans.h"
#include "pic_transDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// Cpic_transDlg 对话框




Cpic_transDlg::Cpic_transDlg(CWnd* pParent /*=NULL*/)
	: CDialog(Cpic_transDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void Cpic_transDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_STATIC_PIC, picture);
	DDX_Control(pDX, IDC_SLIDER_LAB, slider_lab);
	DDX_Control(pDX, IDC_STATIC_LAB, text_lab);
	DDX_Control(pDX, IDC_STATIC_COUNT, text_count);
}

BEGIN_MESSAGE_MAP(Cpic_transDlg, CDialog)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
	ON_NOTIFY(NM_CUSTOMDRAW, IDC_SLIDER_LAB, &Cpic_transDlg::OnNMCustomdrawSliderLab)
END_MESSAGE_MAP()


// Cpic_transDlg 消息处理程序

BOOL Cpic_transDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// 设置此对话框的图标。当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	slider_lab.SetRange(0,255);
	slider_lab.SetPos(255);

	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void Cpic_transDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 用于绘制的设备上下文

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 使图标在工作区矩形中居中
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 绘制图标
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR Cpic_transDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


void Cpic_transDlg::OnNMCustomdrawSliderLab(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMCUSTOMDRAW pNMCD = reinterpret_cast<LPNMCUSTOMDRAW>(pNMHDR);

	CString str;
	DWORD pos = slider_lab.GetPos();
	str.Format(L"%d", pos);
	text_lab.SetWindowText(str);

	float percent = picture.ApplyLValue(pos);
	str.Format(L"%.02f%%", percent*100);
	text_count.SetWindowText(str);
	picture.InvalidateRect(NULL, FALSE);
	*pResult = 0;
}
