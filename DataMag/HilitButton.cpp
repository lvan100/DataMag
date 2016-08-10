#include "StdAfx.h"
#include "HilitButton.h"

static const UINT DelayClickTimer = 404;

IMPLEMENT_DYNAMIC(CHilitButton, CMFCButton)

CHilitButton::CHilitButton()
	: m_bWasDoublelClick(FALSE)
	, m_bEnableDoubleClick(FALSE)
{
	m_bDrawFocus = FALSE;
}

CHilitButton::~CHilitButton()
{
}

BEGIN_MESSAGE_MAP(CHilitButton, CMFCButton)
	ON_WM_LBUTTONDBLCLK()
	ON_WM_LBUTTONDOWN()
	ON_WM_TIMER()
END_MESSAGE_MAP()

void CHilitButton::OnFillBackground(CDC* pDC, const CRect& rectClient)
{
	CMFCButton::OnFillBackground(pDC, rectClient);

	if (m_bHighlighted || (GetFocus() == this)) {
		pDC->FrameRect(rectClient, &afxGlobalData.brHilite);
	}
}

void CHilitButton::PreSubclassWindow()
{
	CMFCButton::PreSubclassWindow();
}

void CHilitButton::OnLButtonDown(UINT nFlags, CPoint point)
{
	CMFCButton::OnLButtonDown(nFlags, point);

	if (!m_bEnableDoubleClick && onClicked) {
		onClicked();
	}

	if (m_bEnableDoubleClick) {
		SetTimer(DelayClickTimer, 150, NULL);
	}
}

void CHilitButton::OnLButtonDblClk(UINT nFlags, CPoint point)
{
	CMFCButton::OnLButtonDblClk(nFlags, point);

	if (m_bEnableDoubleClick && onDoubleClicked) {
		onDoubleClicked();
	}

	m_bWasDoublelClick = TRUE;
}

void CHilitButton::OnTimer(UINT_PTR nIDEvent)
{
	if (nIDEvent == DelayClickTimer) {
		KillTimer(nIDEvent); // É¾³ý¼ÆÊ±Æ÷

		if (m_bWasDoublelClick) {
			m_bWasDoublelClick = FALSE;

		} else{
			if (onClicked) {
				onClicked();
			}
		}
	}

	CMFCButton::OnTimer(nIDEvent);
}