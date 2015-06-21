#include "stdafx.h"
#include "DataMag.h"
#include "LabelNameDlg.h"

IMPLEMENT_DYNAMIC(CLabelNameDlg, CDialog)

CLabelNameDlg::CLabelNameDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CLabelNameDlg::IDD, pParent)
	, Op(CLabelNameDlg::New)
	, m_label_name(_T(""))
{
}

CLabelNameDlg::~CLabelNameDlg()
{
}

void CLabelNameDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_LABEL_NAME, m_label_name);
}

BEGIN_MESSAGE_MAP(CLabelNameDlg, CDialog)
	ON_WM_CREATE()
END_MESSAGE_MAP()

int CLabelNameDlg::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CDialog::OnCreate(lpCreateStruct) == -1)
		return -1;

	if (Op == CLabelNameDlg::Rename)
	{
		SetWindowText(_T("ÐÞ¸Ä±êÇ©"));
	}

	return 0;
}