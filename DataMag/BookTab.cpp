#include "stdafx.h"
#include "FileOp.h"
#include "Common.h"
#include "DataMag.h"
#include "NameDlg.h"
#include "BookTab.h"
#include "Search.h"
#include "SettingDlg.h"
#include "DDXControl.h"

IMPLEMENT_DYNAMIC(CBookTab, CDialogEx)

CBookTab::CBookTab(CWnd* pParent)
	: CDialogEx(CBookTab::IDD, pParent)
	, m_book_list(&theShellManager)
	, m_pLastFocusWnd(nullptr)
{
	m_hCanEditIcon = AfxGetApp()->LoadIcon(IDI_EDIT_TAG);
	m_hNotEditIcon = AfxGetApp()->LoadIcon(IDI_NOT_EDIT);

	m_book_list.EnumFile(FALSE);
	m_book_list.SetListBoxEvent(this);

	DirChangeListener listener;
	listener = bind(&CBookTab::OnBookMagDirChange, this, std::placeholders::_1);
	theApp.AddBookDirChangeListener(this, listener);

	EditChangeListener editListener;
	editListener = bind(&CBookTab::OnItemEditChange, this);
	m_item_text.SetChangeListener(editListener);

	m_search_edit.SetSearchIcon(theApp.GetSearchIcon());
}

CBookTab::~CBookTab()
{
	theApp.DeleteBookDirChangeListener(this);
}

void CBookTab::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_TAG_GROUP, m_tag_group);
	MFC_DDX_Control(pDX, IDC_SETTING, m_setting);
	MFC_DDX_Control(pDX, IDC_BOOK_ADD, m_book_add);
	DDX_Control(pDX, IDC_MORE_INPUT, m_more_input);
	MFC_DDX_Control(pDX, IDC_BOOK_LIST, m_book_list);
	MFC_DDX_Control(pDX, IDC_BOOK_DELETE, m_book_delete);
	MFC_DDX_Control(pDX, IDC_BOOK_RENAME, m_book_rename);
	MFC_DDX_Control(pDX, IDC_TAG_ITEM_INFO, m_item_text);
	MFC_DDX_Control(pDX, IDC_MODIFY_INFO, m_modify_info);
	MFC_DDX_Control(pDX, IDC_BOOK_REFRESH, m_refresh_list);
	DDX_Control(pDX, IDC_TAG_GROUP_TITLE, m_tag_group_title);
	MFC_DDX_Control(pDX, IDC_BOOK_SEARCH_EDIT, m_search_edit);
}

BEGIN_MESSAGE_MAP(CBookTab, CDialogEx)
	ON_WM_MOVE()
	ON_WM_DESTROY()
	ON_WM_ACTIVATE()
	ON_WM_SETFOCUS()
	ON_WM_DROPFILES()
	ON_WM_SYSCOMMAND()
	ON_BN_CLICKED(IDC_SETTING, &CBookTab::OnBnClickedSetting)
	ON_BN_CLICKED(IDC_BOOK_ADD, &CBookTab::OnBnClickedBookAdd)
	ON_BN_CLICKED(IDC_BOOK_DELETE, &CBookTab::OnBnClickedBookDelete)
	ON_BN_CLICKED(IDC_BOOK_RENAME, &CBookTab::OnBnClickedBookRename)
	ON_BN_CLICKED(IDC_MODIFY_INFO, &CBookTab::OnBnClickedModifyInfo)
	ON_BN_CLICKED(IDC_BOOK_REFRESH, &CBookTab::OnBnClickedBookRefresh)
	ON_EN_CHANGE(IDC_BOOK_SEARCH_EDIT, &CBookTab::OnChangeBookSearchEdit)
END_MESSAGE_MAP()

void CBookTab::InitListBox()
{
	CString strFolder = theApp.GetBookDir();
	m_book_list.DisplayFolder(strFolder);
}

