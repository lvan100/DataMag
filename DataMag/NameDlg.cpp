#include "stdafx.h"
#include "DataMag.h"
#include "NameDlg.h"

IMPLEMENT_DYNAMIC(CNameDlg, CDialogEx)

CNameDlg::CNameDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(CNameDlg::IDD, pParent)
	, m_name(_T(""))
{
}

CNameDlg::~CNameDlg()
{
}

void CNameDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_TAG_NAME, m_name);
	DDX_Control(pDX, IDC_TAG_NAME, m_name_edit);
}

BEGIN_MESSAGE_MAP(CNameDlg, CDialogEx)
	ON_WM_CREATE()
END_MESSAGE_MAP()

int CNameDlg::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CDialogEx::OnCreate(lpCreateStruct) == -1)
		return -1;

	SetWindowText(m_title);

	return 0;
}