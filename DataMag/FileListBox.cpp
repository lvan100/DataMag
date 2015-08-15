#include "stdafx.h"
#include "FileListBox.h"

IMPLEMENT_DYNAMIC(CFileListBox, CWnd)

CFileListBox::CFileListBox(CShellManager* pShellManager)
	: m_event(nullptr)
	, m_pidlCurFQ(nullptr)
	, m_bIsDesktop(FALSE)
	, m_psfCurFolder(nullptr)
	, m_pHiliteBorder(nullptr)
	, m_pShellManager(pShellManager)
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
	if (pItem == nullptr || pItem->pidlFQ == nullptr)
	{
		return FALSE;
	}

	TCHAR szPath [MAX_PATH];
	if (!SHGetPathFromIDList(pItem->pidlFQ, szPath))
	{
		return FALSE;
	}

	strPath = szPath;
	return TRUE;
}

BOOL CFileListBox::GetCurrentFolder(CString& strPath)
{
	ASSERT_VALID(this);

	strPath.Empty();

	if (m_pidlCurFQ == nullptr)
	{
		return FALSE;
	}

	TCHAR szPath [MAX_PATH];
	if (!SHGetPathFromIDList(m_pidlCurFQ, szPath))
	{
		return FALSE;
	}

	strPath = szPath;
	return TRUE;
}

BOOL CFileListBox::GetCurrentFolderName(CString& strName)
{
	ASSERT_VALID(this);

	strName.Empty();

	if (m_pidlCurFQ == nullptr)
	{
		return FALSE;
	}

	SHFILEINFO sfi;
	if (!SHGetFileInfo((LPCTSTR)m_pidlCurFQ, 0, &sfi, sizeof(sfi), SHGFI_PIDL | SHGFI_DISPLAYNAME))
	{
		return FALSE;
	}

	strName = sfi.szDisplayName;
	return TRUE;
}

HRESULT CFileListBox::Refresh()
{
	return DisplayFolder((LPAFX_SHELLITEMINFO) nullptr);
}

HRESULT CFileListBox::DisplayFolder(LPCTSTR lpszPath)
{
	if (m_pShellManager == nullptr)
	{
		ASSERT(FALSE);
		return E_FAIL;
	}

	ENSURE(lpszPath != nullptr);
	ASSERT_VALID(m_pShellManager);

	AFX_SHELLITEMINFO info;
	HRESULT hr = m_pShellManager->ItemFromPath(lpszPath, info.pidlRel);

	if (FAILED(hr))
	{
		return hr;
	}

	LPSHELLFOLDER pDesktopFolder;
	hr = SHGetDesktopFolder(&pDesktopFolder);

	if (SUCCEEDED(hr))
	{
		info.pParentFolder = pDesktopFolder;
		info.pidlFQ = info.pidlRel;

		hr = DisplayFolder(&info);
		pDesktopFolder->Release();
	}

	m_pShellManager->FreeItem(info.pidlFQ);
	return hr;
}

HRESULT CFileListBox::DisplayFolder(LPAFX_SHELLITEMINFO pItemInfo)
{
	HRESULT hr = E_FAIL;

	if (m_pShellManager == nullptr)
	{
		ASSERT(FALSE);
		return hr;
	}

	if (pItemInfo != nullptr)
	{
		ReleaseCurrFolder();
		hr = LockCurrentFolder(pItemInfo);

		if (FAILED(hr))
		{
			return hr;
		}
	}

	ResetContent();

	if (m_psfCurFolder != nullptr)
	{
		CWaitCursor wait;
		SetRedraw(FALSE);

		hr = EnumObjects(m_psfCurFolder, m_pidlCurFQ);

		SetRedraw(TRUE);
		RedrawWindow();
	}

	return hr;
}

HRESULT CFileListBox::DisplayParentFolder()
{
	ASSERT_VALID(m_pShellManager);

	HRESULT hr = E_FAIL;
	if (m_pidlCurFQ == nullptr)
	{
		return hr;
	}

	AFX_SHELLITEMINFO info;
	int nLevel = m_pShellManager->GetParentItem(m_pidlCurFQ, info.pidlFQ);

	if (nLevel < 0)
	{
		return hr;
	}

	if (nLevel == 0) // Desktop
	{
		hr = DisplayFolder(&info);
	}
	else
	{
		LPSHELLFOLDER pDesktopFolder;
		hr = SHGetDesktopFolder(&pDesktopFolder);

		if (SUCCEEDED(hr))
		{
			info.pParentFolder = pDesktopFolder;
			info.pidlRel = info.pidlFQ;

			hr = DisplayFolder(&info);
			pDesktopFolder->Release();
		}
	}

	m_pShellManager->FreeItem(info.pidlFQ);
	return hr;
}