void CBookTab::OnDoubleClick()
{
	int nItem = m_book_list.GetCurSel();
	if (nItem >= 0 && nItem < m_book_list.GetCount()){

		m_book_list.DoDefaultDClick(nItem);

		CString strFile = m_book_list.GetItemPath(nItem);
		theApp.SetRecentFile(strFile);
	}
}

void CBookTab::OnSelectChanged()
{
	int nItem = m_book_list.GetCurSel();
	if (nItem >= 0) {

		EnableInfoEidt(FALSE); // 禁用编辑功能

		CString strPath = m_book_list.GetItemPath(nItem);
		if (PathIsDirectory(strPath)) {

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

			file.Close();

			SetWindowTextA(m_item_text.GetSafeHwnd(), szText);
		}
	} else {
		SetWindowTextA(m_item_text.GetSafeHwnd(), nullptr);
	}
}

BOOL CBookTab::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	EnableInfoEidt(FALSE); // 禁用编辑功能

	m_search_edit.EnableSearchButton(FALSE);
	m_search_edit.SetHintText(_T("搜索图书"));

	int limit = m_item_text.GetLimitText();
	m_more_input.SetMaxInputCount(limit);
	m_more_input.SetMoreInputCount(limit);

	CSearch* pWnd = (CSearch*) GetParent();
	CenterWindowInRect(this, pWnd->GetIfVisiableRect());

	[&](){
		LOGFONT logFont = { 0 };
		afxGlobalData.fontRegular.GetLogFont(&logFont);

		logFont.lfHeight = -16;

		HFONT hFont = CreateFontIndirect(&logFont);
		m_tag_group_title.SetFont(CFont::FromHandle(hFont));
	}();

	return FALSE; /* 焦点设置 */
}

void CBookTab::DoCommandAdd()
{
	PostMessage(WM_COMMAND, MAKEWPARAM(IDC_BOOK_ADD, BN_CLICKED), 0);
}

void CBookTab::DoCommandSearch(CString str)
{
	m_book_list.SetFilterString(str);
	m_search_edit.SetWindowText(str);
	m_search_edit.SetSel(-1);

	if (m_book_list.GetCount() == 1){
		CString strPath = m_book_list.GetItemPath(0);
		theApp.SetRecentFile(strPath);
		OpenFolerInShell(strPath);
	}
}

