#include "stdafx.h"
#include "FileOp.h"
#include "DataMag.h"
#include "CodeTab.h"
#include "NameDlg.h"
#include "WndHelp.h"
#include "MainSearch.h"
#include "SettingDlg.h"
#include "DDXControl.h"

IMPLEMENT_DYNAMIC(CCodeTab, CDialogEx)

CCodeTab::CCodeTab(CString strCommand, CWnd* pParent /*=NULL*/)
	: CDialogEx(CCodeTab::IDD, pParent)
	, m_project_list(&theShellManager)
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

	m_project_list.SetListEvent(this);

	DirChangeListener listener;
	listener = bind(&CCodeTab::OnCodeMagDirChange, this, std::placeholders::_1);
	theApp.AddCodeDirChangeListener(listener);

	HICON hSearchIcon = (HICON)LoadImage(AfxGetInstanceHandle()
		, MAKEINTRESOURCE(IDI_SEARCH)
		, IMAGE_ICON, 0, 0, 0);
	m_search_edit.SetSearchIcon(hSearchIcon);
}

CCodeTab::~CCodeTab()
{
}

void CCodeTab::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	MFC_DDX_Control(pDX, IDC_SETTING, m_setting);
	MFC_DDX_Control(pDX, IDC_CODE_ADD, m_project_add);
	MFC_DDX_Control(pDX, IDC_CODE_LIST, m_project_list);
	MFC_DDX_Control(pDX, IDC_TAG_ITEM_INFO, m_item_text);
	MFC_DDX_Control(pDX, IDC_CODE_DELETE, m_project_delete);
	MFC_DDX_Control(pDX, IDC_CODE_RENAME, m_project_rename);
	MFC_DDX_Control(pDX, IDC_CODE_SEARCH_EDIT, m_search_edit);
	MFC_DDX_Control(pDX, IDC_CODE_REFRESH, m_project_refresh);
}

BEGIN_MESSAGE_MAP(CCodeTab, CDialogEx)
	ON_WM_ACTIVATE()
	ON_WM_DROPFILES()
	ON_WM_SYSCOMMAND()
	ON_BN_CLICKED(IDC_SETTING, &CCodeTab::OnBnClickedSetting)
	ON_BN_CLICKED(IDC_CODE_ADD, &CCodeTab::OnBnClickedProjectAdd)
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
	m_project_list.DoDefaultDClick(nItem);

	CString strFile = m_project_list.GetItemPath(nItem);
	theApp.SetRecentFile(strFile);
}

