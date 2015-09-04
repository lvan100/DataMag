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
END_MESSAGE_MAP()

BOOL CAppWnd::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	SetIcon(m_hIcon, TRUE);
	SetIcon(m_hIcon, FALSE);

	return TRUE;
}

void CAppWnd::OnActivate(UINT nState, CWnd* pWndOther, BOOL bMinimized)
{
	CDialogEx::OnActivate(nState, pWndOther, bMinimized);

	if (nState == WA_INACTIVE ) {
		m_pLastFocusWnd = GetFocus();
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