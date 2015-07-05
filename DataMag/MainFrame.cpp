#include "stdafx.h"
#include "DataMag.h"
#include "BookTab.h"
#include "LabelTab.h"
#include "MainFrame.h"
#include "SettingDlg.h"
#include "ProjectTab.h"

IMPLEMENT_DYNAMIC(CMainFrame, CDialogEx)

CMainFrame::CMainFrame(CWnd* pParent /*=nullptr*/)
	: CDialogEx(CMainFrame::IDD, pParent)
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
END_MESSAGE_MAP()

UINT CMainFrame::MainTabAdapter::GetCount()
{
	return 3;
}

LPCTSTR CMainFrame::MainTabAdapter::GetItemText(UINT nItem)
{
	LPCTSTR strItems[] = {_T("标签"), _T("项目"), _T("图书")};
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

	CButton* pButton = (CButton*)GetDlgItem(IDC_SETTING);
	pButton->SetIcon(AfxGetApp()->LoadIcon(IDI_SETTING));

	m_main_tab.SelectTab(2);

	return TRUE;
}

BOOL CMainFrame::PreTranslateMessage(MSG* pMsg)
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

void CMainFrame::OnClickedSetting()
{
	CSettingDlg dlg;
	dlg.DoModal();
}