void CBookTab::OnBnClickedBookAdd()
{
	CNameDlg dlg(this);
	dlg.m_title = _T("新建图书");
	if (dlg.DoModal() == IDOK) {

		CString strFolder = theApp.GetBookDir();
		strFolder += _T("\\") + dlg.m_name;

		if (CreateDirectory(strFolder, nullptr)) {
			CString strFile = strFolder + _T("\\描述.txt");
			CloseHandle(CreateFile(strFile, 0, 0, nullptr, CREATE_ALWAYS, 0, nullptr));

			m_book_list.Refresh();

			m_book_list.SetFocus();
			m_book_list.SelectString(0, dlg.m_name);

			// 立即打开文件夹以方便后续操作
			OpenFolerInShell(strFolder);

			// 添加到最近访问列表
			theApp.SetRecentFile(strFolder);

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
		case VK_ESCAPE: 
			{
				DestroyThisWindow();
				return TRUE;
			}
			break;
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
	for (UINT i = 0; i < DragQueryFile(hDropInfo, -1, nullptr, 0); i++)
	{
		TCHAR szOrgFilePath[MAX_PATH];
		DragQueryFile(hDropInfo, i, szOrgFilePath, MAX_PATH);

		TCHAR szFileName[MAX_PATH];
		_tcsncpy_s(szFileName, szOrgFilePath, MAX_PATH);

		PathStripPath(szFileName);

		CString strBookDir = theApp.GetBookDir();
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
		
		CloseHandle(CreateFile(strBookDir, 0, 0, nullptr, CREATE_ALWAYS, 0, nullptr));

		RenameBook([&]()->CString{
			return szFileName;
		},[&]()->CString{
			return theApp.GetBookDir() + _T("\\") + szFileName;
		});
	}

	m_book_list.Refresh();

	CDialogEx::OnDropFiles(hDropInfo);
}

void CBookTab::EnableInfoEidt(BOOL enable)
{
	if (enable) {
		m_item_text.SetReadOnly(FALSE);
		m_more_input.ShowWindow(SW_SHOW);
		m_modify_info.SetIcon(m_hNotEditIcon);
		m_modify_info.SetWindowText(_T("禁止编辑"));
		m_item_text.SetBackgroundColor(TRUE, RGB(0,0,0));
	} else {
		DWORD dwStyle = m_item_text.GetStyle();
		if (dwStyle & ES_READONLY) {
		} else {

			m_item_text.SetReadOnly(TRUE);
			m_more_input.ShowWindow(SW_HIDE);
			m_modify_info.SetIcon(m_hCanEditIcon);
			m_modify_info.SetWindowText(_T("编辑信息"));
			m_item_text.SetBackgroundColor(FALSE, RGB(226,226,226));

			int nItem = m_book_list.GetLastSelItem();
			if (nItem >= 0) {
				CString strPath = m_book_list.GetItemPath(nItem);
				CString strFile = strPath + _T("\\描述.txt");
				CStdioFile file(strFile, CFile::modeReadWrite | CFile::typeText);

				CString strText;
				m_item_text.GetWindowText(strText);

				CStringA szText(strText);
				file.Write(szText, szText.GetLength());

				file.Close();
			}
		}
	}
}

void CBookTab::OnBnClickedModifyInfo()
{
	DWORD dwStyle = m_item_text.GetStyle();
	if (dwStyle & ES_READONLY) {
		EnableInfoEidt(TRUE);
	} else {
		EnableInfoEidt(FALSE);
	}
}

void CBookTab::OnItemEditChange()
{
	int limit = m_item_text.GetLimitText();
	int length = m_item_text.GetTextLengthEx(GTL_NUMCHARS);
	m_more_input.SetMoreInputCount(limit - length);
}

void CBookTab::OnDestroy()
{
	CDialogEx::OnDestroy();

	EnableInfoEidt(FALSE);
}

void CBookTab::OnMove(int x, int y)
{
	CDialogEx::OnMove(x, y);

	if (IsWindowVisible()) {
		CRect rcWindow;
		GetWindowRect(rcWindow);

		auto* pWnd = (CSearch*) GetParent();
		pWnd->SetIfVisiableRect(rcWindow);
	}
}

void CBookTab::DestroyThisWindow()
{
	auto* pSearchWnd = (CSearch*) GetParent();
	pSearchWnd->DeleteTabWnd(this);
}

void CBookTab::OnSysCommand(UINT nID, LPARAM lParam)
{
	if (nID == SC_MINIMIZE) {
		GetParent()->SendMessage(WM_SYSCOMMAND, nID, lParam);

	} else if (nID == SC_CLOSE) {
		DestroyThisWindow();

	} else {
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

void CBookTab::OnActivate(UINT nState, CWnd* pWndOther, BOOL bMinimized)
{
	CDialogEx::OnActivate(nState, pWndOther, bMinimized);

	if (nState == WA_INACTIVE ) {
		// 已经最小化的窗口不再处理
		if (!bMinimized) {
			m_pLastFocusWnd = GetFocus();
		}
	} else {
		if (m_pLastFocusWnd != nullptr) {
			m_pLastFocusWnd->SetFocus();
		} else {
			CWnd* pWnd = GetDefaultFocusWnd();
			if (pWnd != nullptr) {
				pWnd->SetFocus();
			}
		}
	}
}

void CBookTab::OnSetFocus(CWnd* pOldWnd)
{
	CDialogEx::OnSetFocus(pOldWnd);

	if (m_pLastFocusWnd != nullptr) {
		m_pLastFocusWnd->SetFocus();
	} else {
		CWnd* pWnd = GetDefaultFocusWnd();
		if (pWnd != nullptr) {
			pWnd->SetFocus();
		}
	}
}