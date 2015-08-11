#include "stdafx.h"
#include "FileOp.h"
#include "DataMag.h"
#include "NameDlg.h"
#include "BookTab.h"
#include "WndHelp.h"
#include "MainSearch.h"
#include "SettingDlg.h"
#include "DDXControl.h"

IMPLEMENT_DYNAMIC(CBookTab, CDialogEx)

CBookTab::CBookTab(CString strCommand, CWnd* pParent /*=NULL*/)
	: CDialogEx(CBookTab::IDD, pParent)
	, m_book_list(&theShellManager)
	, m_pLastFocusWnd(NULL)
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

	DirChangeListener listener;
	listener = bind(&CBookTab::OnBookMagDirChange, this, std::placeholders::_1);
	theApp.AddBookMagDirChangeListener(listener);

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
	MFC_DDX_Control(pDX, IDC_SETTING, m_setting);
	MFC_DDX_Control(pDX, IDC_BOOK_ADD, m_book_add);
	MFC_DDX_Control(pDX, IDC_ITEM_INFO, m_item_text);
	MFC_DDX_Control(pDX, IDC_BOOK_LIST, m_book_list);
	MFC_DDX_Control(pDX, IDC_BOOK_DELETE, m_book_delete);
	MFC_DDX_Control(pDX, IDC_BOOK_RENAME, m_book_rename);
	MFC_DDX_Control(pDX, IDC_BOOK_REFRESH, m_refresh_list);
	MFC_DDX_Control(pDX, IDC_BOOK_SEARCH_EDIT, m_search_edit);
}

BEGIN_MESSAGE_MAP(CBookTab, CDialogEx)
	ON_WM_ACTIVATE()
	ON_WM_DROPFILES()
	ON_WM_SYSCOMMAND()
	ON_BN_CLICKED(IDC_SETTING, &CBookTab::OnBnClickedSetting)
	ON_BN_CLICKED(IDC_BOOK_ADD, &CBookTab::OnBnClickedBookAdd)
	ON_BN_CLICKED(IDC_BOOK_DELETE, &CBookTab::OnBnClickedBookDelete)
	ON_BN_CLICKED(IDC_BOOK_RENAME, &CBookTab::OnBnClickedBookRename)
	ON_BN_CLICKED(IDC_BOOK_REFRESH, &CBookTab::OnBnClickedBookRefresh)
	ON_EN_CHANGE(IDC_BOOK_SEARCH_EDIT, &CBookTab::OnChangeBookSearchEdit)
END_MESSAGE_MAP()

void CBookTab::InitListBox()
{
	CString strFolder = theApp.GetBookMagDir();
	m_book_list.DisplayFolder(strFolder);
}

void CBookTab::OnDoubleClick()
{
	int nItem = m_book_list.GetCurSel();
	m_book_list.DoDefaultDClick(nItem);

	CString strFile = m_book_list.GetItemPath(nItem);
	theApp.SetRecentFile(strFile);
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

	m_search_edit.SetHintText(_T("搜索图书"));

	CenterWindowInRect(this, theMainSearch->GetIfVisiableRect());

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
	CNameDlg dlg(this);
	dlg.m_title = _T("新建图书");
	if (dlg.DoModal() == IDOK) {

		CString strFolder = theApp.GetBookMagDir();
		strFolder += _T("\\") + dlg.m_name;

		if (CreateDirectory(strFolder, nullptr)) {
			CString strFile = strFolder + _T("\\描述.txt");
			CloseHandle(CreateFile(strFile, 0, 0, NULL, CREATE_ALWAYS, 0, NULL));

			m_book_list.Refresh();

			m_book_list.SetFocus();
			m_book_list.SelectString(0, dlg.m_name);

			// 立即打开文件夹以方便后续操作
			OpenFoler(strFolder);

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

CString CBookTab::RenameBook(function<CString()> getSelName, function<CString()> getSelPath)
{
	CNameDlg dlg;
	dlg.m_name = getSelName();
	dlg.m_title = _T("图书重命名");

	if (dlg.DoModal() == IDOK)
	{
		TCHAR szOldPath[MAX_PATH];
		_tcsncpy_s(szOldPath, getSelPath(), MAX_PATH);

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
				return _T(""); /* 文件已存在，重命名失败! */

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

		return dlg.m_name;
	}
	return _T("");
}

void CBookTab::OnBnClickedBookRename()
{
	int nItem = m_book_list.GetCurSel();
	if (nItem >= 0)
	{
		CString strName = RenameBook([&]()->CString{
			CString strName;
			m_book_list.GetText(nItem, strName);
			return strName;
		}, [&]()->CString{
			return m_book_list.GetItemPath(nItem);
		});

		if (strName.GetLength() > 0) {
			m_book_list.Refresh();
			m_book_list.SetFocus();
			m_book_list.SelectString(0, strName);
		}
	}
}

void CBookTab::OnBnClickedSetting()
{
	CSettingDlg().DoModal();
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
		TCHAR szOrgFilePath[MAX_PATH];
		DragQueryFile(hDropInfo, i, szOrgFilePath, MAX_PATH);

		TCHAR szFileName[MAX_PATH];
		_tcsncpy_s(szFileName, szOrgFilePath, MAX_PATH);

		PathStripPath(szFileName);

		CString strBookDir = theApp.GetBookMagDir();
		strBookDir += _T("\\");
		strBookDir += szFileName;

		strBookDir += _T("\\");
		strBookDir += szFileName;

		TCHAR szNewFilePath[MAX_PATH];
		_tcsncpy_s(szNewFilePath, strBookDir, MAX_PATH);

		memset(szOrgFilePath + _tcslen(szOrgFilePath), 0, sizeof(TCHAR) * 2);
		memset(szNewFilePath + _tcslen(szNewFilePath), 0, sizeof(TCHAR) * 2);

		SHFILEOPSTRUCT fileOp = {0};
		fileOp.hwnd = GetSafeHwnd();
		fileOp.wFunc = FO_COPY;
		fileOp.pFrom = szOrgFilePath;
		fileOp.pTo = szNewFilePath;
		fileOp.fFlags = FOF_NOCONFIRMMKDIR;

		SHFileOperation(&fileOp);

		PathRemoveFileSpec(strBookDir.GetBuffer());
		PathAppend(strBookDir.GetBuffer(), _T("描述.txt"));
		
		CloseHandle(CreateFile(strBookDir, 0, 0, NULL, CREATE_ALWAYS, 0, NULL));

		RenameBook([&]()->CString{
			return szFileName;
		},[&]()->CString{
			return theApp.GetBookMagDir() + _T("\\") + szFileName;
		});
	}

	m_book_list.Refresh();

	CDialogEx::OnDropFiles(hDropInfo);
}

void CBookTab::OnSysCommand(UINT nID, LPARAM lParam)
{
	if (nID == SC_MINIMIZE) {
		m_pLastFocusWnd = GetFocus();
	}

	CDialogEx::OnSysCommand(nID, lParam);
}

void CBookTab::OnActivate(UINT nState, CWnd* pWndOther, BOOL bMinimized)
{
	CDialogEx::OnActivate(nState, pWndOther, bMinimized);

	if (nState == WA_INACTIVE ) {
		if (!bMinimized) {
			m_pLastFocusWnd = GetFocus();
		}
	} else {
		if (m_pLastFocusWnd != NULL) {
			m_pLastFocusWnd->SetFocus();
		} else {
			m_search_edit.SetFocus();
		}
	}
}