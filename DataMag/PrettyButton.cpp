#include "stdafx.h"
#include "PrettyButton.h"

IMPLEMENT_DYNAMIC(CPrettyButton, CHilitButton)

CPrettyButton::CPrettyButton()
{
}

CPrettyButton::~CPrettyButton()
{
}

BEGIN_MESSAGE_MAP(CPrettyButton, CHilitButton)
END_MESSAGE_MAP()

static int GetConstTextMargin() { return 4; }

void CPrettyButton::OnDrawText(CDC* pDC, const CRect& rect, const CString& strText, UINT uiDTFlags, UINT /*uiState*/)
{
	ASSERT_VALID(pDC);

	CRect rectTextUp(rect), rectTextDown(rect);

	rectTextUp.left += GetConstTextMargin();
	rectTextUp.bottom -= rect.Height() / 2 - GetConstTextMargin();

	rectTextDown.left += GetConstTextMargin();
	rectTextDown.top += rect.Height() / 2 + GetConstTextMargin();
	
	CString strTitle, strContent;

	int nLineBreak = strText.Find('\n');
	if (nLineBreak > 0) {
		strTitle = strText.Left(nLineBreak);
		strContent = strText.Mid(nLineBreak + 1);
	} else {
		strTitle = strContent = strText;
	}

	static CFont titleFont, conFont;
	
	if (titleFont.GetSafeHandle() == nullptr) {

		LOGFONT logFont = { 0 };
		afxGlobalData.fontBold.GetLogFont(&logFont);

		logFont.lfHeight = -20;
		titleFont.CreateFontIndirect(&logFont);
	}

	if (conFont.GetSafeHandle() == nullptr) {

		LOGFONT logFont = { 0 };
		afxGlobalData.fontRegular.GetLogFont(&logFont);

		logFont.lfHeight = -14;
		conFont.CreateFontIndirect(&logFont);
	}

	CFont* pOldFont = pDC->SelectObject(&titleFont);
	pDC->DrawTextEx(strTitle, rectTextUp, DT_LEFT | DT_BOTTOM | DT_SINGLELINE, nullptr);

	pOldFont = pDC->SelectObject(&conFont);
	pDC->DrawTextEx(strContent, rectTextDown, DT_LEFT | DT_TOP | DT_SINGLELINE, nullptr);

	pDC->SelectObject(&pOldFont);
}