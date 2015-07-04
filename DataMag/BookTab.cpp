#include "stdafx.h"
#include "DataMag.h"
#include "BookTab.h"
#include "SettingDlg.h"

IMPLEMENT_DYNAMIC(CBookTab, CDialogEx)

CBookTab::CBookTab(CWnd* pParent /*=NULL*/)
	: CDialogEx(CBookTab::IDD, pParent)
{
	m_book_list.SetListEvent(this);

	DirChangeLinster listener;
	listener = bind(&CBookTab::OnBookMagDirChange, this, std::placeholders::_1);
	theSetting.AddBookMagDirChangeListener(listener);

	HICON hSearchIcon = (HICON)LoadImage(AfxGetInstanceHandle()
		, MAKEINTRESOURCE(IDI_SEARCH)
		, IMAGE_ICON, 0, 0, 0);
	m_search_edit.SetSearchIcon(hSearchIcon);
}

CBookTab::~CBookTab()
{
}

void CBookTab::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_ITEM_INFO, m_item_text);
	DDX_Control(pDX, IDC_BOOK_LIST, m_book_list);
	DDX_Control(pDX, IDC_BOOK_SEARCH_EDIT, m_search_edit);
}

BEGIN_MESSAGE_MAP(CBookTab, CDialogEx)
	ON_EN_CHANGE(IDC_BOOK_SEARCH_EDIT, &CBookTab::OnChangeBookSearchEdit)
END_MESSAGE_MAP()

void CBookTab::InitShellList()
{
	CString strFolder = theSetting.GetBookMagDir();
	m_book_list.DisplayFolder(strFolder);
}

void CBookTab::OnDoubleClick()
{
	m_book_list.DoDefaultDClick();
}

void CBookTab::OnSelectChanged()
{
	POSITION pos = m_book_list.GetFirstSelectedItemPosition();
	int nItem = m_book_list.GetNextSelectedItem(pos);
	if (nItem >= 0)
	{
		CString strPath = m_book_list.GetItemPath(nItem);
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

void CBookTab::OnChangeBookSearchEdit()
{
	CString strFilter;
	m_search_edit.GetWindowText(strFilter);
	m_book_list.SetFilterString(strFilter);
}

BOOL CBookTab::PreTranslateMessage(MSG* pMsg)
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