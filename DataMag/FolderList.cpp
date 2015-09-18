#include "stdafx.h"
#include "FolderList.h"

IMPLEMENT_DYNAMIC(CFolderListCtrl, CListBox)

CFolderListCtrl::CFolderListCtrl(CShellManager* pShellManager)
	: m_event(nullptr)
	, m_hTagImage(nullptr)
	, m_hCodeImage(nullptr)
	, m_hBookImage(nullptr)
	, m_isBkgndCleared(FALSE)
	, m_pShellManager(pShellManager)
{
}

CFolderListCtrl::~CFolderListCtrl()
{
}

BEGIN_MESSAGE_MAP(CFolderListCtrl, CListBox)
	ON_WM_KILLFOCUS()
	ON_WM_ERASEBKGND()
	ON_WM_CTLCOLOR_REFLECT()
	ON_CONTROL_REFLECT(LBN_DBLCLK, &CFolderListCtrl::OnLbnDblclk)
END_MESSAGE_MAP()

HBRUSH CFolderListCtrl::CtlColor(CDC* pDC, UINT nCtlColor)
{
	return afxGlobalData.brBtnFace;
}

void CFolderListCtrl::DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct)
{
	CDC* pDC = CDC::FromHandle(lpDrawItemStruct->hDC);

	if (::IsRectEmpty(&lpDrawItemStruct->rcItem)) {
		if (!m_isBkgndCleared){
			m_isBkgndCleared = TRUE;

			CRect rcClient;
			GetClientRect(rcClient);
			pDC->FillSolidRect(rcClient, afxGlobalData.clrBtnFace);
		}

		return;
	}

	m_isBkgndCleared = FALSE;

	LPCTSTR str = (LPCTSTR)lpDrawItemStruct->itemData;

	CString strPath(str);
	PathRemoveBackslash(strPath.GetBuffer());

	int nBackslash = strPath.ReverseFind('\\');
	CString strName = strPath.Mid(nBackslash + 1);

	CString strSub = strPath.Left(nBackslash);
	nBackslash = strSub.ReverseFind('\\');
	CString strType = strSub.Mid(nBackslash + 1);

	if ((GetFocus() == this) && ((lpDrawItemStruct->itemState & ODS_SELECTED) == ODS_SELECTED)) {
		pDC->FillRect(&lpDrawItemStruct->rcItem, &afxGlobalData.brHilite);
	} else {
		pDC->FillRect(&lpDrawItemStruct->rcItem, &afxGlobalData.brBtnFace);
	}

	CRect rcIcon(lpDrawItemStruct->rcItem);
	rcIcon.left += 7;
	rcIcon.right = rcIcon.left + rcIcon.Height();

	if (strType.CompareNoCase(_T("Ô´Âë")) == 0) {
		DrawIconEx(lpDrawItemStruct->hDC, rcIcon.left, rcIcon.top
			, m_hCodeImage, rcIcon.Width(), rcIcon.Height()
			, 0, nullptr, DI_NORMAL);
	} else if (strType.CompareNoCase(_T("Í¼Êé")) == 0) {
		DrawIconEx(lpDrawItemStruct->hDC, rcIcon.left, rcIcon.top
			, m_hBookImage, rcIcon.Width(), rcIcon.Height()
			, 0, nullptr, DI_NORMAL);
	} else if (strType.CompareNoCase(_T("±êÇ©")) == 0) {
		DrawIconEx(lpDrawItemStruct->hDC, rcIcon.left, rcIcon.top
			, m_hTagImage, rcIcon.Width(), rcIcon.Height()
			, 0, nullptr, DI_NORMAL);
	}

	CRect rcText(lpDrawItemStruct->rcItem);
	rcText.left = rcIcon.right + 4;

	pDC->SetBkMode(TRANSPARENT);
	pDC->DrawText(strName, rcText, DT_LEFT | DT_VCENTER | DT_SINGLELINE);
}

void CFolderListCtrl::MeasureItem(LPMEASUREITEMSTRUCT /*lpMeasureItemStruct*/)
{
	// Do default.
}

BOOL CFolderListCtrl::DoDefault(int iItem)
{
	AFX_SHELLITEMINFO info;

	LPCTSTR szPath = (LPCTSTR)GetItemData(iItem);
	HRESULT hr = m_pShellManager->ItemFromPath(szPath, info.pidlRel);
	if (FAILED(hr)) {
		return FALSE;
	}
	
	IShellFolder *psfFolder = info.pParentFolder;
	if (psfFolder == nullptr) {
		HRESULT hr = SHGetDesktopFolder(&psfFolder);
		if (FAILED(hr)) {
			ASSERT(FALSE);
			return FALSE;
		}

	} else {
		psfFolder->AddRef();
	}

	ULONG ulAttrs = SFGAO_FOLDER;
	psfFolder->GetAttributesOf(1, (LPCITEMIDLIST*) &info.pidlRel, &ulAttrs);

	IContextMenu *pcm;
	hr = psfFolder->GetUIObjectOf(GetSafeHwnd(), 1, (LPCITEMIDLIST*)&info.pidlRel, IID_IContextMenu, nullptr, (LPVOID*)&pcm);
	if (SUCCEEDED(hr)) {

		HMENU hPopup = CreatePopupMenu();
		if (hPopup != nullptr) {

			hr = pcm->QueryContextMenu(hPopup, 0, 1, 0x7fff, CMF_DEFAULTONLY | CMF_EXPLORE);
			if (SUCCEEDED(hr)) {

				UINT idCmd = ::GetMenuDefaultItem(hPopup, FALSE, 0);
				if (idCmd != 0 && idCmd != (UINT)-1) {

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

					hr = pcm->InvokeCommand(&cmi);
				}
			}
		}

		pcm->Release();
	}

	psfFolder->Release();

	m_pShellManager->FreeItem(info.pidlRel);
	return SUCCEEDED(hr);
}

void CFolderListCtrl::OnLbnDblclk()
{
	if (m_event != nullptr) {
		m_event->OnDoubleClick();
	}
}

void CFolderListCtrl::OnKillFocus(CWnd* pNewWnd)
{
	CListBox::OnKillFocus(pNewWnd);

	Invalidate();
}

BOOL CFolderListCtrl::OnEraseBkgnd(CDC* pDC)
{
	return TRUE;
}

CString CFolderListCtrl::GetItemPath(int iItem)
{
	if (iItem >= 0 && iItem < GetCount()) {
		return (LPCTSTR)GetItemData(iItem);
	} else {
		return _T("");
	}
}