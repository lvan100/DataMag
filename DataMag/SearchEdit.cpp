#include "stdafx.h"
#include "SearchEdit.h"

IMPLEMENT_DYNAMIC(CSearchEdit, CEdit)

CSearchEdit::CSearchEdit()
	: m_bMouseHover(FALSE)
	, m_bTextIsHint(FALSE)
	, m_bEnableSearchBtn(TRUE)
{
}

CSearchEdit::~CSearchEdit()
{
}

BEGIN_MESSAGE_MAP(CSearchEdit, CEdit)
	ON_WM_SETFOCUS()
	ON_WM_KILLFOCUS()
	ON_WM_ERASEBKGND()
	ON_WM_CTLCOLOR_REFLECT()
	ON_WM_LBUTTONDOWN()
	ON_WM_SETCURSOR()
	ON_WM_MOUSEMOVE()
	ON_WM_SIZE()
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
	if (hIcon == nullptr) {
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

CRect CSearchEdit::GetSearchBtnRect()
{
	CRect rcClient;
	GetClientRect(rcClient);

	rcClient.left = rcClient.right;
	rcClient.left -= rcClient.Height();

	return rcClient;
}

BOOL CSearchEdit::HitSearchButton()
{
	CPoint prCurPos;
	GetCursorPos(&prCurPos);
	ScreenToClient(&prCurPos);
	return GetSearchBtnRect().PtInRect(prCurPos);
}

void CSearchEdit::PositionNPRect()
{
	CRect rcClient;
	GetClientRect(rcClient);

	CDC* pDC = GetDC();

	TEXTMETRIC tm;
	pDC->GetTextMetrics(&tm);

	int nFontHeight = tm.tmHeight + tm.tmExternalLeading;
	int offY = (rcClient.Height() - nFontHeight) / 2 + 1;

	CRect rcNP;
	rcNP.left = rcClient.left + 5;
	rcNP.top = rcClient.top + offY;
	rcNP.bottom = rcNP.top + nFontHeight;
	rcNP.right = rcClient.right - (5 + rcClient.Height());
	SetRectNP(&rcNP);

	ReleaseDC(pDC);
}

void CSearchEdit::Init()
{
	PositionNPRect();
}

BOOL CSearchEdit::OnEraseBkgnd(CDC* pDC)
{
	CRect rcClient;
	GetClientRect(rcClient);

	CBrush whiteBrush(RGB(255,255,255));
	pDC->FillRect(rcClient, &whiteBrush);

	pDC->FrameRect(rcClient, &afxGlobalData.brHilite);

	CRect rcSearchBtn = GetSearchBtnRect();

	if (m_bEnableSearchBtn && m_bMouseHover) {

		if (m_bFocused) {
			rcSearchBtn.top += 1;
			rcSearchBtn.right -= 1;
			rcSearchBtn.bottom -= 1;
		}

		pDC->FillRect(rcSearchBtn, CBrush::FromHandle(afxGlobalData.hbrBtnShadow));

		if (m_bFocused) {
			rcSearchBtn.top -= 1;
			rcSearchBtn.right += 1;
			rcSearchBtn.bottom += 1;
		}
	} 

	CSize size = GetIconSize(m_search_icon);

	rcSearchBtn.left += (rcSearchBtn.Width() - size.cx) / 2;
	rcSearchBtn.top += (rcClient.Height() - size.cy) / 2;
	rcSearchBtn.right = rcSearchBtn.left + size.cx ;
	rcSearchBtn.bottom = rcSearchBtn.top + size.cy ;

	DrawIconEx(pDC->GetSafeHdc(), rcSearchBtn.left , rcSearchBtn.top
		, m_search_icon, rcSearchBtn.Width(), rcSearchBtn.Height()
		, 0, nullptr, DI_NORMAL);

	return TRUE;
}

void CSearchEdit::OnSetFocus(CWnd* pOldWnd)
{
	CEdit::OnSetFocus(pOldWnd);

	m_bFocused = TRUE;
	InvalidateWithHint();
}

void CSearchEdit::OnKillFocus(CWnd* pNewWnd)
{
	CEdit::OnKillFocus(pNewWnd);

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

BOOL CSearchEdit::OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message)
{
	if (m_bEnableSearchBtn) {
		if (HitSearchButton()) {
			SetCursor(::LoadCursor(nullptr, IDC_ARROW));
			return TRUE;
		}
	}

	return CPrettyEdit::OnSetCursor(pWnd, nHitTest, message);
}

void CSearchEdit::OnMouseMove(UINT nFlags, CPoint point)
{
	if (m_bEnableSearchBtn) {

		CRect rcSearchBtn = GetSearchBtnRect();

		if (rcSearchBtn.PtInRect(point)) {
			if (!m_bMouseHover) {
				this->SetCapture();
				m_bMouseHover = TRUE;
				InvalidateRect(rcSearchBtn);
			}
		} else {
			if (m_bMouseHover) {
				ReleaseCapture();
				m_bMouseHover = FALSE;
				InvalidateRect(rcSearchBtn);
			}
		}
	}

	CPrettyEdit::OnMouseMove(nFlags, point);
}

void CSearchEdit::OnLButtonDown(UINT nFlags, CPoint point)
{
	if (m_bEnableSearchBtn) {
		ReleaseCapture();
		m_bMouseHover = FALSE;
	}

	CPrettyEdit::OnLButtonDown(nFlags, point);

	if (m_bEnableSearchBtn)
	{
		CPrettyEdit::OnLButtonUp(nFlags, point);

		if (HitSearchButton()) {
			if (m_click_event) {
				m_click_event();
			}
		}
	}
}

void CSearchEdit::OnSize(UINT nType, int cx, int cy)
{
	CPrettyEdit::OnSize(nType, cx, cy);

	PositionNPRect();
}