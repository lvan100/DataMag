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
	DDX_Control(pDX, IDC_BOOK_BROWSER, m_book_browser);
	DDX_Control(pDX, IDC_LABEL_BROWSER, m_label_browser);
	DDX_Control(pDX, IDC_PROJECT_BROWSER, m_project_browser);
}

BEGIN_MESSAGE_MAP(CSettingDlg, CDialog)
	ON_BN_CLICKED(IDOK, &CSettingDlg::OnBnClickedOk)
END_MESSAGE_MAP()

BOOL CSettingDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	m_book_browser.EnableFolderBrowseButton();
	m_book_browser.SetWindowText(theSetting.GetBookMagDir());

	m_label_browser.EnableFolderBrowseButton();
	m_label_browser.SetWindowText(theSetting.GetLabelMagDir());

	m_project_browser.EnableFolderBrowseButton();
	m_project_browser.SetWindowText(theSetting.GetCodeMagDir());
	
	return TRUE;
}

void CSettingDlg::OnBnClickedOk()
{
	CString strBookDir;
	m_book_browser.GetWindowText(strBookDir);
	theSetting.SetBookMagDir(strBookDir);

	CString strLabelDir;
	m_label_browser.GetWindowText(strLabelDir);
	theSetting.SetLabelMagDir(strLabelDir);

	CString strProjectDir;
	m_project_browser.GetWindowText(strProjectDir);
	theSetting.SetCodeMagDir(strProjectDir);
	
	CDialog::OnOK();
}