#include "stdafx.h"
#include "HiliteBorder.h"

IMPLEMENT_DYNAMIC(CHiliteBorder, CStatic)

CHiliteBorder::CHiliteBorder()
	: m_bHilited(FALSE)
{
	grayBrush.CreateSolidBrush(RGB(220, 220, 220));
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

	pDC->SetBkMode(TRANSPARENT);

	if (m_bHilited) {
		pDC->FrameRect(rcClient, &afxGlobalData.brHilite);
	} else {
 		pDC->FrameRect(rcClient, &grayBrush);
	}

	return TRUE;
}

void CHiliteBorder::DrawItem(LPDRAWITEMSTRUCT)
{
}