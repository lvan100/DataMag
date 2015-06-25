#include "stdafx.h"
#include "DataMag.h"
#include "ShellListCtrl.h"

IMPLEMENT_DYNAMIC(CShellListCtrl, CMFCShellListCtrl)

CShellListCtrl::CShellListCtrl()
	: m_bDClickOpenFolder(FALSE)
	, m_event(NULL){
}

CShellListCtrl::~CShellListCtrl(){
}

BEGIN_MESSAGE_MAP(CShellListCtrl, CMFCShellListCtrl)
	ON_WM_CREATE()
	ON_NOTIFY_REFLECT(NM_DBLCLK, &CShellListCtrl::OnDoubleClick)
	ON_NOTIFY_REFLECT(LVN_ITEMCHANGED, &CShellListCtrl::OnLvnItemchanged)
END_MESSAGE_MAP()

int CShellListCtrl::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CMFCListCtrl::OnCreate(lpCreateStruct) == -1)
		return -1;

	OnSetColumns();

	if (m_event != NULL) {
		m_event->InitShellList();
	}

	return 0;
}

void CShellListCtrl::PreSubclassWindow()
{
	CMFCListCtrl::PreSubclassWindow();

	OnSetColumns();

	if (m_event != NULL) {
		m_event->InitShellList();
	}
}

void CShellListCtrl::OnSetColumns()
{
	CMFCShellListCtrl::OnSetColumns();

	int nColumnCount = GetHeaderCtrl().GetItemCount();
	for (int i = 0; i < nColumnCount - 1; i++)
	{
		DeleteColumn(1);
	}

	SetColumnWidth(0, LVSCW_AUTOSIZE_USEHEADER);
}

void CShellListCtrl::OnLvnItemchanged(NMHDR *pNMHDR, LRESULT *pResult)
{
	if (m_event != NULL) {
		m_event->OnSelectChanged();
	}

	*pResult = 0;
}

void CShellListCtrl::OnDoubleClick(NMHDR* /*pNMHDR*/, LRESULT* pResult)
{
	if (m_event != NULL) {
		m_event->OnDoubleClick();
	}

	*pResult = 0;
}

void CShellListCtrl::DoDefaultDClick()
{
	int nItem = GetNextItem(-1, LVNI_FOCUSED);
	if (nItem != -1)
	{
		DoDefault(nItem);
	}
}

