#include "stdafx.h"
#include "DataMag.h"
#include "SettingDlg.h"
#include "ProjectTab.h"

IMPLEMENT_DYNAMIC(CProjectTab, CDialogEx)

CProjectTab::CProjectTab(CWnd* pParent /*=NULL*/)
	: CDialogEx(CProjectTab::IDD, pParent)
{
	m_project_list.SetListEvent(this);

	DirChangeLinster listener;
	listener = bind(&CProjectTab::OnCodeMagDirChange, this, std::placeholders::_1);
	theSetting.AddCodeMagDirChangeListener(listener);

	HICON hSearchIcon = (HICON)LoadImage(AfxGetInstanceHandle()
		, MAKEINTRESOURCE(IDI_SEARCH)
		, IMAGE_ICON, 0, 0, 0);
	m_search_edit.SetSearchIcon(hSearchIcon);
}

CProjectTab::~CProjectTab()
{
}

void CProjectTab::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_ITEM_INFO, m_item_text);
	DDX_Control(pDX, IDC_PROJECT_LIST, m_project_list);
	DDX_Control(pDX, IDC_PROJECT_SEARCH_EDIT, m_search_edit);
}

BEGIN_MESSAGE_MAP(CProjectTab, CDialogEx)
	ON_EN_CHANGE(IDC_PROJECT_SEARCH_EDIT, &CProjectTab::OnChangeProjectSearchEdit)
END_MESSAGE_MAP()

void CProjectTab::InitShellList()
{
	CString strFolder = theSetting.GetCodeMagDir();
	m_project_list.DisplayFolder(strFolder);
}

void CProjectTab::OnDoubleClick()
{
	m_project_list.DoDefaultDClick();
}

void CProjectTab::OnSelectChanged()
{
	POSITION pos = m_project_list.GetFirstSelectedItemPosition();
	int nItem = m_project_list.GetNextSelectedItem(pos);
	if (nItem >= 0)
	{
		CString strPath = m_project_list.GetItemPath(nItem);
		if (PathIsDirectory(strPath))
		{
			CString strFile = strPath + _T("\\√Ë ˆ.txt");
			CStdioFile file(strFile, CFile::modeReadWrite | CFile::typeText);

			UINT nSize = UINT(file.GetLength()) + 1;
			char* szText = strText.GetBuffer();

			if ((UINT)strText.GetLength() < nSize)
			{
				szText = strText.GetBufferSetLength(nSize);
			}

			memset(szText, 0, nSize);
			file.Read(szText, nSize);

			SetWindowTextA(m_item_text.GetSafeHwnd(), szText);
		}
	}
}

void CProjectTab::OnChangeProjectSearchEdit()
{
	CString strFilter;
	m_search_edit.GetWindowText(strFilter);
	m_project_list.SetFilterString(strFilter);
}

BOOL CProjectTab::PreTranslateMessage(MSG* pMsg)
{
	if (pMsg->message == WM_KEYDOWN)
	{
		switch((UINT)pMsg->wParam)
		{
		case VK_RETURN:
			{
				return TRUE;
			}
			break;
		default:
			break;
		}
	}

	return CDialogEx::PreTranslateMessage(pMsg);
}

BOOL CProjectTab::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	return TRUE;
}