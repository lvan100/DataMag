#include "stdafx.h"
#include "DataMag.h"
#include "LabelTab.h"
#include "SettingDlg.h"
#include "LabelNameDlg.h"
#include "BookSelectDlg.h"
#include "ProjectSelectDlg.h"

IMPLEMENT_DYNAMIC(CLabelTab, CDialogEx)

CLabelTab::CLabelTab(CWnd* pParent /*=NULL*/)
	: CDialogEx(CLabelTab::IDD, pParent)
{	
	m_label_list.SetListEvent(&m_label_event);
	m_label_info.SetListEvent(&m_label_info_event);
}

CLabelTab::~CLabelTab()
{
}

void CLabelTab::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LABEL_LIST, m_label_list);
	DDX_Control(pDX, IDC_LABEL_INFO_LIST, m_label_info);
	DDX_Control(pDX, IDC_INFO_SEARCH_EDIT, m_info_search_edit);
	DDX_Control(pDX, IDC_LABEL_SEARCH_EDIT, m_label_search_edit);
}

BEGIN_MESSAGE_MAP(CLabelTab, CDialogEx)
	ON_BN_CLICKED(IDC_LABEL_ADD, &CLabelTab::OnBnClickedLabelAdd)
	ON_BN_CLICKED(IDC_LABEL_DELETE, &CLabelTab::OnBnClickedLabelDelete)
	ON_BN_CLICKED(IDC_LABEL_RENAME, &CLabelTab::OnBnClickedLabelRename)
	ON_EN_CHANGE(IDC_INFO_SEARCH_EDIT, &CLabelTab::OnChangeInfoSearchEdit)
	ON_EN_CHANGE(IDC_LABEL_SEARCH_EDIT, &CLabelTab::OnChangeLabelSearchEdit)
	ON_BN_CLICKED(IDC_LABEL_RELATE_BOOK, &CLabelTab::OnBnClickedLabelRelateBook)
	ON_BN_CLICKED(IDC_LABEL_RELATE_PROJECT, &CLabelTab::OnBnClickedLabelRelateProject)
END_MESSAGE_MAP()

void CLabelTab::LabelListEvent::InitShellList()
{
	auto pThis = ((CLabelTab*)((BYTE*)this - offsetof(CLabelTab, m_label_event)));

	CString strFolder = theSetting.strMagFolder;
	strFolder += _T("\\");
	strFolder += LABEL_DIR;

	pThis->m_label_list.DisplayFolder(strFolder);
}

void CLabelTab::LabelListEvent::OnSelectChanged()
{
	auto pThis = ((CLabelTab*)((BYTE*)this - offsetof(CLabelTab, m_label_event)));

	POSITION pos = pThis->m_label_list.GetFirstSelectedItemPosition();
	int nItem = pThis->m_label_list.GetNextSelectedItem(pos);
	if (nItem >= 0)
	{
		CString strPath = pThis->m_label_list.GetItemPath(nItem);
		pThis->m_label_info.DisplayFolder(strPath);
	}
}

static BOOL GetLinkFilePath(CString& strPath, CString strLink)
{
	HRESULT hResult = S_FALSE;

	IShellLink* pShellLink = nullptr;
	AutoRelease<IShellLink*> tmp1(pShellLink);

	IPersistFile* pPersistFile = nullptr;
	AutoRelease<IPersistFile*> tmp2(pPersistFile);

	hResult = CoCreateInstance(CLSID_ShellLink
		, nullptr
		, CLSCTX_INPROC_SERVER
		, IID_IShellLink
		, (void**)&pShellLink);  
	if(FAILED(hResult))
	{
		return FALSE;
	}

	hResult = pShellLink->QueryInterface(IID_IPersistFile
		, (void**)&pPersistFile);
	if(FAILED(hResult))
	{
		return FALSE;
	}

	hResult = pPersistFile->Load(strLink, STGM_READ);
	if(FAILED(hResult))
	{
		return FALSE;
	}

	hResult = pShellLink->Resolve(nullptr, SLR_ANY_MATCH);
	if(FAILED(hResult))
	{
		return FALSE;
	}

	WCHAR szPath[MAX_PATH];

	hResult = pShellLink->GetPath(szPath, MAX_PATH, nullptr, SLGP_SHORTPATH); 
	if(FAILED(hResult))
	{
		return FALSE;
	}

	strPath.SetString(szPath);

	return TRUE; 
}

