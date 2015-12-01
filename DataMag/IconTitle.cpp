#include "stdafx.h"
#include "IconTitle.h"

IMPLEMENT_DYNAMIC(CIconTitle, CStatic)

CIconTitle::CIconTitle()
{
}

CIconTitle::~CIconTitle()
{
}

BEGIN_MESSAGE_MAP(CIconTitle, CStatic)
	ON_WM_PAINT()
END_MESSAGE_MAP()

void CIconTitle::OnPaint()
{
	CPaintDC dc(this);

	CRect rcClient;
	GetClientRect(rcClient);

	CRect rcIcon(rcClient);
	rcIcon.DeflateRect(2, 2, 2, 2);
	rcIcon.right = rcIcon.left + rcIcon.Height();

	DrawIconEx(dc.GetSafeHdc(), rcIcon.left, rcIcon.top
		, m_hIcon, rcIcon.Width(), rcIcon.Height()
		, 0, nullptr, DI_NORMAL);

	CRect rcText(rcClient);
	rcText.left += rcText.Height() + 2;

	CString str;
	GetWindowText(str);
	dc.SetBkMode(TRANSPARENT);
	dc.DrawText(str, rcText, DT_LEFT | DT_VCENTER | DT_SINGLELINE);
}