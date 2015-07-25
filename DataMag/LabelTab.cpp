#include "stdafx.h"
#include "FileOp.h"
#include "DataMag.h"
#include "NameDlg.h"
#include "LabelTab.h"
#include "SettingDlg.h"
#include "BookSelectDlg.h"
#include "ProjectSelectDlg.h"

IMPLEMENT_DYNAMIC(CLabelTab, CDialogEx)

CLabelTab::CLabelTab(CWnd* pParent /*=NULL*/)
	: CDialogEx(CLabelTab::IDD, pParent)
	, m_label_list(&theShellManager)
	, m_label_info(&theShellManager)
{
	m_label_list.SetListEvent(&m_label_event);
	m_label_info.SetListEvent(&m_label_info_event);

	DirChangeLinster listener;
	listener = bind(&CLabelTab::OnLabelMagDirChange, this, std::placeholders::_1);
	theSetting.AddLabelMagDirChangeListener(listener);

	HICON hSearchIcon = (HICON)LoadImage(AfxGetInstanceHandle()
		, MAKEINTRESOURCE(IDI_SEARCH)
		, IMAGE_ICON, 0, 0, 0);
	m_info_search_edit.SetSearchIcon(hSearchIcon);
	m_label_search_edit.SetSearchIcon(hSearchIcon);
}

CLabelTab::~CLabelTab()
{
}

void CLabelTab::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_ITEM_INFO, m_item_text);
	DDX_Control(pDX, IDC_LABEL_LIST, m_label_list);
	DDX_Control(pDX, IDC_LABEL_INFO_LIST, m_label_info);
	DDX_Control(pDX, IDC_INFO_SEARCH_EDIT, m_info_search_edit);
	DDX_Control(pDX, IDC_LABEL_SEARCH_EDIT, m_label_search_edit);
}

BEGIN_MESSAGE_MAP(CLabelTab, CDialogEx)
	ON_WM_SHOWWINDOW()
	ON_BN_CLICKED(IDC_LABEL_ADD, &CLabelTab::OnBnClickedLabelAdd)
	ON_BN_CLICKED(IDC_LABEL_DELETE, &CLabelTab::OnBnClickedLabelDelete)
	ON_BN_CLICKED(IDC_LABEL_RENAME, &CLabelTab::OnBnClickedLabelRename)
	ON_EN_CHANGE(IDC_INFO_SEARCH_EDIT, &CLabelTab::OnChangeInfoSearchEdit)
	ON_EN_CHANGE(IDC_LABEL_SEARCH_EDIT, &CLabelTab::OnChangeLabelSearchEdit)
	ON_BN_CLICKED(IDC_LABEL_RELATE_BOOK, &CLabelTab::OnBnClickedLabelRelateBook)
	ON_BN_CLICKED(IDC_REMOVE_RELATIONSHIP, &CLabelTab::OnBnClickedRemoveRelationship)
	ON_BN_CLICKED(IDC_LABEL_RELATE_PROJECT, &CLabelTab::OnBnClickedLabelRelateProject)
END_MESSAGE_MAP()

void CLabelTab::LabelListEvent::InitListBox()
{
	auto pThis = ((CLabelTab*)((BYTE*)this - offsetof(CLabelTab, m_label_event)));

	CString strFolder = theSetting.GetLabelMagDir();
	pThis->m_label_list.DisplayFolder(strFolder);
}

void CLabelTab::LabelListEvent::OnSelectChanged()
{
	auto pThis = ((CLabelTab*)((BYTE*)this - offsetof(CLabelTab, m_label_event)));

	int nItem = pThis->m_label_list.GetCurSel();
	if (nItem >= 0)
	{
		CString strPath = pThis->m_label_list.GetItemPath(nItem);
		pThis->m_label_info.DisplayFolder(strPath);
	}
}

void CLabelTab::LabelInfoEvent::InitListBox()
{
	auto pThis = ((CLabelTab*)((BYTE*)this - offsetof(CLabelTab, m_label_info_event)));
	pThis->m_label_info.ResetContent();
}

void CLabelTab::LabelInfoEvent::OnDoubleClick()
{
	auto pThis = ((CLabelTab*)((BYTE*)this - offsetof(CLabelTab, m_label_info_event)));
	pThis->m_label_info.DoDefaultDClick(pThis->m_label_info.GetCurSel());
}

