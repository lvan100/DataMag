#include "stdafx.h"
#include "DataMag.h"
#include "TagTab.h"
#include "FileOp.h"
#include "DataMag.h"
#include "NameDlg.h"
#include "WndHelp.h"
#include "BookDlg.h"
#include "CodeDlg.h"
#include "MainSearch.h"
#include "SettingDlg.h"
#include "DDXControl.h"

IMPLEMENT_DYNAMIC(CTagTab, CDialogEx)

CTagTab::CTagTab(CString strCommand, CWnd* pParent /*=NULL*/)
	: CAppWnd(CTagTab::IDD, pParent)
	, m_tag_list(&theShellManager)
	, m_tag_info(&theShellManager)
	, m_pLastFocusWnd(NULL)
{
	int colon = strCommand.Find(':');
	if (colon > 0) {
		m_command.cmd = strCommand.Left(colon);
		m_command.arg = strCommand.Mid(colon + 1);
	} else {
		m_command.cmd = strCommand;
	}

	m_tag_list.SetListEvent(&m_tag_event);
	m_tag_info.SetListEvent(&m_tag_info_event);

	DirChangeListener listener;
	listener = bind(&CTagTab::OnTagDirChange, this, std::placeholders::_1);
	theApp.AddTagDirChangeListener(listener);

	HICON hSearchIcon = theApp.GetSearchIcon();
	m_info_search_edit.SetSearchIcon(hSearchIcon);
	m_tag_search_edit.SetSearchIcon(hSearchIcon);
}

CTagTab::~CTagTab()
{
}

void CTagTab::DoDataExchange(CDataExchange* pDX)
{
	CAppWnd::DoDataExchange(pDX);
	MFC_DDX_Control(pDX, IDC_SETTING, m_setting);
	MFC_DDX_Control(pDX, IDC_TAG_ADD, m_tag_add);
	MFC_DDX_Control(pDX, IDC_TAG_LIST, m_tag_list);
	MFC_DDX_Control(pDX, IDC_TAG_DELETE, m_tag_delete);
	MFC_DDX_Control(pDX, IDC_TAG_ITEM_INFO, m_item_text);
	MFC_DDX_Control(pDX, IDC_TAG_RENAME, m_tag_rename);
	MFC_DDX_Control(pDX, IDC_TAG_INFO_LIST, m_tag_info);
	MFC_DDX_Control(pDX, IDC_TAG_RELATE_BOOK, m_relate_book);
	MFC_DDX_Control(pDX, IDC_TAG_RELATE_CODE, m_relate_project);
	MFC_DDX_Control(pDX, IDC_TAG_SEARCH_EDIT, m_tag_search_edit);
	MFC_DDX_Control(pDX, IDC_INFO_SEARCH_EDIT, m_info_search_edit);
	MFC_DDX_Control(pDX, IDC_REMOVE_RELATIONSHIP, m_remove_relate);
}

BEGIN_MESSAGE_MAP(CTagTab, CAppWnd)
	ON_WM_ACTIVATE()
	ON_WM_SYSCOMMAND()
	ON_BN_CLICKED(IDC_SETTING, &CTagTab::OnBnClickedSetting)
	ON_BN_CLICKED(IDC_TAG_ADD, &CTagTab::OnBnClickedTagAdd)
	ON_BN_CLICKED(IDC_TAG_DELETE, &CTagTab::OnBnClickedTagDelete)
	ON_BN_CLICKED(IDC_TAG_RENAME, &CTagTab::OnBnClickedTagRename)
	ON_EN_CHANGE(IDC_INFO_SEARCH_EDIT, &CTagTab::OnChangeInfoSearchEdit)
	ON_EN_CHANGE(IDC_TAG_SEARCH_EDIT, &CTagTab::OnChangeTagSearchEdit)
	ON_BN_CLICKED(IDC_TAG_RELATE_BOOK, &CTagTab::OnBnClickedTagRelateBook)
	ON_BN_CLICKED(IDC_TAG_RELATE_CODE, &CTagTab::OnBnClickedTagRelateProject)
	ON_BN_CLICKED(IDC_REMOVE_RELATIONSHIP, &CTagTab::OnBnClickedRemoveRelationship)
END_MESSAGE_MAP()

void CTagTab::TagListEvent::InitListBox()
{
	auto pThis = ((CTagTab*)((BYTE*)this - offsetof(CTagTab, m_tag_event)));

	CString strFolder = theApp.GetTagDir();
	pThis->m_tag_list.DisplayFolder(strFolder);
}

void CTagTab::TagListEvent::OnSelectChanged()
{
	auto pThis = ((CTagTab*)((BYTE*)this - offsetof(CTagTab, m_tag_event)));

	int nItem = pThis->m_tag_list.GetCurSel();
	if (nItem >= 0)
	{
		CString strPath = pThis->m_tag_list.GetItemPath(nItem);
		pThis->m_tag_info.DisplayFolder(strPath);
	}
}

