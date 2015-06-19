#include "stdafx.h"
#include "DataMag.h"
#include "ProjectListDlg.h"

IMPLEMENT_DYNAMIC(CProjectListDlg, CDialog)

CProjectListDlg::CProjectListDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CProjectListDlg::IDD, pParent)
{

}

CProjectListDlg::~CProjectListDlg()
{
}

void CProjectListDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_PROJECT_LIST, m_project_list);
}

BEGIN_MESSAGE_MAP(CProjectListDlg, CDialog)
	ON_BN_CLICKED(IDOK, &CProjectListDlg::OnBnClickedOk)
END_MESSAGE_MAP()

void CProjectListDlg::OnBnClickedOk()
{
	POSITION pos = m_project_list.GetFirstSelectedItemPosition();
	int nItem = m_project_list.GetNextSelectedItem(pos);
	while (nItem != -1)
	{
		CString strFolder;
		m_project_list.GetItemPath(strFolder, nItem);
		arrProject.Add(strFolder);
		nItem = m_project_list.GetNextSelectedItem(pos);
	}

	CDialog::OnOK();
}
