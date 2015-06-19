#include "stdafx.h"
#include "DataMag.h"
#include "DataMagDlg.h"
#include "LabelInfoCtrl.h"

IMPLEMENT_DYNAMIC(CLabelInfoCtrl, CMFCShellListCtrl)

CLabelInfoCtrl::CLabelInfoCtrl()
{

}

CLabelInfoCtrl::~CLabelInfoCtrl()
{
}

BEGIN_MESSAGE_MAP(CLabelInfoCtrl, CMFCShellListCtrl)
	ON_WM_CREATE()
	ON_NOTIFY_REFLECT(LVN_ITEMCHANGED, &CLabelInfoCtrl::OnLvnItemchanged)
END_MESSAGE_MAP()

int CLabelInfoCtrl::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CMFCListCtrl::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	OnSetColumns();

	DeleteAllItems();

	return 0;
}

void CLabelInfoCtrl::PreSubclassWindow()
{
	CMFCListCtrl::PreSubclassWindow();

	OnSetColumns();

	DeleteAllItems();
}

void CLabelInfoCtrl::OnSetColumns()
{
	CMFCShellListCtrl::OnSetColumns();

	int nColumnCount = GetHeaderCtrl().GetItemCount();
	for (int i = 0; i < nColumnCount - 1; i++)
	{
		DeleteColumn(1);
	}

	SetColumnWidth(0, LVSCW_AUTOSIZE_USEHEADER);
}

BOOL GetLinkFilePath(CString& strPath, CString strLink)
{
	HRESULT hResult = S_FALSE;

	IShellLink* pShellLink = NULL;
	AutoRelease<IShellLink*> tmp1(pShellLink);

	IPersistFile* pPersistFile = NULL;
	AutoRelease<IPersistFile*> tmp2(pPersistFile);

	hResult = CoCreateInstance(CLSID_ShellLink
		, NULL
		, CLSCTX_INPROC_SERVER
		, IID_IShellLink
		, (void**)&pShellLink);  
	if(FAILED(hResult))
	{
		return FALSE;
	}

	hResult = pShellLink->QueryInterface(IID_IPersistFile
		, (void**)&pPersistFile);
	if(FAILED(hResult))
	{
		return FALSE;
	}

	hResult = pPersistFile->Load(strLink, STGM_READ);
	if(FAILED(hResult))
	{
		return FALSE;
	}

	hResult = pShellLink->Resolve(NULL, SLR_ANY_MATCH);
	if(FAILED(hResult))
	{
		return FALSE;
	}

	WCHAR szPath[MAX_PATH];

	hResult = pShellLink->GetPath(szPath, MAX_PATH, NULL, SLGP_SHORTPATH); 
	if(FAILED(hResult))
	{
		return FALSE;
	}

	strPath.SetString(szPath);

	return TRUE; 
}

void CLabelInfoCtrl::OnLvnItemchanged(NMHDR *pNMHDR, LRESULT *pResult)
{
	int nItem = GetNextItem(-1, LVNI_FOCUSED);
	if (nItem != -1)
	{
		CString strPath;
		CString strLink = GetItemPath(nItem);
		if (GetLinkFilePath(strPath, strLink))
		{
			char szText[1024];
			memset(szText, 0, 1024);

			if (PathIsDirectory(strPath))
			{
				CString strFile = strPath + _T("\\ÃèÊö.txt");
				CStdioFile file(strFile, CFile::modeReadWrite | CFile::typeText);
				file.Read(szText, 1024);
			}

			SetWindowTextA(theDataMagDlg->m_item_text.GetSafeHwnd(), szText);
		}
	}

	*pResult = 0;
}