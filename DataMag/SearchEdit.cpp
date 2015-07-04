#include "stdafx.h"
#include "SearchEdit.h"

IMPLEMENT_DYNAMIC(CSearchEdit, CEdit)

CSearchEdit::CSearchEdit()
{
}

CSearchEdit::~CSearchEdit()
{
}

BEGIN_MESSAGE_MAP(CSearchEdit, CEdit)
	ON_WM_ERASEBKGND()
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
	if (hIcon == NULL)
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
	CRect rc;
	GetRect(rc);

	CRect rcClient;
	GetClientRect(rcClient);

	CBrush whiteBrush(RGB(255,255,255));
	pDC->FillRect(rcClient, &whiteBrush);

	CSize size = GetIconSize(m_search_icon);

	DrawIconEx(pDC->GetSafeHdc()
		, rcClient.right - size.cx - 2, (rcClient.Height() - size.cy + 1) / 2
		, m_search_icon, size.cx, size.cy
		, 0, NULL, DI_NORMAL);

	return TRUE;
}