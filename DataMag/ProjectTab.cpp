#include "stdafx.h"
#include "FileOp.h"
#include "DataMag.h"
#include "NameDlg.h"
#include "SettingDlg.h"
#include "ProjectTab.h"

IMPLEMENT_DYNAMIC(CProjectTab, CDialogEx)

CProjectTab::CProjectTab(CWnd* pParent /*=NULL*/)
	: CDialogEx(CProjectTab::IDD, pParent)
	, m_project_list(&theShellManager)
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
	ON_BN_CLICKED(IDC_PROJECT_ADD, &CProjectTab::OnBnClickedProjectAdd)
	ON_BN_CLICKED(IDC_PROJECT_DELETE, &CProjectTab::OnBnClickedProjectDelete)
	ON_BN_CLICKED(IDC_PROJECT_RENAME, &CProjectTab::OnBnClickedProjectRename)
	ON_BN_CLICKED(IDC_PROJECT_REFRESH, &CProjectTab::OnBnClickedProjectRefresh)
	ON_EN_CHANGE(IDC_PROJECT_SEARCH_EDIT, &CProjectTab::OnChangeProjectSearchEdit)
	ON_WM_DROPFILES()
	ON_WM_SHOWWINDOW()
END_MESSAGE_MAP()

void CProjectTab::InitListBox()
{
	CString strFolder = theSetting.GetCodeMagDir();
	m_project_list.DisplayFolder(strFolder);
}

void CProjectTab::OnDoubleClick()
{
	m_project_list.DoDefaultDClick(m_project_list.GetCurSel());
}

void CProjectTab::OnSelectChanged()
{
	int nItem = m_project_list.GetCurSel();
	if (nItem >= 0)
	{
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

			SetWindowTextA(m_item_text.GetSafeHwnd(), szText);
		}
	}
}

BOOL CProjectTab::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	return FALSE; /* 焦点设置 */
}

void CProjectTab::OnBnClickedProjectAdd()
{
	CNameDlg dlg;
	dlg.m_title = _T("新建项目");
	if (dlg.DoModal() == IDOK) {

		CString strFolder = theSetting.GetCodeMagDir();
		strFolder += _T("\\") + dlg.m_name;

		if (CreateDirectory(strFolder, nullptr)) {

			CString strDataFolder = strFolder + _T("\\资料");
			if (CreateDirectory(strDataFolder, nullptr)){
				CreateDirectory(strDataFolder + _T("\\资料"), nullptr);
				CreateDirectory(strDataFolder + _T("\\官网"), nullptr);
			} else{
				CString strContent = _T("创建资料目录\"\"失败！");
				strContent.Insert(7, strDataFolder);
				MessageBox(strContent, _T("错误"), MB_ICONERROR);
			}

			CString strCodeFolder = strFolder + _T("\\源码");
			if (!CreateDirectory(strCodeFolder, nullptr)){
				CString strContent = _T("创建源码目录\"\"失败！");
				strContent.Insert(7, strCodeFolder);
				MessageBox(strContent, _T("错误"), MB_ICONERROR);
			}

			CString strFile = strFolder + _T("\\描述.txt");
			CloseHandle(CreateFile(strFile, 0, 0, NULL, CREATE_ALWAYS, 0, NULL));

			m_project_list.Refresh();

			m_project_list.SetFocus();
			m_project_list.SelectString(0, dlg.m_name);

		} else {
			CString strContent = _T("创建项目目录\"\"失败！");
			strContent.Insert(7, strFolder);
			MessageBox(strContent, _T("错误"), MB_ICONERROR);
		}
	}
}

void CProjectTab::OnBnClickedProjectDelete()
{
	int nItem = m_project_list.GetCurSel();
	if (nItem >= 0)
	{
		CString strPath = m_project_list.GetItemPath(nItem);
		if (PathIsDirectory(strPath)) {
			DeleteDirectory(strPath);
		} else {
			DeleteFile(strPath);
		}

		m_project_list.Refresh();
	}
}

void CProjectTab::OnBnClickedProjectRename()
{
	int nItem = m_project_list.GetCurSel();
	if (nItem >= 0)
	{
		CNameDlg dlg;
		dlg.m_title = _T("项目重命名");
		m_project_list.GetText(nItem, dlg.m_name);

		if (dlg.DoModal() == IDOK)
		{
			TCHAR szOldPath[MAX_PATH];
			_tcsncpy_s(szOldPath, m_project_list.GetItemPath(nItem), MAX_PATH);

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
					return; /* 文件已存在，重命名失败! */

				} else {
					CFile::Rename(szOldPath, szNewPath);
				}
			}

			m_project_list.Refresh();
		}

		m_project_list.SetFocus();
		m_project_list.SelectString(0, dlg.m_name);
	}
}

void CProjectTab::OnBnClickedProjectRefresh()
{
	m_project_list.Refresh();
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
		CWnd* pFocusWnd = GetFocus();

		switch((UINT)pMsg->wParam)
		{
		case VK_ESCAPE:
			{
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

void CProjectTab::OnDropFiles(HDROP hDropInfo)
{
	for (UINT i = 0; i < DragQueryFile(hDropInfo, -1, NULL, 0); i++)
	{
		TCHAR szFilePath[MAX_PATH];
		DragQueryFile(hDropInfo, i, szFilePath, MAX_PATH);

		TCHAR szFileName[MAX_PATH];
		_tcsncpy_s(szFileName, szFilePath, MAX_PATH);

		PathStripPath(szFileName);
		PathRemoveExtension(szFileName);

		CString strBookDir = theSetting.GetCodeMagDir();
		strBookDir += _T("\\");
		strBookDir += szFileName;

		if (PathIsDirectory(szFilePath))
		{
			strBookDir += _T("\\源码\\");
			strBookDir += szFileName;
		}
		else
		{
			_tcsncpy_s(szFileName, szFilePath, MAX_PATH);

			PathStripPath(szFileName);

			strBookDir += _T("\\源码\\");
			strBookDir += szFileName;
		}

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
		PathRemoveFileSpec(strBookDir.GetBuffer());
		PathAppend(strBookDir.GetBuffer(), _T("描述.txt"));

		CloseHandle(CreateFile(strBookDir, 0, 0, NULL, CREATE_ALWAYS, 0, NULL));

		PathRemoveFileSpec(strBookDir.GetBuffer());
		PathAppend(strBookDir.GetBuffer(), _T("\\资料"));

		CreateDirectory(strBookDir, NULL);
		
		PathAppend(strBookDir.GetBuffer(), _T("\\资料"));

		CreateDirectory(strBookDir, NULL);

		PathRemoveFileSpec(strBookDir.GetBuffer());
		PathAppend(strBookDir.GetBuffer(), _T("\\官网"));

		CreateDirectory(strBookDir, NULL);
	}

	m_project_list.Refresh();

	CDialogEx::OnDropFiles(hDropInfo);
}

void CProjectTab::OnShowWindow(BOOL bShow, UINT nStatus)
{
	if (bShow) {
		m_search_edit.SetFocus();
	}

	CDialogEx::OnShowWindow(bShow, nStatus);
}