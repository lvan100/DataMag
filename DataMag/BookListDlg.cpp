#include "stdafx.h"
#include "DataMag.h"
#include "BookListDlg.h"

IMPLEMENT_DYNAMIC(CBookListDlg, CDialog)

CBookListDlg::CBookListDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CBookListDlg::IDD, pParent)
{
}

CBookListDlg::~CBookListDlg()
{
}

void CBookListDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_BOOK_LIST, m_book_list);
}

BEGIN_MESSAGE_MAP(CBookListDlg, CDialog)
	ON_BN_CLICKED(IDOK, &CBookListDlg::OnBnClickedOk)
END_MESSAGE_MAP()

void CBookListDlg::OnBnClickedOk()
{
	POSITION pos = m_book_list.GetFirstSelectedItemPosition();
	int nItem = m_book_list.GetNextSelectedItem(pos);
	while (nItem != -1) {
		CString strFolder;
		m_book_list.GetItemPath(strFolder, nItem);
		arrBook.Add(strFolder);
		nItem = m_book_list.GetNextSelectedItem(pos);
	}
	
	CDialog::OnOK();
}
