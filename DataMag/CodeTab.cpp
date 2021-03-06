#include "stdafx.h"
#include "FileOp.h"
#include "Common.h"
#include "DataMag.h"
#include "CodeTab.h"
#include "NameDlg.h"
#include "Search.h"
#include "SettingDlg.h"
#include "DDXControl.h"
#include "ImageResource.h"

IMPLEMENT_DYNAMIC(CCodeTab, CDialogEx)

CCodeTab::CCodeTab(CWnd* pParent)
	: CDialogEx(CCodeTab::IDD, pParent)
	, m_project_list(&theShellManager)
	, m_pLastFocusWnd(nullptr)
{
	m_hCanEditIcon = AfxGetApp()->LoadIcon(IDI_EDIT_TAG);
	m_hNotEditIcon = AfxGetApp()->LoadIcon(IDI_NOT_EDIT);

	m_project_list.EnumFile(FALSE);
	m_project_list.SetListBoxEvent(this);

	DirChangeListener listener;
	listener = bind(&CCodeTab::OnCodeMagDirChange, this, std::placeholders::_1);
	theApp.AddCodeDirChangeListener(this, listener);

	EditChangeListener editListener;
	editListener = bind(&CCodeTab::OnItemEditChange, this);
	m_item_text.SetChangeListener(editListener);

	m_search_edit.SetSearchIcon(theImageResource.m_hSearchIcon);
}

CCodeTab::~CCodeTab()
{
	theApp.DeleteCodeDirChangeListener(this);
}

void CCodeTab::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_TAG_GROUP, m_tag_group);
	MFC_DDX_Control(pDX, IDC_SETTING, m_setting);
	DDX_Control(pDX, IDC_MORE_INPUT, m_more_input);
	MFC_DDX_Control(pDX, IDC_CODE_ADD, m_project_add);
	MFC_DDX_Control(pDX, IDC_CODE_LIST, m_project_list);
	MFC_DDX_Control(pDX, IDC_TAG_ITEM_INFO, m_item_text);
	MFC_DDX_Control(pDX, IDC_MODIFY_INFO, m_modify_info);
	MFC_DDX_Control(pDX, IDC_CODE_DELETE, m_project_delete);
	MFC_DDX_Control(pDX, IDC_CODE_RENAME, m_project_rename);
	DDX_Control(pDX, IDC_TAG_GROUP_TITLE, m_tag_group_title);
	MFC_DDX_Control(pDX, IDC_CODE_SEARCH_EDIT, m_search_edit);
	MFC_DDX_Control(pDX, IDC_CODE_REFRESH, m_project_refresh);
}

BEGIN_MESSAGE_MAP(CCodeTab, CDialogEx)
	ON_WM_MOVE()
	ON_WM_DESTROY()
	ON_WM_ACTIVATE()
	ON_WM_SETFOCUS()
	ON_WM_DROPFILES()
	ON_WM_SYSCOMMAND()
	ON_BN_CLICKED(IDC_SETTING, &CCodeTab::OnBnClickedSetting)
	ON_BN_CLICKED(IDC_CODE_ADD, &CCodeTab::OnBnClickedProjectAdd)
	ON_BN_CLICKED(IDC_MODIFY_INFO, &CCodeTab::OnBnClickedModifyInfo)
	ON_BN_CLICKED(IDC_CODE_DELETE, &CCodeTab::OnBnClickedProjectDelete)
	ON_BN_CLICKED(IDC_CODE_RENAME, &CCodeTab::OnBnClickedProjectRename)
	ON_BN_CLICKED(IDC_CODE_REFRESH, &CCodeTab::OnBnClickedProjectRefresh)
	ON_EN_CHANGE(IDC_CODE_SEARCH_EDIT, &CCodeTab::OnChangeProjectSearchEdit)
END_MESSAGE_MAP()

void CCodeTab::InitListBox()
{
	CString strFolder = theApp.GetCodeDir();
	m_project_list.DisplayFolder(strFolder);
}