HRESULT CShellListCtrl::EnumObjects(LPSHELLFOLDER pParentFolder, LPITEMIDLIST pidlParent)
{
	ASSERT_VALID(this);
	ASSERT_VALID(afxShellManager);

	LPENUMIDLIST pEnum = NULL;
	HRESULT hRes = pParentFolder->EnumObjects(NULL, m_nTypes, &pEnum);

	if (SUCCEEDED(hRes) && pEnum != NULL)
	{
		LPITEMIDLIST pidlTemp;
		DWORD dwFetched = 1;
		LPAFX_SHELLITEMINFO pItem;

		//enumerate the item's PIDLs
		while (pEnum->Next(1, &pidlTemp, &dwFetched) == S_OK && dwFetched)
		{
			LVITEM lvItem;
			ZeroMemory(&lvItem, sizeof(lvItem));

			//fill in the TV_ITEM structure for this item
			lvItem.mask = LVIF_PARAM | LVIF_TEXT | LVIF_IMAGE | LVIF_STATE;

			//AddRef the parent folder so it's pointer stays valid
			pParentFolder->AddRef();

			//put the private information in the lParam
			pItem = (LPAFX_SHELLITEMINFO)GlobalAlloc(GPTR, sizeof(AFX_SHELLITEMINFO));

			pItem->pidlRel = pidlTemp;
			pItem->pidlFQ = afxShellManager->ConcatenateItem(pidlParent, pidlTemp);

			pItem->pParentFolder = pParentFolder;
			lvItem.lParam = (LPARAM)pItem;

			lvItem.pszText = _T("");
			lvItem.iImage = OnGetItemIcon(GetItemCount(), pItem);

			//determine if the item is shared
			DWORD dwAttr = SFGAO_DISPLAYATTRMASK;
			pParentFolder->GetAttributesOf(1, (LPCITEMIDLIST*)&pidlTemp, &dwAttr);

			if (dwAttr & SFGAO_SHARE)
			{
				lvItem.mask |= LVIF_STATE;
				lvItem.stateMask |= LVIS_OVERLAYMASK;
				lvItem.state |= INDEXTOOVERLAYMASK(1); //1 is the index for the shared overlay image
			}

			if (dwAttr & SFGAO_GHOSTED)
			{
				lvItem.mask |= LVIF_STATE;
				lvItem.stateMask |= LVIS_CUT;
				lvItem.state |= LVIS_CUT;
			}

			SHFILEINFO sfi;
			if (SHGetFileInfo((LPCTSTR)pItem->pidlFQ, 0, &sfi, sizeof(sfi), SHGFI_PIDL | SHGFI_DISPLAYNAME))
			{
				CString strPath = sfi.szDisplayName;
				strPath.MakeLower();

				if ((m_filter.GetLength() == 0) || (strPath.Find(m_filter) >= 0))
				{
					int iItem = InsertItem(&lvItem);
					if (iItem >= 0)
					{
						const int nColumns = m_wndHeader.GetItemCount();
						for (int iColumn = 0; iColumn < nColumns; iColumn++)
						{
							SetItemText(iItem, iColumn, OnGetItemText(iItem, iColumn, pItem));
						}
					}
				}				
			}

			dwFetched = 0;
		}

		pEnum->Release();
	}

	return hRes;
}

void CShellListCtrl::DoDefault(int iItem)
{
	LVITEM lvItem;

	ZeroMemory(&lvItem, sizeof(lvItem));
	lvItem.mask = LVIF_PARAM;
	lvItem.iItem = iItem;

	if (!GetItem(&lvItem))
	{
		return;
	}

	LPAFX_SHELLITEMINFO pInfo = (LPAFX_SHELLITEMINFO) lvItem.lParam;
	if (pInfo == NULL || pInfo->pParentFolder == NULL || pInfo->pidlRel == NULL)
	{
		ASSERT(FALSE);
		return;
	}

	IShellFolder *psfFolder = pInfo->pParentFolder;
	if (psfFolder == NULL)
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

	if (psfFolder == NULL)
	{
		return;
	}

	// If specified element is a folder, try to display it:
	ULONG ulAttrs = SFGAO_FOLDER;
	psfFolder->GetAttributesOf(1, (const struct _ITEMIDLIST **) &pInfo->pidlRel, &ulAttrs);

	if (m_bDClickOpenFolder && (ulAttrs & SFGAO_FOLDER))
	{
		CMFCShellListCtrl::DisplayFolder(pInfo);
	}
	else
	{
		// Invoke a default menu command:
		IContextMenu *pcm;
		HRESULT hr = psfFolder->GetUIObjectOf(GetSafeHwnd(), 1, (LPCITEMIDLIST*)&pInfo->pidlRel, IID_IContextMenu, NULL, (LPVOID*)&pcm);

		if (SUCCEEDED(hr))
		{
			HMENU hPopup = CreatePopupMenu();

			if (hPopup != NULL)
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
						cmi.lpParameters = NULL;
						cmi.lpDirectory = NULL;
						cmi.nShow = SW_SHOWNORMAL;
						cmi.dwHotKey = 0;
						cmi.hIcon = NULL;

						hr = pcm->InvokeCommand(&cmi);

						if (SUCCEEDED(hr) && GetParent() != NULL)
						{
							GetParent()->SendMessage(AFX_WM_ON_AFTER_SHELL_COMMAND, (WPARAM) idCmd);
						}
					}
				}
			}

			pcm->Release();
		}
	}

	psfFolder->Release();
}