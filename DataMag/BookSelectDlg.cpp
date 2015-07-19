#include "stdafx.h"
#include "DataMag.h"
#include "SettingDlg.h"
#include "BookSelectDlg.h"

IMPLEMENT_DYNAMIC(CBookSelectDlg, CDialog)

CBookSelectDlg::CBookSelectDlg(CWnd* pParent /*=nullptr*/)
	: CDialog(CBookSelectDlg::IDD, pParent)
	, m_book_list(&theShellManager)
{
	m_book_list.SetListEvent(this);

	HICON hSearchIcon = (HICON)LoadImage(AfxGetInstanceHandle()
		, MAKEINTRESOURCE(IDI_SEARCH)
		, IMAGE_ICON, 0, 0, 0);
	m_search_edit.SetSearchIcon(hSearchIcon);
}

CBookSelectDlg::~CBookSelectDlg()
{
}

void CBookSelectDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_BOOK_LIST, m_book_list);
	DDX_Control(pDX, IDC_BOOK_SEARCH_EDIT, m_search_edit);
}

BEGIN_MESSAGE_MAP(CBookSelectDlg, CDialog)
	ON_BN_CLICKED(IDOK, &CBookSelectDlg::OnBnClickedOk)
	ON_EN_CHANGE(IDC_BOOK_SEARCH_EDIT, &CBookSelectDlg::OnChangeBookSearchEdit)
END_MESSAGE_MAP()

void CBookSelectDlg::InitListBox()
{
	CString strFolder = theSetting.GetBookMagDir();
	m_book_list.DisplayFolder(strFolder);
}

void CBookSelectDlg::OnDoubleClick()
{
	OnBnClickedOk();
}

void CBookSelectDlg::OnBnClickedOk()
{
	int nSelCount = m_book_list.GetSelCount();

	CArray<int,int> aryListBoxSel;
	aryListBoxSel.SetSize(nSelCount);
	m_book_list.GetSelItems(nSelCount, aryListBoxSel.GetData());

	for (int i = 0; i < nSelCount; i++) {
		int nSel = aryListBoxSel.GetAt(i);
		arrBook.Add(m_book_list.GetItemPath(nSel));
	}

	CDialog::OnOK();
}

void CBookSelectDlg::OnChangeBookSearchEdit()
{
	CString strFilter;
	m_search_edit.GetWindowText(strFilter);
	m_book_list.SetFilterString(strFilter);
}