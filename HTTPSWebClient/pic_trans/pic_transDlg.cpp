
// pic_transDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "pic_trans.h"
#include "pic_transDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// Cpic_transDlg �Ի���




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


// Cpic_transDlg ��Ϣ�������

BOOL Cpic_transDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// ���ô˶Ի����ͼ�ꡣ��Ӧ�ó��������ڲ��ǶԻ���ʱ����ܽ��Զ�
	//  ִ�д˲���
	SetIcon(m_hIcon, TRUE);			// ���ô�ͼ��
	SetIcon(m_hIcon, FALSE);		// ����Сͼ��

	slider_lab.SetRange(0,255);
	slider_lab.SetPos(255);

	return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
}

// �����Ի��������С����ť������Ҫ����Ĵ���
//  �����Ƹ�ͼ�ꡣ����ʹ���ĵ�/��ͼģ�͵� MFC Ӧ�ó���
//  �⽫�ɿ���Զ���ɡ�

void Cpic_transDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // ���ڻ��Ƶ��豸������

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// ʹͼ���ڹ����������о���
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// ����ͼ��
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

//���û��϶���С������ʱϵͳ���ô˺���ȡ�ù��
//��ʾ��
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
