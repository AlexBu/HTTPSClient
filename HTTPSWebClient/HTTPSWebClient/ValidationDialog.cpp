// ValidationDialog.cpp : implementation file
//

#include "stdafx.h"
#include "HTTPSWebClient.h"
#include "ValidationDialog.h"


// CValidationDialog dialog

IMPLEMENT_DYNAMIC(CValidationDialog, CDialog)

CValidationDialog::CValidationDialog(CWnd* pParent /*=NULL*/)
	: CDialog(CValidationDialog::IDD, pParent)
	, code(_T(""))
{

}

CValidationDialog::~CValidationDialog()
{
}

void CValidationDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_STATIC_VALIDATE_DISPLAY, pic);
	DDX_Text(pDX, IDC_EDIT_CODE, code);
}


BEGIN_MESSAGE_MAP(CValidationDialog, CDialog)
END_MESSAGE_MAP()


// CValidationDialog message handlers
