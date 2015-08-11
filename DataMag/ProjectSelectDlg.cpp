#include "stdafx.h"
#include "DataMag.h"
#include "SettingDlg.h"
#include "ProjectSelectDlg.h"

IMPLEMENT_DYNAMIC(CProjectSelectDlg, CDialog)

CProjectSelectDlg::CProjectSelectDlg(CWnd* pParent /*=nullptr*/)
	: CDialog(CProjectSelectDlg::IDD, pParent)
	, m_project_list(&theShellManager)
{
	m_project_list.SetListEvent(this);

	HICON hSearchIcon = (HICON)LoadImage(AfxGetInstanceHandle()
		, MAKEINTRESOURCE(IDI_SEARCH)
		, IMAGE_ICON, 0, 0, 0);
	m_search_edit.SetSearchIcon(hSearchIcon);
}

CProjectSelectDlg::~CProjectSelectDlg()
{
}

void CProjectSelectDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_PROJECT_LIST, m_project_list);
	DDX_Control(pDX, IDC_PROJECT_SEARCH_EDIT, m_search_edit);
}

BEGIN_MESSAGE_MAP(CProjectSelectDlg, CDialog)
	ON_BN_CLICKED(IDOK, &CProjectSelectDlg::OnBnClickedOk)
	ON_EN_CHANGE(IDC_PROJECT_SEARCH_EDIT, &CProjectSelectDlg::OnChangeProjectSearchEdit)
END_MESSAGE_MAP()

void CProjectSelectDlg::InitListBox()
{
	CString strFolder = theApp.GetCodeMagDir();
	m_project_list.DisplayFolder(strFolder);
}

void CProjectSelectDlg::OnDoubleClick()
{
	OnBnClickedOk();
}

void CProjectSelectDlg::OnBnClickedOk()
{
	int nSelCount = m_project_list.GetSelCount();

	CArray<int,int> aryListBoxSel;
	aryListBoxSel.SetSize(nSelCount);
	m_project_list.GetSelItems(nSelCount, aryListBoxSel.GetData());

	for (int i = 0; i < nSelCount; i++) {
		int nSel = aryListBoxSel.GetAt(i);
		arrProject.Add(m_project_list.GetItemPath(nSel));
	}

	CDialog::OnOK();
}

void CProjectSelectDlg::OnChangeProjectSearchEdit()
{
	CString strFilter;
	m_search_edit.GetWindowText(strFilter);
	m_project_list.SetFilterString(strFilter);
}