void CTagTab::TagInfoEvent::InitListBox()
{
	auto pThis = ((CTagTab*)((BYTE*)this - offsetof(CTagTab, m_tag_info_event)));
	pThis->m_tag_info.ResetContent();
}

void CTagTab::TagInfoEvent::OnDoubleClick()
{
	auto pThis = ((CTagTab*)((BYTE*)this - offsetof(CTagTab, m_tag_info_event)));
	pThis->m_tag_info.DoDefaultDClick(pThis->m_tag_info.GetCurSel());
}

void CTagTab::TagInfoEvent::OnSelectChanged()
{
	auto pThis = ((CTagTab*)((BYTE*)this - offsetof(CTagTab, m_tag_info_event)));
	int nItem = pThis->m_tag_info.GetCurSel();
	if (nItem >= 0)
	{
		CString strLink = pThis->m_tag_info.GetItemPath(nItem);

		CString strPath;
		GetLinkFilePath(strPath, strLink);

		if (PathIsDirectory(strPath))
		{
			CString strFile = strPath + _T("\\描述.txt");
			CStdioFile file(strFile, CFile::modeReadWrite | CFile::typeText);

			UINT nSize = UINT(file.GetLength()) + 1;
			char* szText = pThis->strText.GetBuffer();

			if ((UINT)pThis->strText.GetLength() < nSize)
			{
				szText = pThis->strText.GetBufferSetLength(nSize);
			}

			memset(szText, 0, nSize);
			file.Read(szText, nSize);

			SetWindowTextA(pThis->m_item_text.GetSafeHwnd(), szText);
		}
	}
}

BOOL CTagTab::OnInitDialog()
{
	CAppWnd::OnInitDialog();

	m_info_search_edit.SetHintText(_T("搜索关联"));
	m_tag_search_edit.SetHintText(_T("搜索标签"));

	CenterWindowInRect(this, theMainSearch->GetIfVisiableRect());

	if (m_command.cmd.CompareNoCase(_T("open")) == 0) {

	} else if (m_command.cmd.CompareNoCase(_T("search")) == 0) {
		if (!m_command.arg.IsEmpty()) {
			m_tag_search_edit.SetWindowText(m_command.arg);
			m_tag_search_edit.SetSel(-1);
			m_tag_list.SetFilterString(m_command.arg);
		}

	} else if (m_command.cmd.CompareNoCase(_T("add")) == 0) {
		PostMessage(WM_COMMAND, MAKEWPARAM(IDC_TAG_ADD, BN_CLICKED), NULL);
	}

	return FALSE; /* 焦点设置 */
}

void CTagTab::OnBnClickedTagAdd()
{
	CNameDlg dlg(this);
	dlg.m_title = _T("新建标签");
	if (dlg.DoModal() == IDOK) {

		CString strFolder = theApp.GetTagDir();
		strFolder += _T("\\") + dlg.m_name;

		if (CreateDirectory(strFolder, nullptr)) {
			m_tag_list.Refresh();
			m_tag_info.DisplayFolder(strFolder);

			m_tag_list.SetFocus();
			m_tag_list.SelectString(0, dlg.m_name);
		} else {
			CString strContent = _T("创建标签\"\"失败！");
			strContent.Insert(5, strFolder);
			MessageBox(strContent, _T("错误"), MB_ICONERROR);
		}
	}
}

void CTagTab::OnBnClickedTagDelete()
{
	int nItem = m_tag_list.GetCurSel();
	if (nItem >= 0) {	
		CString strFolder = m_tag_list.GetItemPath(nItem);
		DeleteDirectory(strFolder);
		m_tag_list.Refresh();
	} else {
		MessageBox(_T("未选中任何标签!"), _T("提示"), MB_ICONINFORMATION);
	}
}

void CTagTab::OnBnClickedTagRename()
{
	int nItem = m_tag_list.GetCurSel();
	if (nItem >= 0) {
		CString strOldFolder = m_tag_list.GetItemPath(nItem);

		int nFind = strOldFolder.ReverseFind('\\') + 1;
		CString strTag = strOldFolder.Mid(nFind);

		CNameDlg dlg;
		dlg.m_name = strTag;
		dlg.m_title = _T("标签重命名");

		if (dlg.DoModal() == IDOK)
		{
			CString strNewFolder = theApp.GetTagDir();
			strNewFolder += _T("\\") + dlg.m_name;
			CFile::Rename(strOldFolder, strNewFolder);

			m_tag_list.Refresh();
			m_tag_info.DisplayFolder(strNewFolder);

			m_tag_list.SetFocus();
			m_tag_list.SelectString(0, dlg.m_name);
		}

	} else {
		MessageBox(_T("未选中任何标签!"), _T("提示"), MB_ICONINFORMATION);
	}
}

