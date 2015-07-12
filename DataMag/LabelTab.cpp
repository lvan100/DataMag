#include "stdafx.h"
#include "FileOp.h"
#include "DataMag.h"
#include "LabelTab.h"
#include "SettingDlg.h"
#include "NameDlg.h"
#include "BookSelectDlg.h"
#include "ProjectSelectDlg.h"

IMPLEMENT_DYNAMIC(CLabelTab, CDialogEx)

CLabelTab::CLabelTab(CWnd* pParent /*=NULL*/)
	: CDialogEx(CLabelTab::IDD, pParent)
{	
	m_label_list.SetListEvent(&m_label_event);
	m_label_info.SetListEvent(&m_label_info_event);

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
	ON_BN_CLICKED(IDC_LABEL_RELATE_PROJECT, &CLabelTab::OnBnClickedLabelRelateProject)
END_MESSAGE_MAP()

void CLabelTab::LabelListEvent::InitShellList()
{
	auto pThis = ((CLabelTab*)((BYTE*)this - offsetof(CLabelTab, m_label_event)));

	CString strFolder = theSetting.GetCodeMagDir();
	pThis->m_label_list.DisplayFolder(strFolder);
}

void CLabelTab::LabelListEvent::OnSelectChanged()
{
	auto pThis = ((CLabelTab*)((BYTE*)this - offsetof(CLabelTab, m_label_event)));

	POSITION pos = pThis->m_label_list.GetFirstSelectedItemPosition();
	int nItem = pThis->m_label_list.GetNextSelectedItem(pos);
	if (nItem >= 0)
	{
		CString strPath = pThis->m_label_list.GetItemPath(nItem);
		pThis->m_label_info.DisplayFolder(strPath);
	}
}

void CLabelTab::LabelInfoEvent::InitShellList()
{
	auto pThis = ((CLabelTab*)((BYTE*)this - offsetof(CLabelTab, m_label_info_event)));
	pThis->m_label_info.DeleteAllItems();
}

void CLabelTab::LabelInfoEvent::OnDoubleClick()
{
	auto pThis = ((CLabelTab*)((BYTE*)this - offsetof(CLabelTab, m_label_info_event)));
	pThis->m_label_info.DoDefaultDClick();
}

BOOL CLabelTab::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	return TRUE;
}

void CLabelTab::OnBnClickedLabelAdd()
{
	CNameDlg dlg;
	if (dlg.DoModal() == IDOK)
	{
		CString strFolder = theSetting.GetCodeMagDir();
		strFolder += dlg.m_name;

		CreateDirectory(strFolder, nullptr);
	}

	m_label_list.Refresh();
}

void CLabelTab::OnBnClickedLabelDelete()
{
	POSITION pos = m_label_list.GetFirstSelectedItemPosition();
	int nItem = m_label_list.GetNextSelectedItem(pos);

	if (nItem >= 0)
	{	
		CString strFolder = m_label_list.GetItemPath(nItem);
		DeleteDirectory(strFolder);
	}

	m_label_list.Refresh();
}

void CLabelTab::OnBnClickedLabelRename()
{
	POSITION pos = m_label_list.GetFirstSelectedItemPosition();
	int nItem = m_label_list.GetNextSelectedItem(pos);

	if (nItem >= 0)
	{
		CString strOldFolder = m_label_list.GetItemPath(nItem);

		int nFind = strOldFolder.ReverseFind('\\') + 1;
		CString strLabel = strOldFolder.Mid(nFind);

		CNameDlg dlg;
		dlg.m_name = strLabel;
		dlg.Op = CNameDlg::Rename;

		if (dlg.DoModal() == IDOK)
		{
			CString strNewFolder = theSetting.GetCodeMagDir();
			CFile::Rename(strOldFolder, strNewFolder);
		}
	}

	m_label_list.Refresh();
}

void CLabelTab::OnBnClickedLabelRelateProject()
{
	POSITION pos = m_label_list.GetFirstSelectedItemPosition();
	int nItem = m_label_list.GetNextSelectedItem(pos);
	if (nItem >= 0)
	{
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
				CreateFileLink(strFolder, strLink);
			}
		}

		m_label_info.Refresh();
	}
}

void CLabelTab::OnBnClickedLabelRelateBook()
{
	POSITION pos = m_label_list.GetFirstSelectedItemPosition();
	int nItem = m_label_list.GetNextSelectedItem(pos);
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
				CreateFileLink(strFolder, strLink);
			}
		}

		m_label_info.Refresh();
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
		case VK_RETURN:
			{
				return TRUE;
			}
			break;
		case VK_DELETE:
			{
				if (pFocusWnd == &m_label_info)
				{
					POSITION pos = m_label_info.GetFirstSelectedItemPosition();
					int nItem = m_label_info.GetNextSelectedItem(pos);
					if (nItem >= 0)
					{
						DeleteFile(m_label_info.GetItemPath(nItem));
					}

					m_label_info.Refresh();
				}

				if (pFocusWnd == &m_label_list)
				{
					OnBnClickedLabelDelete();
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