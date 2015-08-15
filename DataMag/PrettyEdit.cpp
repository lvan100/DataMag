#include "stdafx.h"
#include "PrettyEdit.h"

IMPLEMENT_DYNAMIC(CPrettyEdit, CEdit)

CPrettyEdit::CPrettyEdit()
{
}

CPrettyEdit::~CPrettyEdit()
{
}

BEGIN_MESSAGE_MAP(CPrettyEdit, CEdit)
	ON_WM_CREATE()
END_MESSAGE_MAP()

int CPrettyEdit::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CEdit::OnCreate(lpCreateStruct) == -1)
		return -1;

	Init();

	return 0;
}

void CPrettyEdit::PreSubclassWindow()
{
	CEdit::PreSubclassWindow();

	Init();
}

void CPrettyEdit::Init()
{
	CRect rc; 
	GetClientRect(&rc);

	CDC* pDC = GetDC();
	TEXTMETRIC tm;
	pDC->GetTextMetrics(&tm);

	int nFontHeight = tm.tmHeight + tm.tmExternalLeading;
	int offY = (rc.Height() - nFontHeight) / 2 + 1;
	rc.OffsetRect(5,offY);
	rc.right -= 11;
	SetRectNP(&rc);

	ReleaseDC(pDC);
}