#include "stdafx.h"
#include "DataMag.h"
#include "SettingDlg.h"
#include "ProjectTab.h"

IMPLEMENT_DYNAMIC(CProjectTab, CDialogEx)

CProjectTab::CProjectTab(CWnd* pParent /*=NULL*/)
	: CDialogEx(CProjectTab::IDD, pParent)
{
	m_project_list.SetListEvent(this);
}

CProjectTab::~CProjectTab()
{
}

void CProjectTab::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_PROJECT_LIST, m_project_list);
	DDX_Control(pDX, IDC_PROJECT_SEARCH_EDIT, m_project_search_edit);
}

BEGIN_MESSAGE_MAP(CProjectTab, CDialogEx)
	ON_EN_CHANGE(IDC_PROJECT_SEARCH_EDIT, &CProjectTab::OnChangeProjectSearchEdit)
END_MESSAGE_MAP()

void CProjectTab::InitShellList()
{
	CString strFolder = theSetting.strMagFolder;
	strFolder += _T("\\");
	strFolder += PROJECT_DIR;
	m_project_list.DisplayFolder(strFolder);
}

void CProjectTab::OnDoubleClick()
{
	m_project_list.DoDefaultDClick();
}

void CProjectTab::OnSelectChanged()
{

}

void CProjectTab::OnChangeProjectSearchEdit()
{
	CString strFilter;
	m_project_search_edit.GetWindowText(strFilter);
	m_project_list.SetFilterString(strFilter);
}

BOOL CProjectTab::PreTranslateMessage(MSG* pMsg)
{
	if (pMsg->message == WM_KEYDOWN)
	{
		switch((UINT)pMsg->wParam)
		{
		case VK_RETURN:
			{
				return TRUE;
			}
			break;
		default:
			break;
		}
	}

	return CDialogEx::PreTranslateMessage(pMsg);
}