void CTagTab::OnBnClickedTagRelateProject()
{
	int nItem = m_tag_list.GetCurSel();
	if (nItem >= 0) {

		CCodeDlg dlg;
		if (dlg.DoModal() == IDOK)
		{
			for (int i = 0; i < dlg.arrProject.GetCount(); i++)
			{
				CString strFolder = dlg.arrProject.GetAt(i);
				int nFind = strFolder.ReverseFind('\\') + 1;
				CString strTag = strFolder.Mid(nFind);

				CString strLink = m_tag_list.GetItemPath(nItem);
				strLink += _T("\\") + strTag + _T(".lnk");
				
				if (FALSE == CreateFileLink(strFolder, strLink)) {
					CString strContent = _T("关联项目\"\"失败！");
					strContent.Insert(5, strFolder);
					MessageBox(strContent, _T("错误"), MB_ICONERROR);
				}
			}

			if (dlg.arrProject.GetCount() > 0) {
				m_tag_info.Refresh();
			}
		}

	} else {
		MessageBox(_T("未选中任何标签!"), _T("提示"), MB_ICONINFORMATION);
	}
}

void CTagTab::OnBnClickedTagRelateBook()
{
	int nItem = m_tag_list.GetCurSel();
	if (nItem >= 0)
	{
		CBookDlg dlg;
		if (dlg.DoModal() == IDOK)
		{
			for (int i = 0; i < dlg.arrBook.GetCount(); i++)
			{
				CString strFolder = dlg.arrBook.GetAt(i);
				int nFind = strFolder.ReverseFind('\\') + 1;
				CString strTag = strFolder.Mid(nFind);

				CString strLink = m_tag_list.GetItemPath(nItem);
				strLink += _T("\\") + strTag + _T(".lnk");

				if (FALSE == CreateFileLink(strFolder, strLink)) {
					CString strContent = _T("关联项目\"\"失败！");
					strContent.Insert(5, strFolder);
					MessageBox(strContent, _T("错误"), MB_ICONERROR);
				}
			}

			if (dlg.arrBook.GetCount() > 0) {
				m_tag_info.Refresh();
			}
		}

	} else {
		MessageBox(_T("未选中任何标签!"), _T("提示"), MB_ICONINFORMATION);
	}
}

void CTagTab::OnBnClickedRemoveRelationship()
{
	int nItem = m_tag_info.GetCurSel();
	if (nItem >= 0) {
		DeleteFile(m_tag_info.GetItemPath(nItem));
		m_tag_info.Refresh();
	} else {
		MessageBox(_T("未选中任何标签!"), _T("提示"), MB_ICONINFORMATION);
	}
}

void CTagTab::OnBnClickedSetting()
{
	CSettingDlg().DoModal();
}

void CTagTab::OnChangeTagSearchEdit()
{
	CString strFilter;
	m_tag_search_edit.GetWindowText(strFilter);
	m_tag_list.SetFilterString(strFilter);
}

void CTagTab::OnChangeInfoSearchEdit()
{
	CString strFilter;
	m_info_search_edit.GetWindowText(strFilter);
	m_tag_info.SetFilterString(strFilter);
}

BOOL CTagTab::PreTranslateMessage(MSG* pMsg)
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
				if (pFocusWnd == &m_tag_info)
				{
					OnBnClickedRemoveRelationship();
				}

				if (pFocusWnd == &m_tag_list)
				{
					OnBnClickedTagDelete();
				}

				return TRUE;
			}
			break;
		case VK_F5:
			{
				m_tag_list.Refresh();
				return TRUE;
			}
			break;
		case VK_F3:
			{
				m_tag_search_edit.SetFocus();
				return TRUE;
			}
			break;
		case VK_F2:
			{
				if (pFocusWnd == &m_tag_list)
				{
					OnBnClickedTagRename();
					return TRUE;
				}
			}
			break;
		default:
			break;
		}
	}

	return CAppWnd::PreTranslateMessage(pMsg);
}

void CTagTab::OnSysCommand(UINT nID, LPARAM lParam)
{
	if (nID == SC_MINIMIZE) {
		m_pLastFocusWnd = GetFocus();
	}

	CAppWnd::OnSysCommand(nID, lParam);
}

void CTagTab::OnActivate(UINT nState, CWnd* pWndOther, BOOL bMinimized)
{
	CAppWnd::OnActivate(nState, pWndOther, bMinimized);

	if (nState == WA_INACTIVE) {
		if (!bMinimized) {
			m_pLastFocusWnd = GetFocus();
		}
	} else {
		if (m_pLastFocusWnd != NULL) {
			m_pLastFocusWnd->SetFocus();
		} else {
			m_tag_search_edit.SetFocus();
		}
	}
}