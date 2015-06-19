#include "stdafx.h"
#include "DataMag.h"
#include "SettingDlg.h"
#include "BookListDlg.h"
#include "BookListCtrl.h"

IMPLEMENT_DYNAMIC(CBookListCtrl, CMFCShellListCtrl)

CBookListCtrl::CBookListCtrl()
{
}

CBookListCtrl::~CBookListCtrl()
{
}

BEGIN_MESSAGE_MAP(CBookListCtrl, CMFCShellListCtrl)
	ON_WM_CREATE()
	ON_NOTIFY_REFLECT(NM_DBLCLK, &CBookListCtrl::OnDblClk)
END_MESSAGE_MAP()

int CBookListCtrl::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CMFCListCtrl::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	InitBookList();

	return 0;
}

void CBookListCtrl::PreSubclassWindow()
{
	CMFCListCtrl::PreSubclassWindow();

	InitBookList();
}

void CBookListCtrl::InitBookList()
{
	OnSetColumns();

	CString strFolder = theSetting.strMagFolder;
	strFolder += _T("\\");
	strFolder += BOOK_DIR;
	DisplayFolder(strFolder);
}

void CBookListCtrl::OnSetColumns()
{
	CMFCShellListCtrl::OnSetColumns();

	int nColumnCount = GetHeaderCtrl().GetItemCount();
	for (int i = 0; i < nColumnCount - 1; i++)
	{
		DeleteColumn(1);
	}

	SetColumnWidth(0, LVSCW_AUTOSIZE_USEHEADER);
}

void CBookListCtrl::OnDblClk(NMHDR* /*pNMHDR*/, LRESULT* pResult)
{
	auto dlg = (CBookListDlg*)GetParent();
	dlg->SendMessage(WM_COMMAND, IDOK);

	*pResult = 0;
}