void CLabelTab::LabelInfoEvent::InitShellList()
{
	auto pThis = ((CLabelTab*)((BYTE*)this - offsetof(CLabelTab, m_label_info_event)));
	pThis->m_label_info.DeleteAllItems();
}

void CLabelTab::LabelInfoEvent::OnDoubleClick()
{
	auto pThis = ((CLabelTab*)((BYTE*)this - offsetof(CLabelTab, m_label_info_event)));
	pThis->m_label_info.DoDefaultDClick();
}

BOOL CLabelTab::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	CButton* pButton = (CButton*)GetDlgItem(IDC_LABEL_ADD);
	pButton->SetIcon(AfxGetApp()->LoadIcon(IDI_LABEL_ADD));

	pButton = (CButton*)GetDlgItem(IDC_LABEL_DELETE);
	pButton->SetIcon(AfxGetApp()->LoadIcon(IDI_LABEL_DELETE));

	pButton = (CButton*)GetDlgItem(IDC_LABEL_RENAME);
	pButton->SetIcon(AfxGetApp()->LoadIcon(IDI_LABEL_RENAME));

	pButton = (CButton*)GetDlgItem(IDC_LABEL_RELATE_PROJECT);
	pButton->SetIcon(AfxGetApp()->LoadIcon(IDI_CODE));

	pButton = (CButton*)GetDlgItem(IDC_LABEL_RELATE_BOOK);
	pButton->SetIcon(AfxGetApp()->LoadIcon(IDI_BOOK));

	return TRUE;
}

void CLabelTab::OnBnClickedLabelAdd()
{
	CLabelNameDlg dlg;
	if (dlg.DoModal() == IDOK)
	{
		CString strFolder = theSetting.strMagFolder;
		strFolder += _T("\\");
		strFolder += LABEL_DIR;
		strFolder += _T("\\");
		strFolder += dlg.m_label_name;

		CreateDirectory(strFolder, nullptr);
	}

	m_label_list.Refresh();
}

static void DeleteDirectory(CString strDir)
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

void CLabelTab::OnBnClickedLabelDelete()
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

void CLabelTab::OnBnClickedLabelRename()
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

static BOOL CreateFileLink(CString strPath, CString strLink)
{
	HRESULT hResult = S_FALSE;

	IShellLink* pShellLink = nullptr;
	AutoRelease<IShellLink*> tmp1(pShellLink);

	IPersistFile* pPersistFile = nullptr;
	AutoRelease<IPersistFile*> tmp2(pPersistFile);

	hResult = CoCreateInstance(CLSID_ShellLink
		, nullptr
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

void CLabelTab::OnBnClickedLabelRelateProject()
{
	POSITION pos = m_label_list.GetFirstSelectedItemPosition();
	int nItem = m_label_list.GetNextSelectedItem(pos);
	if (nItem >= 0)
	{
		CProjectSelectDlg dlg;
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

void CLabelTab::OnBnClickedLabelRelateBook()
{
	POSITION pos = m_label_list.GetFirstSelectedItemPosition();
	int nItem = m_label_list.GetNextSelectedItem(pos);
	if (nItem >= 0)
	{
		CBookSelectDlg dlg;
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

void CLabelTab::OnChangeLabelSearchEdit()
{
	CString strFilter;
	m_label_search_edit.GetWindowText(strFilter);
	m_label_list.SetFilterString(strFilter);
}

void CLabelTab::OnChangeInfoSearchEdit()
{
	CString strFilter;
	m_info_search_edit.GetWindowText(strFilter);
	m_label_info.SetFilterString(strFilter);
}

BOOL CLabelTab::PreTranslateMessage(MSG* pMsg)
{
	if (pMsg->message == WM_KEYDOWN)
	{
		CWnd* pFocusWnd = GetFocus();

		switch((UINT)pMsg->wParam)
		{
		case VK_RETURN:
			{
				return TRUE;
			}
			break;
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