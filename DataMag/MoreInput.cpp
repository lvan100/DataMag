#include "stdafx.h"
#include "MoreInput.h"

IMPLEMENT_DYNAMIC(CMoreInput, CStatic)

CMoreInput::CMoreInput()
	: m_nMaxInputCount(1000)
	, m_nMoreInputCount(1000)
{
}

CMoreInput::~CMoreInput()
{
}

BEGIN_MESSAGE_MAP(CMoreInput, CStatic)
	ON_WM_PAINT()
END_MESSAGE_MAP()

void CMoreInput::SetMoreInputCount(int count)
{
	m_nMoreInputCount = count;
	RedrawWindow();
}

void CMoreInput::SetMaxInputCount(int count)
{
	m_nMaxInputCount = count;
}

void CMoreInput::OnPaint()
{
	CPaintDC dc(this);

	CRect rcClient;
	GetClientRect(rcClient);

	CString prefix = _T("还可以再输入");
	CString ending = _T("字");

	CString count;
	count.Format(_T("%d"), m_nMoreInputCount);

	CFont* pOldFont = dc.SelectObject(GetFont());

	CSize szPrefix = dc.GetTextExtent(prefix);
	CSize szEnding = dc.GetTextExtent(ending);
	CSize szCount = dc.GetTextExtent(count);
	
	CRect rcEnding(rcClient);
	rcEnding.right = rcClient.right - 4;
	rcEnding.left = rcEnding.right - szEnding.cx;

	CRect rcCount(rcClient);
	rcCount.right = rcEnding.left;
	rcCount.left = rcCount.right - szCount.cx;
	
	CRect rcPrefix(rcClient);
	rcPrefix.right = rcCount.left;
	rcPrefix.left = rcPrefix.right - szPrefix.cx;

	dc.FillSolidRect(rcClient, afxGlobalData.clrBtnFace);

	dc.SetBkMode(TRANSPARENT);
	dc.DrawText(prefix, rcPrefix, DT_VCENTER | DT_SINGLELINE);

	int nInputedCount = m_nMaxInputCount - m_nMoreInputCount;
	int nInputedPercent = (nInputedCount) * 100 / m_nMaxInputCount;

	COLORREF oldClr = dc.GetTextColor();
	
	if (nInputedPercent >= 90) {
		oldClr = dc.SetTextColor(RGB(255,0,0));
	} else if (nInputedPercent >= 80) {
		oldClr = dc.SetTextColor(RGB(255,215,0));
	}

	dc.DrawText(count, rcCount, DT_VCENTER | DT_SINGLELINE);

	dc.SetTextColor(oldClr);
	dc.DrawText(ending, rcEnding, DT_VCENTER | DT_SINGLELINE);

	dc.SelectObject(pOldFont);
}