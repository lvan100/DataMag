#include "stdafx.h"
#include "FileOp.h"
#include "DataMag.h"
#include "NameDlg.h"
#include "BookTab.h"
#include "SettingDlg.h"

IMPLEMENT_DYNAMIC(CBookTab, CDialogEx)

CBookTab::CBookTab(CWnd* pParent /*=NULL*/)
	: CDialogEx(CBookTab::IDD, pParent)
	, m_book_list(&theShellManager)
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
	ON_WM_DROPFILES()
	ON_WM_SHOWWINDOW()
END_MESSAGE_MAP()

void CBookTab::InitListBox()
{
	CString strFolder = theSetting.GetBookMagDir();
	m_book_list.DisplayFolder(strFolder);
}

void CBookTab::OnDoubleClick()
{
	m_book_list.DoDefaultDClick(m_book_list.GetCurSel());
}

void CBookTab::OnSelectChanged()
{
	int nItem = m_book_list.GetCurSel();
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

BOOL CBookTab::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	return TRUE;
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
		CWnd* pFocusWnd = GetFocus();

		switch((UINT)pMsg->wParam)
		{
		case VK_RETURN:
			{
				if (pFocusWnd != &m_item_text) {
					return TRUE;
				}
			}
			break;
		case VK_DELETE:
			{
				if (pFocusWnd == &m_book_list)
				{
					int nItem = m_book_list.GetCurSel();
					if (nItem >= 0)
					{
						CString strPath = m_book_list.GetItemPath(nItem);
						if (PathIsDirectory(strPath)) {
							DeleteDirectory(strPath);
						} else {
							DeleteFile(strPath);
						}
					}

					m_book_list.Refresh();

					return TRUE;
				}
			}
			break;
		case VK_F5:
			{
				m_book_list.Refresh();

				return TRUE;
			}
			break;
		case VK_F3:
			{
				m_search_edit.SetFocus();

				return TRUE;
			}
			break;
		case VK_F2:
			{
				if (pFocusWnd == &m_book_list)
				{
					OnRenameBook();
				}

				return TRUE;
			}
			break;
		default:
			break;
		}
	}

	return CDialogEx::PreTranslateMessage(pMsg);
}

void CBookTab::OnDropFiles(HDROP hDropInfo)
{
	for (UINT i = 0; i < DragQueryFile(hDropInfo, -1, NULL, 0); i++)
	{
		TCHAR szFilePath[MAX_PATH];
		DragQueryFile(hDropInfo, i, szFilePath, MAX_PATH);

		TCHAR szFileName[MAX_PATH];
		_tcsncpy_s(szFileName, szFilePath, MAX_PATH);

		PathStripPath(szFileName);

		CString strBookDir = theSetting.GetBookMagDir();
		strBookDir += _T("\\");
		strBookDir += szFileName;

		strBookDir += _T("\\");
		strBookDir += szFileName;

		_tcsncpy_s(szFileName, strBookDir, MAX_PATH);

		memset(szFilePath + _tcslen(szFilePath), 0, sizeof(TCHAR) * 2);
		memset(szFileName + _tcslen(szFileName), 0, sizeof(TCHAR) * 2);

		SHFILEOPSTRUCT fileOp = {0};
		fileOp.hwnd = GetSafeHwnd();
		fileOp.wFunc = FO_COPY;
		fileOp.pFrom = szFilePath;
		fileOp.pTo = szFileName;
		fileOp.fFlags = FOF_NOCONFIRMMKDIR;

		SHFileOperation(&fileOp);

		PathRemoveFileSpec(strBookDir.GetBuffer());
		PathAppend(strBookDir.GetBuffer(), _T("√Ë ˆ.txt"));
		
		CloseHandle(CreateFile(strBookDir, 0, 0, NULL, CREATE_ALWAYS, 0, NULL));
	}

	m_book_list.Refresh();

	CDialogEx::OnDropFiles(hDropInfo);
}

void CBookTab::OnShowWindow(BOOL bShow, UINT nStatus)
{
	if (bShow) {
		m_search_edit.SetFocus();
	}

	CDialogEx::OnShowWindow(bShow, nStatus);
}

void CBookTab::OnRenameBook()
{
	int nItem = m_book_list.GetCurSel();
	if (nItem >= 0)
	{
		CNameDlg dlg;
		dlg.Op = CNameDlg::Rename;
		m_book_list.GetText(nItem, dlg.m_name);

		if (dlg.DoModal() == IDOK)
		{
			TCHAR szOldPath[MAX_PATH];
			_tcsncpy_s(szOldPath, m_book_list.GetItemPath(nItem), MAX_PATH);

			TCHAR szOldName[MAX_PATH];
			_tcsncpy_s(szOldName, szOldPath, MAX_PATH);

			PathStripPath(szOldName);

			TCHAR szNewPath[MAX_PATH];
			_tcsncpy_s(szNewPath, szOldPath, MAX_PATH);

			PathRemoveFileSpec(szNewPath);
			PathAppend(szNewPath, dlg.m_name);

			CFile::Rename(szOldPath, szNewPath);

			_tcsncpy_s(szOldPath, szNewPath, MAX_PATH);

			PathAppend(szNewPath, dlg.m_name);
			PathAppend(szOldPath, szOldName);

			CFile::Rename(szOldPath, szNewPath);
		}
	}

	m_book_list.Refresh();
}