void CLabelTab::LabelInfoEvent::OnSelectChanged()
{
	auto pThis = ((CLabelTab*)((BYTE*)this - offsetof(CLabelTab, m_label_info_event)));
	int nItem = pThis->m_label_info.GetCurSel();
	if (nItem >= 0)
	{
		CString strLink = pThis->m_label_info.GetItemPath(nItem);

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

BOOL CLabelTab::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	return FALSE; /* 焦点设置 */
}

void CLabelTab::OnBnClickedLabelAdd()
{
	CNameDlg dlg;
	dlg.m_title = _T("新建标签");
	if (dlg.DoModal() == IDOK)
	{
		CString strFolder = theSetting.GetLabelMagDir();
		strFolder += _T("\\") + dlg.m_name;

		if (CreateDirectory(strFolder, nullptr)) {
			m_label_list.Refresh();
			m_label_info.DisplayFolder(strFolder);

			m_label_list.SetFocus();
			m_label_list.SelectString(0, dlg.m_name);
		} else {
			CString strContent = _T("创建标签\"\"失败！");
			strContent.Insert(5, strFolder);
			MessageBox(strContent, _T("错误"), MB_ICONERROR);
		}
	}
}

void CLabelTab::OnBnClickedLabelDelete()
{
	int nItem = m_label_list.GetCurSel();
	if (nItem >= 0) {	
		CString strFolder = m_label_list.GetItemPath(nItem);
		DeleteDirectory(strFolder);
		m_label_list.Refresh();
	} else {
		MessageBox(_T("未选中任何标签!"), _T("提示"), MB_ICONINFORMATION);
	}
}

void CLabelTab::OnBnClickedLabelRename()
{
	int nItem = m_label_list.GetCurSel();
	if (nItem >= 0) {
		CString strOldFolder = m_label_list.GetItemPath(nItem);

		int nFind = strOldFolder.ReverseFind('\\') + 1;
		CString strLabel = strOldFolder.Mid(nFind);

		CNameDlg dlg;
		dlg.m_name = strLabel;
		dlg.m_title = _T("标签重命名");

		if (dlg.DoModal() == IDOK)
		{
			CString strNewFolder = theSetting.GetLabelMagDir();
			strNewFolder += _T("\\") + dlg.m_name;
			CFile::Rename(strOldFolder, strNewFolder);

			m_label_list.Refresh();
			m_label_info.DisplayFolder(strNewFolder);

			m_label_list.SetFocus();
			m_label_list.SelectString(0, dlg.m_name);
		}

	} else {
		MessageBox(_T("未选中任何标签!"), _T("提示"), MB_ICONINFORMATION);
	}
}

void CLabelTab::OnBnClickedLabelRelateProject()
{
	int nItem = m_label_list.GetCurSel();
	if (nItem >= 0) {

		CProjectSelectDlg dlg;
		if (dlg.DoModal() == IDOK)
		{
			for (int i = 0; i < dlg.arrProject.GetCount(); i++)
			{
				CString strFolder = dlg.arrProject.GetAt(i);
				int nFind = strFolder.ReverseFind('\\') + 1;
				CString strLabel = strFolder.Mid(nFind);

				CString strLink = m_label_list.GetItemPath(nItem);
				strLink += _T("\\") + strLabel + _T(".lnk");
				
				if (FALSE == CreateFileLink(strFolder, strLink)) {
					CString strContent = _T("关联项目\"\"失败！");
					strContent.Insert(5, strFolder);
					MessageBox(strContent, _T("错误"), MB_ICONERROR);
				}
			}

			if (dlg.arrProject.GetCount() > 0) {
				m_label_info.Refresh();
			}
		}

	} else {
		MessageBox(_T("未选中任何标签!"), _T("提示"), MB_ICONINFORMATION);
	}
}

void CLabelTab::OnBnClickedLabelRelateBook()
{
	int nItem = m_label_list.GetCurSel();
	if (nItem >= 0)
	{
		CBookSelectDlg dlg;
		if (dlg.DoModal() == IDOK)
		{
			for (int i = 0; i < dlg.arrBook.GetCount(); i++)
			{
				CString strFolder = dlg.arrBook.GetAt(i);
				int nFind = strFolder.ReverseFind('\\') + 1;
				CString strLabel = strFolder.Mid(nFind);

				CString strLink = m_label_list.GetItemPath(nItem);
				strLink += _T("\\") + strLabel + _T(".lnk");

				if (FALSE == CreateFileLink(strFolder, strLink)) {
					CString strContent = _T("关联项目\"\"失败！");
					strContent.Insert(5, strFolder);
					MessageBox(strContent, _T("错误"), MB_ICONERROR);
				}
			}

			if (dlg.arrBook.GetCount() > 0) {
				m_label_info.Refresh();
			}
		}

	} else {
		MessageBox(_T("未选中任何标签!"), _T("提示"), MB_ICONINFORMATION);
	}
}

void CLabelTab::OnBnClickedRemoveRelationship()
{
	int nItem = m_label_info.GetCurSel();
	if (nItem >= 0) {
		DeleteFile(m_label_info.GetItemPath(nItem));
		m_label_info.Refresh();
	} else {
		MessageBox(_T("未选中任何标签!"), _T("提示"), MB_ICONINFORMATION);
	}
}

void CLabelTab::OnChangeLabelSearchEdit()
{
	CString strFilter;
	m_label_search_edit.GetWindowText(strFilter);
	m_label_list.SetFilterString(strFilter);
}

void CLabelTab::OnChangeInfoSearchEdit()
{
	CString strFilter;
	m_info_search_edit.GetWindowText(strFilter);
	m_label_info.SetFilterString(strFilter);
}

BOOL CLabelTab::PreTranslateMessage(MSG* pMsg)
{
	if (pMsg->message == WM_KEYDOWN)
	{
		CWnd* pFocusWnd = GetFocus();

		switch((UINT)pMsg->wParam)
		{
		case VK_ESCAPE:
		case VK_RETURN:
			{
				GetParent()->GetParent()->SendMessage(WM_CLOSE);
				return TRUE;
			}
			break;
		case VK_DELETE:
			{
				if (pFocusWnd == &m_label_info)
				{
					OnBnClickedRemoveRelationship();
				}

				if (pFocusWnd == &m_label_list)
				{
					OnBnClickedLabelDelete();
				}

				return TRUE;
			}
			break;
		case VK_F5:
			{
				m_label_list.Refresh();
				return TRUE;
			}
			break;
		case VK_F3:
			{
				m_label_search_edit.SetFocus();
				return TRUE;
			}
			break;
		case VK_F2:
			{
				if (pFocusWnd == &m_label_list)
				{
					OnBnClickedLabelRename();
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

void CLabelTab::OnShowWindow(BOOL bShow, UINT nStatus)
{
	if (bShow) {
		m_label_search_edit.SetFocus();
	}

	CDialogEx::OnShowWindow(bShow, nStatus);
}