void CCodeTab::OnDoubleClick()
{
	int nItem = m_project_list.GetCurSel();
	if (nItem >= 0 && nItem < m_project_list.GetCount()){

		m_project_list.DoDoubleClick(nItem);

		CString strFile = m_project_list.GetItemPath(nItem);
		theApp.SetRecentFile(strFile);
	}
}

void CCodeTab::OnSelectChanged()
{
	int nItem = m_project_list.GetCurSel();
	if (nItem >= 0) {

		EnableInfoEidt(FALSE); // 禁用编辑功能

		CString strPath = m_project_list.GetItemPath(nItem);
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

			file.Close();

			SetWindowTextA(m_item_text.GetSafeHwnd(), szText);
		}
	} else {
		SetWindowTextA(m_item_text.GetSafeHwnd(), nullptr);
	}
}

BOOL CCodeTab::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	EnableInfoEidt(FALSE); // 禁用编辑功能

	m_search_edit.EnableSearchButton(FALSE);
	m_search_edit.SetHintText(_T("搜索项目"));

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

void CCodeTab::DoCommandAdd()
{
	PostMessage(WM_COMMAND, MAKEWPARAM(IDC_CODE_ADD, BN_CLICKED), 0);
}

void CCodeTab::DoCommandSearch(CString str)
{
	m_project_list.SetFilterString(str);
	m_search_edit.SetWindowText(str);
	m_search_edit.SetSel(-1);

	if (m_project_list.GetCount() == 1){
		CString strPath = m_project_list.GetItemPath(0);
		theApp.SetRecentFile(strPath);
		OpenFolerInShell(strPath);
	}
}

void CCodeTab::OnBnClickedProjectAdd()
{
	CNameDlg dlg(this);
	dlg.m_title = _T("新建项目");
	if (dlg.DoModal() == IDOK) {

		
	}
}

void CCodeTab::OnBnClickedProjectDelete()
{
	int nItem = m_project_list.GetCurSel();
	if (nItem >= 0)
	{
		

		m_project_list.Refresh();
	}
}

CString CCodeTab::RenameBook(function<CString()> getSelName, function<CString()> getSelPath)
{
	CNameDlg dlg;
	dlg.m_name = getSelName();
	dlg.m_title = _T("项目重命名");

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
				CString strContent = _T("项目目录\"\"已存在！");
				strContent.Insert(5, szNewPath);
				MessageBox(strContent, _T("错误"), MB_ICONERROR);
				return _T(""); /* 文件已存在，重命名失败! */

			} else {
				CFile::Rename(szOldPath, szNewPath);
			}
		}

		return dlg.m_name;
	}
	return _T("");
}

void CCodeTab::OnBnClickedProjectRename()
{
	int nItem = m_project_list.GetCurSel();
	if (nItem >= 0) {

		CString strNewName = RenameBook([&]()->CString{
			CString strName;
			m_project_list.GetText(nItem, strName);
			return strName;
		}, [&]()->CString{
			return m_project_list.GetItemPath(nItem);
		});

		if (strNewName.GetLength() > 0) {
			m_project_list.Refresh();
			m_project_list.SetFocus();
			m_project_list.SelectString(0, strNewName);
		}
	}
}

void CCodeTab::OnBnClickedSetting()
{
	CSettingDlg().DoModal();
}

void CCodeTab::OnBnClickedProjectRefresh()
{
	m_project_list.Refresh();
}

void CCodeTab::OnChangeProjectSearchEdit()
{
	CString strFilter;
	m_search_edit.GetWindowText(strFilter);
	m_project_list.SetFilterString(strFilter);
}

