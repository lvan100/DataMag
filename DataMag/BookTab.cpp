#include "stdafx.h"
#include "FileOp.h"
#include "DataMag.h"
#include "NameDlg.h"
#include "BookTab.h"
#include "SettingDlg.h"

IMPLEMENT_DYNAMIC(CBookTab, CDialogEx)

CBookTab::CBookTab(CString strCommand, CWnd* pParent /*=NULL*/)
	: CDialogEx(CBookTab::IDD, pParent)
	, m_book_list(&theShellManager)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);

	int colon = strCommand.Find(':');
	if (colon > 0) {
		m_command.cmd = strCommand.Left(colon);
		m_command.arg = strCommand.Mid(colon + 1);
	} else {
		m_command.cmd = strCommand;
	}

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
	DDX_Control(pDX, IDC_BOOK_ADD, m_book_add);
	DDX_Control(pDX, IDC_ITEM_INFO, m_item_text);
	DDX_Control(pDX, IDC_BOOK_LIST, m_book_list);
	DDX_Control(pDX, IDC_BOOK_DELETE, m_book_delete);
	DDX_Control(pDX, IDC_BOOK_RENAME, m_book_rename);
	DDX_Control(pDX, IDC_BOOK_REFRESH, m_refresh_list);
	DDX_Control(pDX, IDC_BOOK_SEARCH_EDIT, m_search_edit);
}

BEGIN_MESSAGE_MAP(CBookTab, CDialogEx)
	ON_BN_CLICKED(IDC_BOOK_ADD, &CBookTab::OnBnClickedBookAdd)
	ON_BN_CLICKED(IDC_BOOK_DELETE, &CBookTab::OnBnClickedBookDelete)
	ON_BN_CLICKED(IDC_BOOK_RENAME, &CBookTab::OnBnClickedBookRename)
	ON_BN_CLICKED(IDC_BOOK_REFRESH, &CBookTab::OnBnClickedBookRefresh)
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
			CString strFile = strPath + _T("\\描述.txt");
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

	SetIcon(m_hIcon, TRUE);
	SetIcon(m_hIcon, FALSE);

	CenterWindow(GetDesktopWindow());

	if (m_command.cmd.CompareNoCase(_T("open")) == 0) {

	} else if (m_command.cmd.CompareNoCase(_T("search")) == 0) {
		if (!m_command.arg.IsEmpty()) {
			m_search_edit.SetWindowText(m_command.arg);
			m_search_edit.SetSel(-1);
			m_book_list.SetFilterString(m_command.arg);
		}

	} else if (m_command.cmd.CompareNoCase(_T("add")) == 0) {
		PostMessage(WM_COMMAND, MAKEWPARAM(IDC_BOOK_ADD, BN_CLICKED), NULL);
	}

	return FALSE; /* 焦点设置 */
}

void CBookTab::OnBnClickedBookAdd()
{
	CNameDlg dlg;
	dlg.m_title = _T("新建图书");
	if (dlg.DoModal() == IDOK) {

		CString strFolder = theSetting.GetBookMagDir();
		strFolder += _T("\\") + dlg.m_name;

		if (CreateDirectory(strFolder, nullptr)) {
			CString strFile = strFolder + _T("\\描述.txt");
			CloseHandle(CreateFile(strFile, 0, 0, NULL, CREATE_ALWAYS, 0, NULL));

			m_book_list.Refresh();

			m_book_list.SetFocus();
			m_book_list.SelectString(0, dlg.m_name);

		} else {
			CString strContent = _T("创建图书目录\"\"失败！");
			strContent.Insert(7, strFolder);
			MessageBox(strContent, _T("错误"), MB_ICONERROR);
		}
	}
}

void CBookTab::OnBnClickedBookDelete()
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

		m_book_list.Refresh();
	}
}

void CBookTab::OnBnClickedBookRename()
{
	int nItem = m_book_list.GetCurSel();
	if (nItem >= 0)
	{
		CNameDlg dlg;
		dlg.m_title = _T("图书重命名");
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

			if (PathFileExists(szOldPath)) {
				if (PathFileExists(szNewPath)) {
					CString strContent = _T("图书目录\"\"已存在！");
					strContent.Insert(5, szNewPath);
					MessageBox(strContent, _T("错误"), MB_ICONERROR);
					return; /* 文件已存在，重命名失败! */

				} else {
					CFile::Rename(szOldPath, szNewPath);
				}
			}

			_tcsncpy_s(szOldPath, szNewPath, MAX_PATH);

			PathAppend(szNewPath, dlg.m_name);
			PathAppend(szOldPath, szOldName);

			if (PathFileExists(szOldPath)) {
				CFile::Rename(szOldPath, szNewPath);
			}

			m_book_list.Refresh();
		}

		m_book_list.SetFocus();
		m_book_list.SelectString(0, dlg.m_name);
	}
}

void CBookTab::OnBnClickedBookRefresh()
{
	m_book_list.Refresh();
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
					OnBnClickedBookDelete();
					return TRUE;
				}
			}
			break;
		case VK_F5:
			{
				OnBnClickedBookRefresh();
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
					OnBnClickedBookRename();
					return TRUE;
				}
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
		PathAppend(strBookDir.GetBuffer(), _T("描述.txt"));
		
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