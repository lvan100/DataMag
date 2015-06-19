#include "stdafx.h"
#include "SettingDlg.h"
#include "LabelListCtrl.h"

IMPLEMENT_DYNAMIC(CLabelListCtrl, CMFCShellListCtrl)

CLabelListCtrl::CLabelListCtrl()
	: m_pInfoCtrl(NULL)
{
}

CLabelListCtrl::~CLabelListCtrl()
{
}

BEGIN_MESSAGE_MAP(CLabelListCtrl, CMFCShellListCtrl)
	ON_WM_CREATE()
	ON_NOTIFY_REFLECT(NM_DBLCLK, &CLabelListCtrl::OnDblClk)
	ON_NOTIFY_REFLECT(LVN_ITEMCHANGED, &CLabelListCtrl::OnLvnItemchanged)
END_MESSAGE_MAP()

int CLabelListCtrl::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CMFCListCtrl::OnCreate(lpCreateStruct) == -1)
		return -1;

	InitLabelList();

	return 0;
}

void CLabelListCtrl::PreSubclassWindow()
{
	CMFCListCtrl::PreSubclassWindow();

	InitLabelList();
}

void CLabelListCtrl::InitLabelList()
{
	OnSetColumns();

	CString strFolder = theSetting.strMagFolder;
	strFolder += _T("\\");
	strFolder += LABEL_DIR;
	DisplayFolder(strFolder);
}

void CLabelListCtrl::OnSetColumns()
{
	CMFCShellListCtrl::OnSetColumns();

	int nColumnCount = GetHeaderCtrl().GetItemCount();
	for (int i = 0; i < nColumnCount - 1; i++)
	{
		DeleteColumn(1);
	}

	SetColumnWidth(0, LVSCW_AUTOSIZE_USEHEADER);
}

void CLabelListCtrl::OnLvnItemchanged(NMHDR *pNMHDR, LRESULT *pResult)
{
	int nItem = GetNextItem(-1, LVNI_FOCUSED);
	if (nItem != -1 && m_pInfoCtrl != NULL)
	{
		m_pInfoCtrl->DisplayFolder(GetItemPath(nItem));
	}

	*pResult = 0;
}

void CLabelListCtrl::OnDblClk(NMHDR* /*pNMHDR*/, LRESULT* pResult)
{
	*pResult = 0;
}