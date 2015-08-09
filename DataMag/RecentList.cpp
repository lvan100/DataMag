#include "stdafx.h"
#include "DataMag.h"
#include "RecentList.h"

IMPLEMENT_DYNAMIC(CRecentList, CListBox)

CRecentList::CRecentList()
	: m_hLabelImage(NULL)
	, m_hCodeImage(NULL)
	, m_hBookImage(NULL)
	, m_event(NULL)
{
}

CRecentList::~CRecentList()
{
}

BEGIN_MESSAGE_MAP(CRecentList, CListBox)
	ON_WM_CTLCOLOR_REFLECT()
	ON_CONTROL_REFLECT(LBN_DBLCLK, &CRecentList::OnLbnDblclk)
END_MESSAGE_MAP()

HBRUSH CRecentList::CtlColor(CDC* pDC, UINT nCtlColor)
{
	return (HBRUSH)afxGlobalData.brBtnFace.GetSafeHandle();
}

void CRecentList::DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct)
{
	LPCTSTR str = (LPCTSTR)lpDrawItemStruct->itemData;

	CString strPath(str);
	PathRemoveBackslash(strPath.GetBuffer());

	int nBackslash = strPath.ReverseFind('\\');
	CString strName = strPath.Mid(nBackslash + 1);

	CString strSub = strPath.Left(nBackslash);
	nBackslash = strSub.ReverseFind('\\');
	CString strType = strSub.Mid(nBackslash + 1);

	CDC* pDC = CDC::FromHandle(lpDrawItemStruct->hDC);

	if ((lpDrawItemStruct->itemState & ODS_SELECTED) == ODS_SELECTED) {
		pDC->FillRect(&lpDrawItemStruct->rcItem, &afxGlobalData.brHilite);
	} else {
		pDC->FillRect(&lpDrawItemStruct->rcItem, &afxGlobalData.brBtnFace);
	}

	CRect rcIcon(lpDrawItemStruct->rcItem);
	rcIcon.left += 2;
	rcIcon.right = rcIcon.left + rcIcon.Height();

	if (strType.CompareNoCase(_T("Ô´Âë")) == 0) {
		DrawIconEx(lpDrawItemStruct->hDC, rcIcon.left, rcIcon.top
			, m_hCodeImage, rcIcon.Width(), rcIcon.Height()
			, 0, NULL, DI_NORMAL);
	} else if (strType.CompareNoCase(_T("Í¼Êé")) == 0) {
		DrawIconEx(lpDrawItemStruct->hDC, rcIcon.left, rcIcon.top
			, m_hBookImage, rcIcon.Width(), rcIcon.Height()
			, 0, NULL, DI_NORMAL);
	} else if (strType.CompareNoCase(_T("±êÇ©")) == 0) {
		DrawIconEx(lpDrawItemStruct->hDC, rcIcon.left, rcIcon.top
			, m_hLabelImage, rcIcon.Width(), rcIcon.Height()
			, 0, NULL, DI_NORMAL);
	}

	CRect rcText(lpDrawItemStruct->rcItem);
	rcText.left = rcIcon.right + 4;

	pDC->SetBkMode(TRANSPARENT);
	pDC->DrawText(strName, rcText, DT_LEFT | DT_VCENTER | DT_SINGLELINE);
}

void CRecentList::MeasureItem(LPMEASUREITEMSTRUCT /*lpMeasureItemStruct*/)
{
	// Do default.
}

void CRecentList::OnLbnDblclk()
{
	if (m_event != NULL) {
		m_event->OnDoubleClick();
	}
}