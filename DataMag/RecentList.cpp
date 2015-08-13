#include "stdafx.h"
#include "DataMag.h"
#include "RecentList.h"

IMPLEMENT_DYNAMIC(CRecentList, CListBox)

CRecentList::CRecentList(CShellManager* pShellManager)
	: m_event(nullptr)
	, m_hTagImage(nullptr)
	, m_hCodeImage(nullptr)
	, m_hBookImage(nullptr)
	, m_pShellManager(pShellManager)
{
}

CRecentList::~CRecentList()
{
}

BEGIN_MESSAGE_MAP(CRecentList, CListBox)
	ON_WM_KILLFOCUS()
	ON_WM_ERASEBKGND()
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

	if ((GetFocus() == this) && ((lpDrawItemStruct->itemState & ODS_SELECTED) == ODS_SELECTED)) {
		pDC->FillRect(&lpDrawItemStruct->rcItem, &afxGlobalData.brHilite);
	} else {
		pDC->FillRect(&lpDrawItemStruct->rcItem, &afxGlobalData.brBtnFace);
	}

	CRect rcIcon(lpDrawItemStruct->rcItem);
	rcIcon.left += 7;
	rcIcon.right = rcIcon.left + rcIcon.Height();

	if (strType.CompareNoCase(_T("源码")) == 0) {
		DrawIconEx(lpDrawItemStruct->hDC, rcIcon.left, rcIcon.top
			, m_hCodeImage, rcIcon.Width(), rcIcon.Height()
			, 0, nullptr, DI_NORMAL);
	} else if (strType.CompareNoCase(_T("图书")) == 0) {
		DrawIconEx(lpDrawItemStruct->hDC, rcIcon.left, rcIcon.top
			, m_hBookImage, rcIcon.Width(), rcIcon.Height()
			, 0, nullptr, DI_NORMAL);
	} else if (strType.CompareNoCase(_T("标签")) == 0) {
		DrawIconEx(lpDrawItemStruct->hDC, rcIcon.left, rcIcon.top
			, m_hTagImage, rcIcon.Width(), rcIcon.Height()
			, 0, nullptr, DI_NORMAL);
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

void CRecentList::DoDefault(int iItem)
{
	AFX_SHELLITEMINFO info;

	LPCTSTR szPath = (LPCTSTR)GetItemData(iItem);
	HRESULT hr = m_pShellManager->ItemFromPath(szPath, info.pidlRel);

	if (FAILED(hr))
	{
		return;
	}
	
	IShellFolder *psfFolder = info.pParentFolder;
	if (psfFolder == nullptr)
	{
		HRESULT hr = SHGetDesktopFolder(&psfFolder);
		if (FAILED(hr))
		{
			ASSERT(FALSE);
			return;
		}
	}
	else
	{
		psfFolder->AddRef();
	}

	if (psfFolder == nullptr)
	{
		return;
	}

	// If specified element is a folder, try to display it:
	ULONG ulAttrs = SFGAO_FOLDER;
	psfFolder->GetAttributesOf(1, (const struct _ITEMIDLIST **) &info.pidlRel, &ulAttrs);

	// Invoke a default menu command:
	IContextMenu *pcm;
	hr = psfFolder->GetUIObjectOf(GetSafeHwnd(), 1, (LPCITEMIDLIST*)&info.pidlRel, IID_IContextMenu, nullptr, (LPVOID*)&pcm);

	if (SUCCEEDED(hr))
	{
		HMENU hPopup = CreatePopupMenu();

		if (hPopup != nullptr)
		{
			hr = pcm->QueryContextMenu(hPopup, 0, 1, 0x7fff, CMF_DEFAULTONLY | CMF_EXPLORE);

			if (SUCCEEDED(hr))
			{
				UINT idCmd = ::GetMenuDefaultItem(hPopup, FALSE, 0);
				if (idCmd != 0 && idCmd != (UINT)-1)
				{
					CMINVOKECOMMANDINFO cmi;
					cmi.cbSize = sizeof(CMINVOKECOMMANDINFO);
					cmi.fMask = 0;
					cmi.hwnd = GetParent()->GetSafeHwnd();
					cmi.lpVerb = (LPCSTR)(INT_PTR)(idCmd - 1);
					cmi.lpParameters = nullptr;
					cmi.lpDirectory = nullptr;
					cmi.nShow = SW_SHOWNORMAL;
					cmi.dwHotKey = 0;
					cmi.hIcon = nullptr;

					pcm->InvokeCommand(&cmi);
				}
			}
		}

		pcm->Release();
	}

	psfFolder->Release();

	// 释放内核资源
	m_pShellManager->FreeItem(info.pidlRel);
}

void CRecentList::OnLbnDblclk()
{
	if (m_event != nullptr) {
		m_event->OnDoubleClick();
	}
}

void CRecentList::OnKillFocus(CWnd* pNewWnd)
{
	CListBox::OnKillFocus(pNewWnd);

	Invalidate();
}

BOOL CRecentList::OnEraseBkgnd(CDC* pDC)
{
	return TRUE;
}