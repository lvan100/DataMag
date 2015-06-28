#include "stdafx.h"
#include "DataMag.h"
#include "BookTab.h"
#include "SettingDlg.h"

IMPLEMENT_DYNAMIC(CBookTab, CDialogEx)

CBookTab::CBookTab(CWnd* pParent /*=NULL*/)
	: CDialogEx(CBookTab::IDD, pParent)
{
	m_book_list.SetListEvent(this);
}

CBookTab::~CBookTab()
{
}

void CBookTab::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_BOOK_LIST, m_book_list);
	DDX_Control(pDX, IDC_BOOK_SEARCH_EDIT, m_book_search_edit);
}

BEGIN_MESSAGE_MAP(CBookTab, CDialogEx)
	ON_EN_CHANGE(IDC_BOOK_SEARCH_EDIT, &CBookTab::OnChangeBookSearchEdit)
END_MESSAGE_MAP()

void CBookTab::InitShellList()
{
	CString strFolder = theSetting.strMagFolder;
	strFolder += _T("\\");
	strFolder += BOOK_DIR;
	m_book_list.DisplayFolder(strFolder);
}

void CBookTab::OnDoubleClick()
{
	m_book_list.DoDefaultDClick();
}

void CBookTab::OnSelectChanged()
{

}

void CBookTab::OnChangeBookSearchEdit()
{
	CString strFilter;
	m_book_search_edit.GetWindowText(strFilter);
	m_book_list.SetFilterString(strFilter);
}

BOOL CBookTab::PreTranslateMessage(MSG* pMsg)
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