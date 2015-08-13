#include "stdafx.h"
#include "SearchEdit.h"

IMPLEMENT_DYNAMIC(CSearchEdit, CEdit)

CSearchEdit::CSearchEdit()
	: m_bFocused(FALSE)
	, m_bTextIsHint(FALSE)
{
}

CSearchEdit::~CSearchEdit()
{
}

BEGIN_MESSAGE_MAP(CSearchEdit, CEdit)
	ON_WM_ERASEBKGND()
	ON_WM_SETFOCUS()
	ON_WM_KILLFOCUS()
	ON_WM_CTLCOLOR_REFLECT()
END_MESSAGE_MAP()

/**
 * 获取图标的大小
 *
 * @param hIcon
 *        图标
 * @return 返回图标的大小
 */
static CSize GetIconSize(HICON hIcon)
{
	if (hIcon == nullptr)
	{
		return CSize(0, 0);
	}

	ICONINFO iconInfo; 
	GetIconInfo(hIcon, &iconInfo);

	BITMAP bitmap; 
	GetObject(iconInfo.hbmColor, sizeof(BITMAP), &bitmap); 

	DeleteObject(iconInfo.hbmColor);
	DeleteObject(iconInfo.hbmMask);

	return CSize(bitmap.bmWidth, bitmap.bmHeight);
}

void CSearchEdit::Init()
{
	CPrettyEdit::Init();

	CRect rc;
	GetRect(rc);

	CSize size = GetIconSize(m_search_icon);
	rc.right -= size.cx + rc.left * 2;

	SetRectNP(&rc);
}

BOOL CSearchEdit::OnEraseBkgnd(CDC* pDC)
{
	CRect rcClient;
	GetClientRect(rcClient);

	CBrush whiteBrush(RGB(255,255,255));
	pDC->FillRect(rcClient, &whiteBrush);

	if (GetFocus() == this) {
		pDC->FrameRect(rcClient, &afxGlobalData.brHilite);
	}

	CSize size = GetIconSize(m_search_icon);

	DrawIconEx(pDC->GetSafeHdc()
		, rcClient.right - size.cx - 2, (rcClient.Height() - size.cy + 1) / 2
		, m_search_icon, size.cx, size.cy
		, 0, nullptr, DI_NORMAL);

	return TRUE;
}

void CSearchEdit::OnSetFocus(CWnd* pOldWnd)
{
	CPrettyEdit::OnSetFocus(pOldWnd);

	m_bFocused = TRUE;
	InvalidateWithHint();
}

void CSearchEdit::OnKillFocus(CWnd* pNewWnd)
{
	CPrettyEdit::OnKillFocus(pNewWnd);

	m_bFocused = FALSE;
	InvalidateWithHint();
}

void CSearchEdit::InvalidateWithHint()
{
	ASSERT(::IsWindow(m_hWnd));

	if (m_bFocused) {
		if (m_bTextIsHint) {
			SetWindowText(_T(""));
			m_bTextIsHint = FALSE;
		}

	} else {
		CString strText;
		GetWindowText(strText);

		if (strText.IsEmpty()) {
			SetWindowText(m_strHint);
			m_bTextIsHint = TRUE;
		}
	}

	Invalidate(TRUE);
}

HBRUSH CSearchEdit::CtlColor(CDC* pDC, UINT nCtlColor)
{
	if (m_bTextIsHint) {
		pDC->SetTextColor(RGB(210,210,210));
	} else {
		pDC->SetTextColor(RGB(0,0,0));
	}

	return GetSysColorBrush(0);
}