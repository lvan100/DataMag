#include "stdafx.h"
#include "DataMag.h"
#include "SettingDlg.h"
#include "BookListDlg.h"
#include "BookListCtrl.h"

IMPLEMENT_DYNAMIC(CBookListCtrl, CMFCShellListCtrl)

CBookListCtrl::CBookListCtrl()
{
}

CBookListCtrl::~CBookListCtrl()
{
}

BEGIN_MESSAGE_MAP(CBookListCtrl, CMFCShellListCtrl)
	ON_WM_CREATE()
	ON_NOTIFY_REFLECT(NM_DBLCLK, &CBookListCtrl::OnDblClk)
END_MESSAGE_MAP()

int CBookListCtrl::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CMFCListCtrl::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	InitBookList();

	return 0;
}

void CBookListCtrl::PreSubclassWindow()
{
	CMFCListCtrl::PreSubclassWindow();

	InitBookList();
}

void CBookListCtrl::InitBookList()
{
	OnSetColumns();

	CString strFolder = theSetting.strMagFolder;
	strFolder += _T("\\");
	strFolder += BOOK_DIR;
	DisplayFolder(strFolder);
}

void CBookListCtrl::OnSetColumns()
{
	CMFCShellListCtrl::OnSetColumns();

	int nColumnCount = GetHeaderCtrl().GetItemCount();
	for (int i = 0; i < nColumnCount - 1; i++)
	{
		DeleteColumn(1);
	}

	SetColumnWidth(0, LVSCW_AUTOSIZE_USEHEADER);
}

void CBookListCtrl::OnDblClk(NMHDR* /*pNMHDR*/, LRESULT* pResult)
{
	auto dlg = (CBookListDlg*)GetParent();
	dlg->SendMessage(WM_COMMAND, IDOK);

	*pResult = 0;
}

HRESULT CBookListCtrl::EnumObjects(LPSHELLFOLDER pParentFolder, LPITEMIDLIST pidlParent)
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
				if (_tcsstr(sfi.szDisplayName, strFilter) != NULL)
				{
					int iItem = InsertItem(&lvItem);
					if (iItem >= 0)
					{
						// Set columns:
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