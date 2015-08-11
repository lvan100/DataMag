#include "stdafx.h"
#include "DataMag.h"
#include "SettingDlg.h"

IMPLEMENT_DYNAMIC(CSettingDlg, CDialog)

CSettingDlg::CSettingDlg(CWnd* pParent /*=nullptr*/)
	: CDialog(CSettingDlg::IDD, pParent)
{
}

CSettingDlg::~CSettingDlg()
{
}

void CSettingDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_TAG_BROWSER, m_tag_browser);
	DDX_Control(pDX, IDC_BOOK_BROWSER, m_book_browser);
	DDX_Control(pDX, IDC_CODE_BROWSER, m_project_browser);
}

BEGIN_MESSAGE_MAP(CSettingDlg, CDialog)
	ON_BN_CLICKED(IDOK, &CSettingDlg::OnBnClickedOk)
END_MESSAGE_MAP()

BOOL CSettingDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	m_book_browser.EnableFolderBrowseButton();
	m_book_browser.SetWindowText(theApp.GetBookDir());

	m_tag_browser.EnableFolderBrowseButton();
	m_tag_browser.SetWindowText(theApp.GetTagDir());

	m_project_browser.EnableFolderBrowseButton();
	m_project_browser.SetWindowText(theApp.GetCodeDir());
	
	return TRUE;
}

void CSettingDlg::OnBnClickedOk()
{
	CString strBookDir;
	m_book_browser.GetWindowText(strBookDir);
	theApp.SetBookDir(strBookDir);

	CString strTagDir;
	m_tag_browser.GetWindowText(strTagDir);
	theApp.SetTagDir(strTagDir);

	CString strProjectDir;
	m_project_browser.GetWindowText(strProjectDir);
	theApp.SetCodeDir(strProjectDir);
	
	CDialog::OnOK();
}