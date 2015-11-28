#include "stdafx.h"
#include "FolderList.h"

IMPLEMENT_DYNAMIC(CFolderListCtrl, CListBox)

CFolderListCtrl::CFolderListCtrl(CShellManager* pShellManager)
	: m_event(nullptr)
	, m_hTagImage(nullptr)
	, m_hCodeImage(nullptr)
	, m_hBookImage(nullptr)
	, m_isBkgndCleared(FALSE)
	, m_pHiliteBorder(nullptr)
	, m_pShellManager(pShellManager)
{
}

CFolderListCtrl::~CFolderListCtrl()
{
}

BEGIN_MESSAGE_MAP(CFolderListCtrl, CListBox)
	ON_WM_CREATE()
	ON_WM_DESTROY()
	ON_WM_SETFOCUS()
	ON_WM_KILLFOCUS()
	ON_WM_ERASEBKGND()
	ON_WM_CTLCOLOR_REFLECT()
	ON_CONTROL_REFLECT(LBN_DBLCLK, &CFolderListCtrl::OnLbnDblclk)
END_MESSAGE_MAP()

int CFolderListCtrl::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CListBox::OnCreate(lpCreateStruct) == -1) {
		return -1;
	}

	if (m_event != nullptr) {
		m_event->InitListBox();
	}

	if (!InitBorder()) {
		return -1;
	}

	return 0;
}

void CFolderListCtrl::PreSubclassWindow()
{
	CListBox::PreSubclassWindow();

	if (m_event != nullptr) {
		m_event->InitListBox();
	}

	if (!InitBorder()) {
		ASSERT(FALSE);
	}
}
BOOL CFolderListCtrl::InitBorder()
{
	ASSERT(m_pHiliteBorder == nullptr);

	CRect rcBorder;
	GetWindowRect(rcBorder);
	GetParent()->ScreenToClient(rcBorder);

	m_pHiliteBorder = new CHiliteBorder();

	DWORD dwStyle = WS_VISIBLE | WS_CHILD | SS_OWNERDRAW;
	if (!m_pHiliteBorder->Create(nullptr, dwStyle, rcBorder, GetParent())) {
		return FALSE;
	}

	rcBorder.DeflateRect(1, 1, 1, 1);
	MoveWindow(rcBorder);

	return TRUE;
}

void CFolderListCtrl::OnDestroy()
{
	if (m_pHiliteBorder != nullptr) {
		m_pHiliteBorder->DestroyWindow();
		delete m_pHiliteBorder;
	}

	CListBox::OnDestroy();
}

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

void CFolderListCtrl::OnSetFocus(CWnd* pOldWnd)
{
	CListBox::OnSetFocus(pOldWnd);

	if (m_pHiliteBorder != nullptr) {
		m_pHiliteBorder->Hilite(TRUE);
	}
}

void CFolderListCtrl::OnKillFocus(CWnd* pNewWnd)
{
	CListBox::OnKillFocus(pNewWnd);

	if (m_pHiliteBorder != nullptr) {
		m_pHiliteBorder->Hilite(FALSE);
	}
}

BOOL CFolderListCtrl::OnEraseBkgnd(CDC* pDC)
{
	if (GetFocus() == this) {
		CRect rcClient;
		GetClientRect(rcClient);

		rcClient.top += 5;
		rcClient.left += 5;
		rcClient.right -= 5;
		rcClient.bottom -= 5;		

		pDC->DrawFocusRect(rcClient);
	}

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