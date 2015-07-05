#include "stdafx.h"
#include "DataMag.h"
#include "NameDlg.h"

IMPLEMENT_DYNAMIC(CNameDlg, CDialog)

CNameDlg::CNameDlg(CWnd* pParent /*=nullptr*/)
	: CDialog(CNameDlg::IDD, pParent)
	, Op(CNameDlg::New)
	, m_name(_T(""))
{
}

CNameDlg::~CNameDlg()
{
}

void CNameDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_LABEL_NAME, m_name);
}

BEGIN_MESSAGE_MAP(CNameDlg, CDialog)
	ON_WM_CREATE()
END_MESSAGE_MAP()

int CNameDlg::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CDialog::OnCreate(lpCreateStruct) == -1)
		return -1;

	if (Op == CNameDlg::Rename)
	{
		SetWindowText(_T("ÖØÃüÃû"));
	}

	return 0;
}