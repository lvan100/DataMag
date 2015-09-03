#include "stdafx.h"
#include "DataMag.h"
#include "SettingDlg.h"
#include "CodeDlg.h"

IMPLEMENT_DYNAMIC(CCodeDlg, CDialogEx)

CCodeDlg::CCodeDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(CCodeDlg::IDD, pParent)
	, m_project_list(&theShellManager)
{
	m_project_list.SetListBoxEvent(this);
	m_search_edit.SetSearchIcon(theApp.GetSearchIcon());
}

CCodeDlg::~CCodeDlg()
{
}

void CCodeDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_CODE_LIST, m_project_list);
	DDX_Control(pDX, IDC_CODE_SEARCH_EDIT, m_search_edit);
}

BEGIN_MESSAGE_MAP(CCodeDlg, CDialogEx)
	ON_BN_CLICKED(IDOK, &CCodeDlg::OnBnClickedOk)
	ON_EN_CHANGE(IDC_CODE_SEARCH_EDIT, &CCodeDlg::OnChangeProjectSearchEdit)
END_MESSAGE_MAP()

void CCodeDlg::InitListBox()
{
	CString strFolder = theApp.GetCodeDir();
	m_project_list.DisplayFolder(strFolder);
}

void CCodeDlg::OnDoubleClick()
{
	OnBnClickedOk();
}

void CCodeDlg::OnBnClickedOk()
{
	int nSelCount = m_project_list.GetSelCount();

	CArray<int,int> aryListBoxSel;
	aryListBoxSel.SetSize(nSelCount);
	m_project_list.GetSelItems(nSelCount, aryListBoxSel.GetData());

	for (int i = 0; i < nSelCount; i++) {
		int nSel = aryListBoxSel.GetAt(i);
		arrProject.Add(m_project_list.GetItemPath(nSel));
	}

	CDialogEx::OnOK();
}

void CCodeDlg::OnChangeProjectSearchEdit()
{
	CString strFilter;
	m_search_edit.GetWindowText(strFilter);
	m_project_list.SetFilterString(strFilter);
}