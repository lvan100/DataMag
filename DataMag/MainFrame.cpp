#include "stdafx.h"
#include "DataMag.h"
#include "BookTab.h"
#include "LabelTab.h"
#include "MainFrame.h"
#include "SettingDlg.h"
#include "ProjectTab.h"

IMPLEMENT_DYNAMIC(CMainFrame, CDialogEx)

CMainFrame::CMainFrame(MFCommand c, CString a, CWnd* pParent /*=nullptr*/)
	: CDialogEx(CMainFrame::IDD, pParent)
	, cmd(c)
	, arg(a)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);

	m_main_tab.SetAdapter(&m_main_tab_adapter);
}

CMainFrame::~CMainFrame()
{
}

void CMainFrame::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_MAIN_TAB, m_main_tab);
}

BEGIN_MESSAGE_MAP(CMainFrame, CDialogEx)
	ON_BN_CLICKED(IDC_SETTING, &CMainFrame::OnClickedSetting)
	ON_WM_ACTIVATEAPP()
	ON_WM_SYSCOMMAND()
END_MESSAGE_MAP()

UINT CMainFrame::MainTabAdapter::GetCount()
{
	return 3;
}

LPCTSTR CMainFrame::MainTabAdapter::GetItemText(UINT nItem)
{
	static LPCTSTR strItems[] = {_T("标签管理"), _T("项目管理"), _T("图书管理")};
	return strItems[nItem];
}

CWnd* CMainFrame::MainTabAdapter::GetItem(UINT nItem, CWnd* parent)
{
	switch (nItem)
	{
	case 0:
		{
			CLabelTab* pItem = new CLabelTab();
			if (pItem->Create(IDD_LABELTAB, parent))
			{
				return pItem;
			}
		}
		break;
	case 1:
		{
			CProjectTab* pItem = new CProjectTab();
			if (pItem->Create(IDD_PROJECTTAB, parent))
			{
				return pItem;
			}
		}
		break;
	case 2:
		{
			CBookTab* pItem = new CBookTab();
			if (pItem->Create(IDD_BOOKTAB, parent))
			{
				return pItem;
			}
		}
		break;
	default:
		break;
	}

	return nullptr;
}

BOOL CMainFrame::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	
	SetIcon(m_hIcon, TRUE);
	SetIcon(m_hIcon, FALSE);

	switch (cmd) {
	case OpenLabel:
		{
			m_main_tab.SelectTab(0);
		}
		break;
	case SearchLabel:
		{
			CWnd* label = m_main_tab.SelectTab(0);
			((CLabelTab*)label)->SetLabelSearch(arg);
		}
		break;
	case OpenProject:
		{
			m_main_tab.SelectTab(1);
		}
		break;
	case SearchProject:
		{
			CWnd* label = m_main_tab.SelectTab(1);
			((CProjectTab*)label)->SetProjectSearch(arg);
		}
		break;
	case OpenBook:
		{
			m_main_tab.SelectTab(2);
		}
		break;
	case SearchBook:
		{
			CWnd* label = m_main_tab.SelectTab(2);
			((CBookTab*)label)->SetBookSearch(arg);
		}
		break;
	}

	return FALSE;
}

void CMainFrame::OnActivateApp(BOOL bActive, DWORD dwThreadID)
{
	CDialogEx::OnActivateApp(bActive, dwThreadID);

	if (bActive) {
		m_main_tab.SetFocus();
	}
}

void CMainFrame::OnClickedSetting()
{
	CSettingDlg().DoModal();
}

void CMainFrame::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & SC_MOVE) == SC_MOVE) {
		// 禁止窗口移动
	} else {
		CDialogEx::OnSysCommand(nID, lParam);
	}
}