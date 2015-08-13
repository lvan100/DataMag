#include "stdafx.h"
#include "DataMag.h"
#include "AppWnd.h"

IMPLEMENT_DYNAMIC(CAppWnd, CDialogEx)

CAppWnd::CAppWnd(UINT nIDTemplate, CWnd* pParent)
	: CDialogEx(nIDTemplate, pParent)
	, m_pLastFocusWnd(nullptr)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

CAppWnd::~CAppWnd()
{
}

void CAppWnd::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAppWnd, CDialogEx)
	ON_WM_ACTIVATE()
	ON_WM_SYSCOMMAND()
END_MESSAGE_MAP()

BOOL CAppWnd::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	SetIcon(m_hIcon, TRUE);
	SetIcon(m_hIcon, FALSE);

	return TRUE;
}

void CAppWnd::OnSysCommand(UINT nID, LPARAM lParam)
{
	if (nID == SC_MINIMIZE) {
		m_pLastFocusWnd = GetFocus();
	}

	CDialogEx::OnSysCommand(nID, lParam);
}

void CAppWnd::OnActivate(UINT nState, CWnd* pWndOther, BOOL bMinimized)
{
	CDialogEx::OnActivate(nState, pWndOther, bMinimized);

	if (nState == WA_INACTIVE ) {
		if (!bMinimized) {
			m_pLastFocusWnd = GetFocus();
		}
	} else {
		if (m_pLastFocusWnd != nullptr) {
			m_pLastFocusWnd->SetFocus();
		} else {
			CWnd* pWnd = GetDefaultFocusWnd();
			if (pWnd != NULL) {
				pWnd->SetFocus();
			}
		}
	}
}