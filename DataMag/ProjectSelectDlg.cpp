#include "stdafx.h"
#include "DataMag.h"
#include "SettingDlg.h"
#include "ProjectSelectDlg.h"

IMPLEMENT_DYNAMIC(CProjectSelectDlg, CDialog)

CProjectSelectDlg::CProjectSelectDlg(CWnd* pParent /*=nullptr*/)
	: CDialog(CProjectSelectDlg::IDD, pParent)
{
	m_project_list.SetListEvent(this);
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

void CProjectSelectDlg::InitShellList()
{
	CString strFolder = theSetting.GetCodeMagDir();
	m_project_list.DisplayFolder(strFolder);
}

void CProjectSelectDlg::OnDoubleClick()
{
	OnBnClickedOk();
}

void CProjectSelectDlg::OnBnClickedOk()
{
	POSITION pos = m_project_list.GetFirstSelectedItemPosition();
	int nItem = m_project_list.GetNextSelectedItem(pos);
	while (nItem != -1)
	{
		CString strFolder =	m_project_list.GetItemPath(nItem);
		arrProject.Add(strFolder);
		nItem = m_project_list.GetNextSelectedItem(pos);
	}

	CDialog::OnOK();
}

void CProjectSelectDlg::OnChangeProjectSearchEdit()
{
	CString strFilter;
	m_search_edit.GetWindowText(strFilter);
	m_project_list.SetFilterString(strFilter);
}