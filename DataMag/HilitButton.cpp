#include "StdAfx.h"
#include "HilitButton.h"

IMPLEMENT_DYNAMIC(CHilitButton, CMFCButton)

CHilitButton::CHilitButton()
{
	m_bDrawFocus = FALSE;
}

CHilitButton::~CHilitButton()
{
}

BEGIN_MESSAGE_MAP(CHilitButton, CMFCButton)
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