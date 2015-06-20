#include "stdafx.h"
#include "DataMag.h"
#include "DataMagDlg.h"
#include "SettingDlg.h"
#include "BookListDlg.h"
#include "LabelNameDlg.h"
#include "ProjectListDlg.h"

CDataMagDlg* theDataMagDlg = NULL;

CDataMagDlg::CDataMagDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CDataMagDlg::IDD, pParent)
{
	if (theDataMagDlg != NULL)
	{
		ASSERT(FALSE);
	}
	theDataMagDlg = this;

	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CDataMagDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LABEL_LIST, m_label_list);
	DDX_Control(pDX, IDC_LABEL_INFO_LIST, m_label_info);
	DDX_Control(pDX, IDC_ITEM_INFO, m_item_text);
	DDX_Control(pDX, IDC_LABEL_SEARCH_EDIT, m_label_search_edit);
	DDX_Control(pDX, IDC_INFO_SEARCH_EDIT, m_info_search_edit);
}

BEGIN_MESSAGE_MAP(CDataMagDlg, CDialogEx)
	ON_BN_CLICKED(IDC_LABEL_ADD, &CDataMagDlg::OnBnClickedLabelAdd)
	ON_BN_CLICKED(IDC_LABEL_DELETE, &CDataMagDlg::OnBnClickedLabelDelete)
	ON_BN_CLICKED(IDC_LABEL_RENAME, &CDataMagDlg::OnBnClickedLabelRename)
	ON_BN_CLICKED(IDC_LABEL_RELATE_BOOK, &CDataMagDlg::OnBnClickedLabelRelateBook)
	ON_BN_CLICKED(IDC_LABEL_RELATE_PROJECT, &CDataMagDlg::OnBnClickedLabelRelateProject)
	ON_BN_CLICKED(IDC_SETTING, &CDataMagDlg::OnBnClickedSetting)
	ON_BN_CLICKED(IDC_LABEL_SEARCH_BUTTON, &CDataMagDlg::OnBnClickedLabelSearchButton)
	ON_BN_CLICKED(IDC_INFO_SEARCH_BUTTON, &CDataMagDlg::OnBnClickedInfoSearchButton)
END_MESSAGE_MAP()

BOOL CDataMagDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	SetIcon(m_hIcon, TRUE);
	SetIcon(m_hIcon, FALSE);
	
	CButton* pButton = (CButton*)GetDlgItem(IDC_SETTING);
	pButton->SetIcon(AfxGetApp()->LoadIcon(IDI_SETTING));

	pButton = (CButton*)GetDlgItem(IDC_LABEL_ADD);
	pButton->SetIcon(AfxGetApp()->LoadIcon(IDI_LABEL_ADD));

	pButton = (CButton*)GetDlgItem(IDC_LABEL_DELETE);
	pButton->SetIcon(AfxGetApp()->LoadIcon(IDI_LABEL_DELETE));

	pButton = (CButton*)GetDlgItem(IDC_LABEL_RENAME);
	pButton->SetIcon(AfxGetApp()->LoadIcon(IDI_LABEL_RENAME));

	pButton = (CButton*)GetDlgItem(IDC_LABEL_RELATE_PROJECT);
	pButton->SetIcon(AfxGetApp()->LoadIcon(IDI_CODE));

	pButton = (CButton*)GetDlgItem(IDC_LABEL_RELATE_BOOK);
	pButton->SetIcon(AfxGetApp()->LoadIcon(IDI_BOOK));

	pButton = (CButton*)GetDlgItem(IDC_LABEL_SEARCH_BUTTON);
	pButton->SetIcon(AfxGetApp()->LoadIcon(IDI_SEARCH));

	pButton = (CButton*)GetDlgItem(IDC_INFO_SEARCH_BUTTON);
	pButton->SetIcon(AfxGetApp()->LoadIcon(IDI_SEARCH));

	m_label_list.SetInfoCtrl(&m_label_info);

	return TRUE;
}

void CDataMagDlg::OnBnClickedLabelAdd()
{
	CLabelNameDlg dlg;
	if (dlg.DoModal() == IDOK)
	{
		CString strFolder = theSetting.strMagFolder;
		strFolder += _T("\\");
		strFolder += LABEL_DIR;
		strFolder += _T("\\");
		strFolder += dlg.m_label_name;

		CreateDirectory(strFolder, NULL);
	}

	m_label_list.Refresh();
}

void DeleteDirectory(CString strDir)
{
	CFileFind fileFind;

	BOOL IsFinded = fileFind.FindFile(strDir + _T("\\*.*"));
	IsFinded = fileFind.FindNextFile();	// .
	IsFinded = fileFind.FindNextFile();	// ..

	while (IsFinded)
	{
		IsFinded = fileFind.FindNextFile();
		DeleteFile(fileFind.GetFilePath());
	}

	fileFind.Close();

	RemoveDirectory(strDir);
}

void CDataMagDlg::OnBnClickedLabelDelete()
{
	POSITION pos = m_label_list.GetFirstSelectedItemPosition();
	int nItem = m_label_list.GetNextSelectedItem(pos);

	if (nItem >= 0)
	{	
		CString strFolder = m_label_list.GetItemPath(nItem);
		DeleteDirectory(strFolder);
	}

	m_label_list.Refresh();
}

