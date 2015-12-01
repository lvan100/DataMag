#include "stdafx.h"
#include "ResourceSet.h"

/**
* 全局唯一的资源集合
*/
CResourceSet theResourceSet;

CResourceSet::CResourceSet()
{
}

CResourceSet::~CResourceSet()
{
}

float CResourceSet::GetScreenScale()
{
	HDC hDC = GetDC(GetDesktopWindow());
	int dpi = GetDeviceCaps(hDC, LOGPIXELSX);
	ReleaseDC(GetDesktopWindow(), hDC);
	return dpi / 96.0f;
}

CFont* CResourceSet::GetFontBySize(int size)
{
	int actualSize = int(size * GetScreenScale());
	auto iter = m_font_set.find(actualSize);
	if (iter != m_font_set.end()) {
		return (*iter).second;
	} else {
		LOGFONT logFont = { 0 };
		afxGlobalData.fontRegular.GetLogFont(&logFont);

		logFont.lfHeight = -(actualSize);
		HFONT hFont = CreateFontIndirect(&logFont);

		CFont* pResultFont =  CFont::FromHandle(hFont);
		m_font_set.insert(make_pair(actualSize, pResultFont));
		return pResultFont;
	}
}