#include "stdafx.h"
#include "DataMag.h"
#include "SettingDlg.h"
#include "BookSelectDlg.h"

IMPLEMENT_DYNAMIC(CBookSelectDlg, CDialog)

CBookSelectDlg::CBookSelectDlg(CWnd* pParent /*=nullptr*/)
	: CDialog(CBookSelectDlg::IDD, pParent)
{
	m_book_list.SetListEvent(this);
}

CBookSelectDlg::~CBookSelectDlg()
{
}

void CBookSelectDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_BOOK_LIST, m_book_list);
	DDX_Control(pDX, IDC_BOOK_SEARCH_EDIT, m_book_search_edit);
}

BEGIN_MESSAGE_MAP(CBookSelectDlg, CDialog)
	ON_BN_CLICKED(IDOK, &CBookSelectDlg::OnBnClickedOk)
	ON_EN_CHANGE(IDC_BOOK_SEARCH_EDIT, &CBookSelectDlg::OnChangeBookSearchEdit)
END_MESSAGE_MAP()

void CBookSelectDlg::InitShellList()
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
	POSITION pos = m_book_list.GetFirstSelectedItemPosition();
	int nItem = m_book_list.GetNextSelectedItem(pos);
	while (nItem != -1) {
		CString strFolder =	m_book_list.GetItemPath(nItem);
		arrBook.Add(strFolder);
		nItem = m_book_list.GetNextSelectedItem(pos);
	}
	
	CDialog::OnOK();
}

void CBookSelectDlg::OnChangeBookSearchEdit()
{
	CString strFilter;
	m_book_search_edit.GetWindowText(strFilter);
	m_book_list.SetFilterString(strFilter);
}