#include "stdafx.h"
#include "DataMag.h"
#include "SettingDlg.h"
#include "ProjectListDlg.h"

IMPLEMENT_DYNAMIC(CProjectListDlg, CDialog)

CProjectListDlg::CProjectListDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CProjectListDlg::IDD, pParent)
{
	m_project_list.SetListEvent(this);
}

CProjectListDlg::~CProjectListDlg()
{
}

void CProjectListDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_PROJECT_LIST, m_project_list);
	DDX_Control(pDX, IDC_PROJECT_SEARCH_EDIT, m_project_search_edit);
}

BEGIN_MESSAGE_MAP(CProjectListDlg, CDialog)
	ON_BN_CLICKED(IDOK, &CProjectListDlg::OnBnClickedOk)
	ON_BN_CLICKED(IDC_PROJECT_SEARCH_BUTTON, &CProjectListDlg::OnBnClickedProjectSearchButton)
END_MESSAGE_MAP()

void CProjectListDlg::InitShellList()
{
	CString strFolder = theSetting.strMagFolder;
	strFolder += _T("\\");
	strFolder += PROJECT_DIR;
	m_project_list.DisplayFolder(strFolder);
}

BOOL CProjectListDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	auto pButton = (CButton*)GetDlgItem(IDC_PROJECT_SEARCH_BUTTON);
	pButton->SetIcon(AfxGetApp()->LoadIcon(IDI_SEARCH));

	return TRUE;
}

void CProjectListDlg::OnBnClickedProjectSearchButton()
{
	CString strFilter;
	m_project_search_edit.GetWindowText(strFilter);
	m_project_list.SetFilterString(strFilter);
}

void CProjectListDlg::OnBnClickedOk()
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

void CProjectListDlg::OnDoubleClick()
{
	OnBnClickedOk();
}