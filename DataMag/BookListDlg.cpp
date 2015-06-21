#include "stdafx.h"
#include "DataMag.h"
#include "SettingDlg.h"
#include "BookListDlg.h"

IMPLEMENT_DYNAMIC(CBookListDlg, CDialog)

CBookListDlg::CBookListDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CBookListDlg::IDD, pParent)
{
	m_book_list.SetListEvent(this);
}

CBookListDlg::~CBookListDlg()
{
}

void CBookListDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_BOOK_LIST, m_book_list);
	DDX_Control(pDX, IDC_BOOK_SEARCH_EDIT, m_book_search_edit);
}

BEGIN_MESSAGE_MAP(CBookListDlg, CDialog)
	ON_BN_CLICKED(IDOK, &CBookListDlg::OnBnClickedOk)
	ON_BN_CLICKED(IDC_BOOK_SEARCH_BUTTON, &CBookListDlg::OnBnClickedBookSearchButton)
END_MESSAGE_MAP()

void CBookListDlg::InitShellList()
{
	CString strFolder = theSetting.strMagFolder;
	strFolder += _T("\\");
	strFolder += BOOK_DIR;
	m_book_list.DisplayFolder(strFolder);
}

BOOL CBookListDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	auto pButton = (CButton*)GetDlgItem(IDC_BOOK_SEARCH_BUTTON);
	pButton->SetIcon(AfxGetApp()->LoadIcon(IDI_SEARCH));

	return TRUE;
}

void CBookListDlg::OnBnClickedBookSearchButton()
{
	CString strFilter;
	m_book_search_edit.GetWindowText(strFilter);
	m_book_list.SetFilterString(strFilter);
}

void CBookListDlg::OnBnClickedOk()
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

void CBookListDlg::OnDoubleClick()
{
	OnBnClickedOk();
}