HRESULT CFileListBox::LockCurrentFolder(LPAFX_SHELLITEMINFO pItemInfo)
{
	ASSERT_VALID(m_pShellManager);

	HRESULT hr = E_FAIL;
	m_pidlCurFQ = nullptr;

	if (pItemInfo != nullptr && pItemInfo->pParentFolder != nullptr)
	{
		ENSURE(pItemInfo->pidlRel != nullptr);
		hr = pItemInfo->pParentFolder->BindToObject(pItemInfo->pidlRel, nullptr, IID_IShellFolder, (LPVOID*)&m_psfCurFolder);

		m_bIsDesktop = FALSE;
	}
	else
	{
		hr = SHGetDesktopFolder(&m_psfCurFolder);
		m_bIsDesktop = TRUE;
	}

	if (SUCCEEDED(hr) && pItemInfo != nullptr)
	{
		m_pidlCurFQ = m_pShellManager->CopyItem(pItemInfo->pidlFQ);
	}

	return hr;
}

void CFileListBox::ReleaseCurrFolder()
{
	ASSERT_VALID(m_pShellManager);

	if (m_psfCurFolder != nullptr)
	{
		m_psfCurFolder->Release();
		m_psfCurFolder = nullptr;

		m_pShellManager->FreeItem(m_pidlCurFQ);
		m_pidlCurFQ = nullptr;
	}
}

CString CFileListBox::OnGetItemText(LPAFX_SHELLITEMINFO pItem)
{
	ASSERT_VALID(this);
	ENSURE(pItem != nullptr);

	SHFILEINFO sfi;

	if (SHGetFileInfo((LPCTSTR)pItem->pidlFQ, 0, &sfi, sizeof(sfi), SHGFI_PIDL | SHGFI_DISPLAYNAME))
	{
		return sfi.szDisplayName;
	}

	return _T("");
}

HRESULT CFileListBox::EnumObjects(LPSHELLFOLDER pParentFolder, LPITEMIDLIST pidlParent)
{
	ASSERT_VALID(this);
	ASSERT_VALID(m_pShellManager);

	LPENUMIDLIST pEnum = nullptr;
	HRESULT hRes = pParentFolder->EnumObjects(nullptr, (SHCONTF)(SHCONTF_FOLDERS | SHCONTF_NONFOLDERS), &pEnum);

	if (SUCCEEDED(hRes) && pEnum != nullptr)
	{
		LPITEMIDLIST pidlTemp;
		DWORD dwFetched = 1;
		LPAFX_SHELLITEMINFO pItem;

		//enumerate the item's PIDLs
		while (pEnum->Next(1, &pidlTemp, &dwFetched) == S_OK && dwFetched)
		{
			//AddRef the parent folder so it's pointer stays valid
			pParentFolder->AddRef();

			//put the private information in the lParam
			pItem = (LPAFX_SHELLITEMINFO)GlobalAlloc(GPTR, sizeof(AFX_SHELLITEMINFO));

			pItem->pidlRel = pidlTemp;
			pItem->pidlFQ = m_pShellManager->ConcatenateItem(pidlParent, pidlTemp);

			pItem->pParentFolder = pParentFolder;

			CString strItem = OnGetItemText(pItem);
			if (strItem.Find(m_filter) >= 0)
			{
				int nItem = AddString(strItem);
				if (nItem != -1)
				{
					SetItemData(nItem, (DWORD_PTR)pItem);
				}
			}

			dwFetched = 0;
		}

		pEnum->Release();
	}

	return hRes;
}

void CFileListBox::DoDefault(int iItem)
{
	LPAFX_SHELLITEMINFO pInfo = (LPAFX_SHELLITEMINFO) GetItemData(iItem);
	if (pInfo == nullptr || pInfo->pParentFolder == nullptr || pInfo->pidlRel == nullptr)
	{
		ASSERT(FALSE);
		return;
	}

	IShellFolder *psfFolder = pInfo->pParentFolder;
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
	psfFolder->GetAttributesOf(1, (const struct _ITEMIDLIST **) &pInfo->pidlRel, &ulAttrs);

	// Invoke a default menu command:
	IContextMenu *pcm;
	HRESULT hr = psfFolder->GetUIObjectOf(GetSafeHwnd(), 1, (LPCITEMIDLIST*)&pInfo->pidlRel, IID_IContextMenu, nullptr, (LPVOID*)&pcm);

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

					hr = pcm->InvokeCommand(&cmi);

					if (SUCCEEDED(hr) && GetParent() != nullptr)
					{
						GetParent()->SendMessage(AFX_WM_ON_AFTER_SHELL_COMMAND, (WPARAM) idCmd);
					}
				}
			}
		}

		pcm->Release();
	}

	psfFolder->Release();
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
	ASSERT_VALID(m_pShellManager);

	LPAFX_SHELLITEMINFO pItem = (LPAFX_SHELLITEMINFO)lpDeleteItemStruct->itemData;

	//free up the pidls that we allocated
	m_pShellManager->FreeItem(pItem->pidlFQ);
	m_pShellManager->FreeItem(pItem->pidlRel);

	//this may be nullptr if this is the root item
	if (pItem->pParentFolder != nullptr)
	{
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

	if (FAILED(hr))
	{
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