void CDataMagDlg::OnBnClickedLabelRename()
{
	POSITION pos = m_label_list.GetFirstSelectedItemPosition();
	int nItem = m_label_list.GetNextSelectedItem(pos);

	if (nItem >= 0)
	{
		CString strOldFolder = m_label_list.GetItemPath(nItem);

		int nFind = strOldFolder.ReverseFind('\\') + 1;
		CString strLabel = strOldFolder.Mid(nFind);

		CLabelNameDlg dlg;
		dlg.m_label_name = strLabel;
		dlg.Op = CLabelNameDlg::Rename;

		if (dlg.DoModal() == IDOK)
		{
			CString strNewFolder = theSetting.strMagFolder;
			strNewFolder += _T("\\");
			strNewFolder += LABEL_DIR;
			strNewFolder += _T("\\");
			strNewFolder += dlg.m_label_name;

			CFile::Rename(strOldFolder, strNewFolder);
		}
	}

	m_label_list.Refresh();
}

BOOL CreateFileLink(CString strPath, CString strLink)
{
	HRESULT hResult = S_FALSE;

	IShellLink* pShellLink = NULL;
	AutoRelease<IShellLink*> tmp1(pShellLink);

	IPersistFile* pPersistFile = NULL;
	AutoRelease<IPersistFile*> tmp2(pPersistFile);

	hResult = CoCreateInstance(CLSID_ShellLink
		, NULL
		, CLSCTX_INPROC_SERVER
		, IID_IShellLink
		, (void**)&pShellLink);  
	if(FAILED(hResult))
	{
		return FALSE;
	}

	pShellLink->SetPath(strPath);

	hResult = pShellLink->QueryInterface(IID_IPersistFile
		, (void**)&pPersistFile);
	if(FAILED(hResult))
	{
		return FALSE;
	}

	hResult = pPersistFile->Save(strLink, STGM_DIRECT);

	return TRUE;   
}

void CDataMagDlg::OnBnClickedLabelRelateProject()
{
	POSITION pos = m_label_list.GetFirstSelectedItemPosition();
	int nItem = m_label_list.GetNextSelectedItem(pos);
	if (nItem >= 0)
	{
		CProjectListDlg dlg;
		if (dlg.DoModal() == IDOK)
		{
			for (int i = 0; i < dlg.arrProject.GetCount(); i++)
			{
				CString strFolder = dlg.arrProject.GetAt(i);
				int nFind = strFolder.ReverseFind('\\') + 1;
				CString strLabel = strFolder.Mid(nFind);

				CString strLink = m_label_list.GetItemPath(nItem);
				strLink += _T("\\") + strLabel + _T(".lnk");
				CreateFileLink(strFolder, strLink);
			}
		}

		m_label_info.Refresh();
	}
}

void CDataMagDlg::OnBnClickedLabelRelateBook()
{
	POSITION pos = m_label_list.GetFirstSelectedItemPosition();
	int nItem = m_label_list.GetNextSelectedItem(pos);
	if (nItem >= 0)
	{
		CBookListDlg dlg;
		if (dlg.DoModal() == IDOK)
		{
			for (int i = 0; i < dlg.arrBook.GetCount(); i++)
			{
				CString strFolder = dlg.arrBook.GetAt(i);
				int nFind = strFolder.ReverseFind('\\') + 1;
				CString strLabel = strFolder.Mid(nFind);

				CString strLink = m_label_list.GetItemPath(nItem);
				strLink += _T("\\") + strLabel + _T(".lnk");
				CreateFileLink(strFolder, strLink);
			}
		}

		m_label_info.Refresh();
	}
}

void CDataMagDlg::OnBnClickedSetting()
{
	CSettingDlg dlg;
	if (dlg.DoModal() == IDOK)
	{
		theSetting.Save();
	}

	CString strFolder = theSetting.strMagFolder;
	strFolder += _T("\\");
	strFolder += LABEL_DIR;
	m_label_list.DisplayFolder(strFolder);
}

BOOL CDataMagDlg::PreTranslateMessage(MSG* pMsg)
{
	if (pMsg->message == WM_KEYDOWN)
	{
		CWnd* pFocusWnd = GetFocus();

		switch((UINT)pMsg->wParam)
		{
		case VK_DELETE:
			{
				if (pFocusWnd == &m_label_info)
				{
					POSITION pos = m_label_info.GetFirstSelectedItemPosition();
					int nItem = m_label_info.GetNextSelectedItem(pos);
					if (nItem >= 0)
					{
						DeleteFile(m_label_info.GetItemPath(nItem));
					}

					m_label_info.Refresh();
				}
				
				if (pFocusWnd == &m_label_list)
				{
					OnBnClickedLabelDelete();
				}
			}
			break;
		default:
			break;
		}
	}

	return CDialogEx::PreTranslateMessage(pMsg);
}

void CDataMagDlg::OnBnClickedLabelSearchButton()
{
	CString strFilter;
	m_label_search_edit.GetWindowText(strFilter);
	m_label_list.SetFilterString(strFilter);
}

void CDataMagDlg::OnBnClickedInfoSearchButton()
{
	CString strFilter;
	m_info_search_edit.GetWindowText(strFilter);
	m_label_info.SetFilterString(strFilter);
}
