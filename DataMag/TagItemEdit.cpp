#include "stdafx.h"
#include "TagItemEdit.h"

IMPLEMENT_DYNAMIC(CTagItemEdit, CEdit)

CTagItemEdit::CTagItemEdit()
{
}

CTagItemEdit::~CTagItemEdit()
{
}

BEGIN_MESSAGE_MAP(CTagItemEdit, CEdit)
	ON_WM_CREATE()
	ON_WM_SETFOCUS()
	ON_WM_KILLFOCUS()
	ON_WM_ERASEBKGND()
END_MESSAGE_MAP()

int CTagItemEdit::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CEdit::OnCreate(lpCreateStruct) == -1)
		return -1;

	Init();

	return 0;
}

void CTagItemEdit::PreSubclassWindow()
{
	CEdit::PreSubclassWindow();

	Init();
}

void CTagItemEdit::Init()
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

BOOL CTagItemEdit::OnEraseBkgnd(CDC* pDC)
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

void CTagItemEdit::OnSetFocus(CWnd* pOldWnd)
{
	CEdit::OnSetFocus(pOldWnd);

	Invalidate(TRUE);
}

void CTagItemEdit::OnKillFocus(CWnd* pNewWnd)
{
	CEdit::OnKillFocus(pNewWnd);

	Invalidate(TRUE);
}