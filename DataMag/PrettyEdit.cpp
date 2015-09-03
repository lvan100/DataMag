#include "stdafx.h"
#include "PrettyEdit.h"

IMPLEMENT_DYNAMIC(CPrettyEdit, CEdit)

CPrettyEdit::CPrettyEdit()
	: m_bFocused(FALSE)
{
}

CPrettyEdit::~CPrettyEdit()
{
}

BEGIN_MESSAGE_MAP(CPrettyEdit, CEdit)
	ON_WM_CREATE()
	ON_WM_SETFOCUS()
	ON_WM_KILLFOCUS()
	ON_WM_ERASEBKGND()
END_MESSAGE_MAP()

int CPrettyEdit::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CEdit::OnCreate(lpCreateStruct) == -1)
		return -1;

	Init();

	return 0;
}

void CPrettyEdit::PreSubclassWindow()
{
	CEdit::PreSubclassWindow();

	Init();
}

void CPrettyEdit::Init()
{
	CRect rc; 
	GetClientRect(&rc);

	CDC* pDC = GetDC();

	TEXTMETRIC tm;
	pDC->GetTextMetrics(&tm);

	int nFontHeight = tm.tmHeight + tm.tmExternalLeading;
	int offY = (rc.Height() - nFontHeight) / 2 + 1;
	rc.OffsetRect(5,offY);
	rc.right -= 11;
	SetRectNP(&rc);

	ReleaseDC(pDC);
}

BOOL CPrettyEdit::OnEraseBkgnd(CDC* pDC)
{
	CRect rcClient;
	GetClientRect(rcClient);

	CBrush whiteBrush(RGB(255,255,255));
	pDC->FillRect(rcClient, &whiteBrush);

	if (GetFocus() == this) {
		pDC->FrameRect(rcClient, &afxGlobalData.brHilite);
	}

	return TRUE;
}

void CPrettyEdit::OnSetFocus(CWnd* pOldWnd)
{
	CEdit::OnSetFocus(pOldWnd);

	m_bFocused = TRUE;
	Invalidate(TRUE);
}

void CPrettyEdit::OnKillFocus(CWnd* pNewWnd)
{
	CEdit::OnKillFocus(pNewWnd);

	m_bFocused = FALSE;
	Invalidate(TRUE);
}