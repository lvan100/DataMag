#include "stdafx.h"
#include "DataMag.h"
#include "AppWnd.h"

IMPLEMENT_DYNAMIC(CAppWnd, CDialogEx)

CAppWnd::CAppWnd(UINT nIDTemplate, CWnd* pParent)
	: CDialogEx(nIDTemplate, pParent)
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
END_MESSAGE_MAP()

BOOL CAppWnd::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	SetIcon(m_hIcon, TRUE);
	SetIcon(m_hIcon, FALSE);

	return TRUE;
}