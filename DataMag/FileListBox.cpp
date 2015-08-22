#include "stdafx.h"
#include "FileListBox.h"

IMPLEMENT_DYNAMIC(CFileListBox, CWnd)

CFileListBox::CFileListBox(CShellManager* pShellManager)
	: CFolderEnum(pShellManager)
	, m_pHiliteBorder(nullptr)
	, m_nLastSelItem(-1)
	, m_event(nullptr)
{
}

CFileListBox::~CFileListBox()
{
}

BEGIN_MESSAGE_MAP(CFileListBox, CWnd)
	ON_WM_CREATE()
	ON_WM_DESTROY()
	ON_WM_SETFOCUS()
	ON_WM_KILLFOCUS()
	ON_WM_DELETEITEM_REFLECT()
	ON_WM_COMPAREITEM_REFLECT()
	ON_CONTROL_REFLECT(LBN_DBLCLK, &CFileListBox::OnLbnDblclk)
	ON_CONTROL_REFLECT(LBN_SELCHANGE, &CFileListBox::OnLbnSelchange)
END_MESSAGE_MAP()

int CFileListBox::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CListBox::OnCreate(lpCreateStruct) == -1)
		return -1;

	if (m_event != nullptr) {
		m_event->InitListBox();
	}

	if (!InitBorder()) {
		return -1;
	}

	return 0;
}

void CFileListBox::PreSubclassWindow()
{
	CListBox::PreSubclassWindow();

	if (m_event != nullptr) {
		m_event->InitListBox();
	}

	if (!InitBorder()) {
		ASSERT(FALSE);
	}
}

BOOL CFileListBox::InitBorder() 
{
	ASSERT(m_pHiliteBorder == nullptr);

	CRect rcBorder;
	GetWindowRect(rcBorder);
	GetParent()->ScreenToClient(rcBorder);

	m_pHiliteBorder = new CHiliteBorder();

	DWORD dwStyle = WS_VISIBLE | WS_CHILD | SS_OWNERDRAW;
	if (!m_pHiliteBorder->Create(NULL, dwStyle, rcBorder, GetParent())) {
		return FALSE;
	}

	rcBorder.DeflateRect(1,1,1,1);
	MoveWindow(rcBorder);

	return TRUE;
}

BOOL CFileListBox::GetItemPath(CString& strPath, int iItem)
{
	ASSERT_VALID(this);

	strPath.Empty();

	LPAFX_SHELLITEMINFO pItem = (LPAFX_SHELLITEMINFO) GetItemData(iItem);
	if (pItem == nullptr || pItem->pidlFQ == nullptr) {
		return FALSE;
	}

	TCHAR szPath [MAX_PATH];
	if (!SHGetPathFromIDList(pItem->pidlFQ, szPath)) {
		return FALSE;
	}

	strPath = szPath;
	return TRUE;
}

void CFileListBox::OnDisplayFolderInit()
{
	if (m_event != nullptr) {
		m_event->OnSelectChanged();
	}

	ResetContent();

	m_nLastSelItem = -1;

	if (m_event != nullptr) {
		m_event->OnSelectChanged();
	}
}

void CFileListBox::OnDisplayFolderBefore()
{
	SetRedraw(FALSE);
}

void CFileListBox::OnDisplayFolderAfter()
{
	SetRedraw(TRUE);
	RedrawWindow();
}

BOOL CFileListBox::OnEnumObject(LPAFX_SHELLITEMINFO pItem)
{
	CString strItem = GetItemText(pItem);
	if (strItem.Find(m_filter) >= 0) {
		int nItem = AddString(strItem);
		if (nItem != -1) {
			return (SetItemData(nItem, (DWORD_PTR)pItem) >= 0);
		}
	}
	return FALSE;
}

BOOL CFileListBox::DoDefault(int iItem)
{
	LPAFX_SHELLITEMINFO pInfo = (LPAFX_SHELLITEMINFO) GetItemData(iItem);
	if (pInfo == nullptr || pInfo->pParentFolder == nullptr || pInfo->pidlRel == nullptr) {
		ASSERT(FALSE);
		return FALSE;
	}

	IShellFolder *psfFolder = pInfo->pParentFolder;
	if (psfFolder == nullptr) {
		HRESULT hr = SHGetDesktopFolder(&psfFolder);
		if (FAILED(hr))	{
			ASSERT(FALSE);
			return FALSE;
		}

	} else {
		psfFolder->AddRef();
	}

	ULONG ulAttrs = SFGAO_FOLDER;
	psfFolder->GetAttributesOf(1, (LPCITEMIDLIST*) &pInfo->pidlRel, &ulAttrs);

	IContextMenu *pcm;
	HRESULT hr = psfFolder->GetUIObjectOf(GetSafeHwnd(), 1
		, (LPCITEMIDLIST*)&pInfo->pidlRel
		, IID_IContextMenu, nullptr, (LPVOID*)&pcm);

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
	return SUCCEEDED(hr);
}

void CFileListBox::OnDestroy()
{
	ReleaseCurrFolder();

	if (m_pHiliteBorder != nullptr) {
		m_pHiliteBorder->DestroyWindow();
		delete m_pHiliteBorder;
	}

	CListBox::OnDestroy();
}

void CFileListBox::DeleteItem(LPDELETEITEMSTRUCT lpDeleteItemStruct)
{
	LPAFX_SHELLITEMINFO pItem = (LPAFX_SHELLITEMINFO)lpDeleteItemStruct->itemData;

	m_pShellManager->FreeItem(pItem->pidlFQ);
	m_pShellManager->FreeItem(pItem->pidlRel);

	if (pItem->pParentFolder != nullptr) {
		pItem->pParentFolder->Release();
		pItem->pParentFolder = nullptr;
	}

	GlobalFree((HGLOBAL) pItem);
}

int CFileListBox::CompareItem(LPCOMPAREITEMSTRUCT lpCompareItemStruct)
{
	LPAFX_SHELLITEMINFO pItem1 = (LPAFX_SHELLITEMINFO)lpCompareItemStruct->itemData1;
	LPAFX_SHELLITEMINFO pItem2 = (LPAFX_SHELLITEMINFO)lpCompareItemStruct->itemID2;

	HRESULT hr = pItem1->pParentFolder->CompareIDs(0, pItem1->pidlRel, pItem2->pidlRel);
	if (FAILED(hr)) {
		return 0;
	}

	return (short) SCODE_CODE(GetScode(hr));
}

void CFileListBox::OnLbnDblclk()
{
	if (m_event != nullptr) {
		m_event->OnDoubleClick();
	}
}

void CFileListBox::OnLbnSelchange()
{
	if (m_event != nullptr) {
		m_event->OnSelectChanged();
	}

	m_nLastSelItem = GetCurSel();
}

void CFileListBox::OnSetFocus(CWnd* pOldWnd)
{
	CListBox::OnSetFocus(pOldWnd);

	if (m_pHiliteBorder != nullptr) {
		m_pHiliteBorder->Hilite(TRUE);
	}
}

void CFileListBox::OnKillFocus(CWnd* pNewWnd)
{
	CListBox::OnKillFocus(pNewWnd);

	if (m_pHiliteBorder != nullptr) {
		m_pHiliteBorder->Hilite(FALSE);
	}
}