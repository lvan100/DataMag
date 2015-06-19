#include "stdafx.h"
#include "SettingDlg.h"
#include "ProjectListCtrl.h"

IMPLEMENT_DYNAMIC(CProjectListCtrl, CMFCShellListCtrl)

CProjectListCtrl::CProjectListCtrl()
{

}

CProjectListCtrl::~CProjectListCtrl()
{
}

BEGIN_MESSAGE_MAP(CProjectListCtrl, CMFCShellListCtrl)
	ON_WM_CREATE()
	ON_NOTIFY_REFLECT(NM_DBLCLK, &CProjectListCtrl::OnDblClk)
END_MESSAGE_MAP()

int CProjectListCtrl::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CMFCListCtrl::OnCreate(lpCreateStruct) == -1)
		return -1;

	InitProjectList();

	return 0;
}

void CProjectListCtrl::PreSubclassWindow()
{
	CMFCListCtrl::PreSubclassWindow();

	InitProjectList();
}

void CProjectListCtrl::InitProjectList()
{
	OnSetColumns();

	CString strFolder = theSetting.strMagFolder;
	strFolder += _T("\\");
	strFolder += PROJECT_DIR;
	DisplayFolder(strFolder);
}

void CProjectListCtrl::OnSetColumns()
{
	CMFCShellListCtrl::OnSetColumns();

	int nColumnCount = GetHeaderCtrl().GetItemCount();
	for (int i = 0; i < nColumnCount - 1; i++)
	{
		DeleteColumn(1);
	}

	SetColumnWidth(0, LVSCW_AUTOSIZE_USEHEADER);
}

void CProjectListCtrl::OnDblClk(NMHDR* /*pNMHDR*/, LRESULT* pResult)
{
	*pResult = 0;
}