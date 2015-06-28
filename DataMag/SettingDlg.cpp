#include "stdafx.h"
#include "DataMag.h"
#include "SettingDlg.h"

/**
 * 全局配置对象
 */
CSetting theSetting;

void CSetting::Load(CString strFile)
{
	TCHAR szString[512];

	strIniFile = strFile;
	
	GetPrivateProfileString(_T("GlobalSetting"), _T("RootDir")
		, _T(""), szString, 512, strFile);
	strMagFolder.SetString(szString);
}

void CSetting::Save()
{
	WritePrivateProfileString(_T("GlobalSetting"), _T("RootDir")
		, strMagFolder, strIniFile);
}

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
	DDX_Control(pDX, IDC_MFCEDITBROWSE1, m_folder_select);
}

BEGIN_MESSAGE_MAP(CSettingDlg, CDialog)
	ON_BN_CLICKED(IDOK, &CSettingDlg::OnBnClickedOk)
END_MESSAGE_MAP()

BOOL CSettingDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	m_folder_select.EnableFolderBrowseButton();
	m_folder_select.SetWindowText(theSetting.strMagFolder);
	
	return TRUE;
}

void CSettingDlg::OnBnClickedOk()
{
	m_folder_select.GetWindowText(theSetting.strMagFolder);
	
	CDialog::OnOK();
}