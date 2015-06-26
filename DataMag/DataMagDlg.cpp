#include "stdafx.h"
#include "DataMag.h"
#include "DataMagDlg.h"
#include "SettingDlg.h"
#include "BookListDlg.h"
#include "LabelNameDlg.h"
#include "ProjectListDlg.h"

/**
 * 全局的主对话框对象
 */
CDataMagDlg* theDataMagDlg = NULL;

CDataMagDlg::CDataMagDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CDataMagDlg::IDD, pParent)
	, m_bExpand(FALSE)
{
	if (theDataMagDlg != NULL)
	{
		ASSERT(FALSE);
	}
	theDataMagDlg = this;

	m_label_list.SetListEvent(&m_label_event);
	m_label_info.SetListEvent(&m_label_info_event);

	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CDataMagDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_ITEM_INFO, m_item_text);
	DDX_Control(pDX, IDC_LABEL_LIST, m_label_list);
	DDX_Control(pDX, IDC_LABEL_INFO_LIST, m_label_info);
	DDX_Control(pDX, IDC_INFO_SEARCH_EDIT, m_info_search_edit);
	DDX_Control(pDX, IDC_LABEL_SEARCH_EDIT, m_label_search_edit);
}

BEGIN_MESSAGE_MAP(CDataMagDlg, CDialogEx)
	ON_BN_CLICKED(IDC_SETTING, &CDataMagDlg::OnBnClickedSetting)
	ON_BN_CLICKED(IDC_LABEL_ADD, &CDataMagDlg::OnBnClickedLabelAdd)
	ON_BN_CLICKED(IDC_LABEL_DELETE, &CDataMagDlg::OnBnClickedLabelDelete)
	ON_BN_CLICKED(IDC_LABEL_RENAME, &CDataMagDlg::OnBnClickedLabelRename)
	ON_BN_CLICKED(IDC_LABEL_RELATE_BOOK, &CDataMagDlg::OnBnClickedLabelRelateBook)
	ON_BN_CLICKED(IDC_LABEL_RELATE_PROJECT, &CDataMagDlg::OnBnClickedLabelRelateProject)
	ON_EN_CHANGE(IDC_LABEL_SEARCH_EDIT, &CDataMagDlg::OnChangeLabelSearchEdit)
	ON_EN_CHANGE(IDC_INFO_SEARCH_EDIT, &CDataMagDlg::OnChangeInfoSearchEdit)
	ON_BN_CLICKED(IDC_EXPAND_BUTTON, &CDataMagDlg::OnBnClickedExpandButton)
END_MESSAGE_MAP()

void CDataMagDlg::LabelListEvent::InitShellList()
{
	CString strFolder = theSetting.strMagFolder;
	strFolder += _T("\\");
	strFolder += LABEL_DIR;
	theDataMagDlg->m_label_list.DisplayFolder(strFolder);
}

void CDataMagDlg::LabelListEvent::OnSelectChanged()
{
	POSITION pos = theDataMagDlg->m_label_list.GetFirstSelectedItemPosition();
	int nItem = theDataMagDlg->m_label_list.GetNextSelectedItem(pos);
	if (nItem >= 0)
	{
		CString strPath = theDataMagDlg->m_label_list.GetItemPath(nItem);
		theDataMagDlg->m_label_info.DisplayFolder(strPath);
	}
}

BOOL GetLinkFilePath(CString& strPath, CString strLink)
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

	hResult = pShellLink->Resolve(NULL, SLR_ANY_MATCH);
	if(FAILED(hResult))
	{
		return FALSE;
	}

	WCHAR szPath[MAX_PATH];

	hResult = pShellLink->GetPath(szPath, MAX_PATH, NULL, SLGP_SHORTPATH); 
	if(FAILED(hResult))
	{
		return FALSE;
	}

	strPath.SetString(szPath);

	return TRUE; 
}

void CDataMagDlg::LabelInfoEvent::InitShellList()
{
	theDataMagDlg->m_label_info.DeleteAllItems();
}

void CDataMagDlg::LabelInfoEvent::OnSelectChanged()
{
	SetWindowTextA(theDataMagDlg->m_item_text.GetSafeHwnd(), "");

	POSITION pos = theDataMagDlg->m_label_info.GetFirstSelectedItemPosition();
	int nItem = theDataMagDlg->m_label_info.GetNextSelectedItem(pos);
	if (nItem >= 0)
	{
		CString strPath;
		CString strLink = theDataMagDlg->m_label_info.GetItemPath(nItem);
		if (GetLinkFilePath(strPath, strLink))
		{
			if (PathIsDirectory(strPath))
			{
				CString strFile = strPath + _T("\\描述.txt");
				CStdioFile file(strFile, CFile::modeReadWrite | CFile::typeText);

				UINT nSize = file.GetLength() + 1;
				char* szText = strText.GetBuffer();

				if ((UINT)strText.GetLength() < nSize)
				{
					szText = strText.GetBufferSetLength(nSize);
				}

				memset(szText, 0, nSize);
				file.Read(szText, nSize);

				SetWindowTextA(theDataMagDlg->m_item_text.GetSafeHwnd(), szText);
			}
		}
	}
}

void CDataMagDlg::LabelInfoEvent::OnDoubleClick()
{
	theDataMagDlg->m_label_info.DoDefaultDClick();
}

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

	CRect rcRichEdit;
	GetDlgItem(IDC_ITEM_INFO)->GetWindowRect(rcRichEdit);

	CRect rcWnd;
	GetWindowRect(rcWnd);

	sizeLarge= rcWnd.Size();
	
	sizeSmall = sizeLarge;
	sizeSmall.cx -= rcRichEdit.Width();
	sizeSmall.cx -= 7; /* 对话框内边框大小*/

	AdjustWndRect();

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
	else
	{
		CProjectListDlg dlg;
		dlg.Op = CProjectListDlg::Display;
		dlg.DoModal();
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
	else
	{
		CBookListDlg dlg;
		dlg.Op = CBookListDlg::Display;
		dlg.DoModal();
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

void CDataMagDlg::OnChangeLabelSearchEdit()
{
	CString strFilter;
	m_label_search_edit.GetWindowText(strFilter);
	m_label_list.SetFilterString(strFilter);
}

void CDataMagDlg::OnChangeInfoSearchEdit()
{
	CString strFilter;
	m_info_search_edit.GetWindowText(strFilter);
	m_label_info.SetFilterString(strFilter);
}

void CDataMagDlg::AdjustWndRect()
{
	CRect rcWnd;
	GetWindowRect(rcWnd);

	if (m_bExpand)
	{
		rcWnd.right = rcWnd.left + sizeLarge.cx;
		rcWnd.bottom = rcWnd.top + sizeLarge.cy;

		GetDlgItem(IDC_ITEM_INFO)->ShowWindow(SW_SHOW);
	}
	else
	{
		rcWnd.right = rcWnd.left + sizeSmall.cx;
		rcWnd.bottom = rcWnd.top + sizeSmall.cy;

		GetDlgItem(IDC_ITEM_INFO)->ShowWindow(SW_HIDE);
	}

	MoveWindow(rcWnd);

	CenterWindow();
}

void CDataMagDlg::OnBnClickedExpandButton()
{
	m_bExpand = !m_bExpand;

	AdjustWndRect();
}