BOOL CCodeTab::PreTranslateMessage(MSG* pMsg)
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
				if (pFocusWnd == &m_project_list)
				{
					OnBnClickedProjectDelete();
					return TRUE;
				}
			}
			break;
		case VK_F5:
			{
				OnBnClickedProjectRefresh();
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
				if (pFocusWnd == &m_project_list)
				{
					OnBnClickedProjectRename();
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

void CCodeTab::OnDropFiles(HDROP hDropInfo)
{
	for (UINT i = 0; i < DragQueryFile(hDropInfo, -1, nullptr, 0); i++)
	{
		TCHAR szOrgFilePath[MAX_PATH];
		DragQueryFile(hDropInfo, i, szOrgFilePath, MAX_PATH);

		TCHAR szFileName[MAX_PATH];
		_tcsncpy_s(szFileName, szOrgFilePath, MAX_PATH);

		PathStripPath(szFileName);
		PathRemoveExtension(szFileName);

		CString strBookDir = theApp.GetCodeDir();
		strBookDir += _T("\\");
		strBookDir += szFileName;

		if (PathIsDirectory(szOrgFilePath))
		{
			strBookDir += _T("\\源码\\");
			strBookDir += szFileName;
		}
		else
		{
			_tcsncpy_s(szFileName, szOrgFilePath, MAX_PATH);

			PathStripPath(szFileName);

			strBookDir += _T("\\源码\\");
			strBookDir += szFileName;
		}

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
		PathRemoveFileSpec(strBookDir.GetBuffer());
		PathAppend(strBookDir.GetBuffer(), _T("描述.txt"));

		CloseHandle(CreateFile(strBookDir, 0, 0, nullptr, CREATE_ALWAYS, 0, nullptr));

		PathRemoveFileSpec(strBookDir.GetBuffer());
		PathAppend(strBookDir.GetBuffer(), _T("\\资料"));

		CreateDirectory(strBookDir, nullptr);
		
		PathAppend(strBookDir.GetBuffer(), _T("\\资料"));

		CreateDirectory(strBookDir, nullptr);

		PathRemoveFileSpec(strBookDir.GetBuffer());
		PathAppend(strBookDir.GetBuffer(), _T("\\官网"));

		CreateDirectory(strBookDir, nullptr);

		RenameBook([&]()->CString{
			return szFileName;
		}, [&]()->CString{
			return theApp.GetCodeDir() + _T("\\") + szFileName;
		});
	}

	m_project_list.Refresh();

	CDialogEx::OnDropFiles(hDropInfo);
}

void CCodeTab::EnableInfoEidt(BOOL enable)
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
			
			int nItem = m_project_list.GetLastSelItem();
			if (nItem >= 0) {
				CString strPath = m_project_list.GetItemPath(nItem);
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

void CCodeTab::OnBnClickedModifyInfo()
{
	DWORD dwStyle = m_item_text.GetStyle();
	if (dwStyle & ES_READONLY) {
		EnableInfoEidt(TRUE);
	} else {
		EnableInfoEidt(FALSE);
	}
}

void CCodeTab::OnItemEditChange()
{
	int limit = m_item_text.GetLimitText();
	int length = m_item_text.GetTextLengthEx(GTL_NUMCHARS);
	m_more_input.SetMoreInputCount(limit - length);
}

void CCodeTab::OnDestroy()
{
	CDialogEx::OnDestroy();

	EnableInfoEidt(FALSE);
}

void CCodeTab::OnMove(int x, int y)
{
	CDialogEx::OnMove(x, y);

	if (IsWindowVisible()) {
		CRect rcWindow;
		GetWindowRect(rcWindow);

		auto* pWnd = (CSearch*) GetParent();
		pWnd->SetIfVisiableRect(rcWindow);
	}
}

void CCodeTab::DestroyThisWindow()
{
	auto* pSearchWnd = (CSearch*) GetParent();
	pSearchWnd->DeleteTabWnd(this);
}

void CCodeTab::OnSysCommand(UINT nID, LPARAM lParam)
{
	if (nID == SC_MINIMIZE) {
		GetParent()->SendMessage(WM_SYSCOMMAND, nID, lParam);

	} else if (nID == SC_CLOSE) {
		DestroyThisWindow();

	} else {
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

void CCodeTab::OnActivate(UINT nState, CWnd* pWndOther, BOOL bMinimized)
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

void CCodeTab::OnSetFocus(CWnd* pOldWnd)
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