void CCodeTab::OnSelectChanged()
{
	int nItem = m_project_list.GetCurSel();
	if (nItem >= 0)
	{
		CString strPath = m_project_list.GetItemPath(nItem);
		if (PathIsDirectory(strPath))
		{
			CString strFile = strPath + _T("\\����.txt");
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

BOOL CCodeTab::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	SetIcon(m_hIcon, TRUE);
	SetIcon(m_hIcon, FALSE);

	m_search_edit.SetHintText(_T("������Ŀ"));

	CenterWindowInRect(this, theMainSearch->GetIfVisiableRect());

	if (m_command.cmd.CompareNoCase(_T("open")) == 0) {

	} else if (m_command.cmd.CompareNoCase(_T("search")) == 0) {
		if (!m_command.arg.IsEmpty()) {
			m_search_edit.SetWindowText(m_command.arg);
			m_search_edit.SetSel(-1);
			m_project_list.SetFilterString(m_command.arg);
		}

	} else if (m_command.cmd.CompareNoCase(_T("add")) == 0) {
		PostMessage(WM_COMMAND, MAKEWPARAM(IDC_CODE_ADD, BN_CLICKED), NULL);
	}

	return FALSE; /* �������� */
}

void CCodeTab::OnBnClickedProjectAdd()
{
	CNameDlg dlg(this);
	dlg.m_title = _T("�½���Ŀ");
	if (dlg.DoModal() == IDOK) {

		CString strFolder = theApp.GetCodeDir();
		strFolder += _T("\\") + dlg.m_name;

		if (CreateDirectory(strFolder, nullptr)) {

			CString strDataFolder = strFolder + _T("\\����");
			if (CreateDirectory(strDataFolder, nullptr)){
				CreateDirectory(strDataFolder + _T("\\����"), nullptr);
				CreateDirectory(strDataFolder + _T("\\����"), nullptr);
			} else{
				CString strContent = _T("��������Ŀ¼\"\"ʧ�ܣ�");
				strContent.Insert(7, strDataFolder);
				MessageBox(strContent, _T("����"), MB_ICONERROR);
			}

			CString strCodeFolder = strFolder + _T("\\Դ��");
			if (!CreateDirectory(strCodeFolder, nullptr)){
				CString strContent = _T("����Դ��Ŀ¼\"\"ʧ�ܣ�");
				strContent.Insert(7, strCodeFolder);
				MessageBox(strContent, _T("����"), MB_ICONERROR);
			}

			CString strFile = strFolder + _T("\\����.txt");
			CloseHandle(CreateFile(strFile, 0, 0, NULL, CREATE_ALWAYS, 0, NULL));

			m_project_list.Refresh();

			m_project_list.SetFocus();
			m_project_list.SelectString(0, dlg.m_name);

			// �������ļ����Է����������
			OpenFoler(strFolder);

		} else {
			CString strContent = _T("������ĿĿ¼\"\"ʧ�ܣ�");
			strContent.Insert(7, strFolder);
			MessageBox(strContent, _T("����"), MB_ICONERROR);
		}
	}
}

void CCodeTab::OnBnClickedProjectDelete()
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

CString CCodeTab::RenameBook(function<CString()> getSelName, function<CString()> getSelPath)
{
	CNameDlg dlg;
	dlg.m_name = getSelName();
	dlg.m_title = _T("��Ŀ������");

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
				CString strContent = _T("��ĿĿ¼\"\"�Ѵ��ڣ�");
				strContent.Insert(5, szNewPath);
				MessageBox(strContent, _T("����"), MB_ICONERROR);
				return _T(""); /* �ļ��Ѵ��ڣ�������ʧ��! */

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
	for (UINT i = 0; i < DragQueryFile(hDropInfo, -1, NULL, 0); i++)
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
			strBookDir += _T("\\Դ��\\");
			strBookDir += szFileName;
		}
		else
		{
			_tcsncpy_s(szFileName, szOrgFilePath, MAX_PATH);

			PathStripPath(szFileName);

			strBookDir += _T("\\Դ��\\");
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
		PathAppend(strBookDir.GetBuffer(), _T("����.txt"));

		CloseHandle(CreateFile(strBookDir, 0, 0, NULL, CREATE_ALWAYS, 0, NULL));

		PathRemoveFileSpec(strBookDir.GetBuffer());
		PathAppend(strBookDir.GetBuffer(), _T("\\����"));

		CreateDirectory(strBookDir, NULL);
		
		PathAppend(strBookDir.GetBuffer(), _T("\\����"));

		CreateDirectory(strBookDir, NULL);

		PathRemoveFileSpec(strBookDir.GetBuffer());
		PathAppend(strBookDir.GetBuffer(), _T("\\����"));

		CreateDirectory(strBookDir, NULL);

		RenameBook([&]()->CString{
			return szFileName;
		}, [&]()->CString{
			return theApp.GetCodeDir() + _T("\\") + szFileName;
		});
	}

	m_project_list.Refresh();

	CDialogEx::OnDropFiles(hDropInfo);
}

void CCodeTab::OnSysCommand(UINT nID, LPARAM lParam)
{
	if (nID == SC_MINIMIZE) {
		m_pLastFocusWnd = GetFocus();
	}

	CDialogEx::OnSysCommand(nID, lParam);
}

void CCodeTab::OnActivate(UINT nState, CWnd* pWndOther, BOOL bMinimized)
{
	CDialogEx::OnActivate(nState, pWndOther, bMinimized);

	if (nState == WA_INACTIVE) {
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