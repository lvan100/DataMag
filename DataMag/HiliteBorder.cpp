#include "stdafx.h"
#include "HiliteBorder.h"

IMPLEMENT_DYNAMIC(CHiliteBorder, CStatic)

CHiliteBorder::CHiliteBorder()
	: m_bHilited(FALSE)
{
	m_gray_brush.CreateSolidBrush(RGB(220, 220, 220));
}

CHiliteBorder::~CHiliteBorder()
{
}

BEGIN_MESSAGE_MAP(CHiliteBorder, CStatic)
	ON_WM_ERASEBKGND()
END_MESSAGE_MAP()

BOOL CHiliteBorder::OnEraseBkgnd(CDC* pDC)
{
	CRect rcClient;
	GetClientRect(rcClient);

	// 为了边框刚好合适
	rcClient.bottom -= 1;

	pDC->SetBkMode(TRANSPARENT);

	if (m_bHilited) {
		pDC->FrameRect(rcClient, &afxGlobalData.brBtnFace);
		pDC->DrawFocusRect(rcClient); /* 绘制焦点区域 */
	} else {
 		pDC->FrameRect(rcClient, &m_gray_brush);
	}

	return TRUE;
}

void CHiliteBorder::DrawItem(LPDRAWITEMSTRUCT)
{
}