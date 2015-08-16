#include "StdAfx.h"
#include "FolderEnum.h"

CFolderEnum::CFolderEnum(CShellManager* pShellManager)
	: m_pShellManager(pShellManager)
	, m_psfCurFolder(nullptr)
	, m_pidlCurFQ(nullptr)
	, m_bIsDesktop(FALSE)
{
	m_tfEnumConfig = (SHCONTF)(SHCONTF_FOLDERS | SHCONTF_NONFOLDERS);
}

CFolderEnum::~CFolderEnum(void)
{
}

BOOL CFolderEnum::GetCurrentFolder(CString& strPath)
{
	strPath.Empty();

	if (m_pidlCurFQ == nullptr) {
		return FALSE;
	}

	TCHAR szPath [MAX_PATH];
	if (!SHGetPathFromIDList(m_pidlCurFQ, szPath)) {
		return FALSE;
	}

	strPath = szPath;
	return TRUE;
}

BOOL CFolderEnum::GetCurrentFolderName(CString& strName)
{
	strName.Empty();

	if (m_pidlCurFQ == nullptr) {
		return FALSE;
	}

	SHFILEINFO sfi;
	if (!SHGetFileInfo((LPCTSTR)m_pidlCurFQ, 0, &sfi, sizeof(sfi), SHGFI_PIDL | SHGFI_DISPLAYNAME))	{
		return FALSE;
	}

	strName = sfi.szDisplayName;
	return TRUE;
}

HRESULT CFolderEnum::Refresh()
{
	return DisplayFolder((LPAFX_SHELLITEMINFO) nullptr);
}

HRESULT CFolderEnum::DisplayParentFolder()
{
	ASSERT_VALID(m_pShellManager);

	HRESULT hr = E_FAIL;
	if (m_pidlCurFQ == nullptr)	{
		return hr;
	}

	AFX_SHELLITEMINFO info;
	int nLevel = m_pShellManager->GetParentItem(m_pidlCurFQ, info.pidlFQ);
	if (nLevel < 0)	{
		return hr;
	}

	if (nLevel == 0) {
		// 整个文件系统的根节点
		hr = DisplayFolder(&info);

	} else {
		// 获取根节点 Shell 操作接口
		LPSHELLFOLDER pDesktopFolder;
		hr = SHGetDesktopFolder(&pDesktopFolder);

		if (SUCCEEDED(hr)) {

			info.pParentFolder = pDesktopFolder;
			info.pidlRel = info.pidlFQ;

			hr = DisplayFolder(&info);
			pDesktopFolder->Release();
		}
	}

	m_pShellManager->FreeItem(info.pidlFQ);
	return hr;
}

HRESULT CFolderEnum::DisplayFolder(LPCTSTR lpszPath)
{
	if (m_pShellManager == nullptr)	{
		ASSERT(FALSE);
		return E_FAIL;
	}

	ENSURE(lpszPath != nullptr);
	ASSERT_VALID(m_pShellManager);

	AFX_SHELLITEMINFO info;
	HRESULT hr = m_pShellManager->ItemFromPath(lpszPath, info.pidlRel);
	if (FAILED(hr))	{
		return hr;
	}

	// 获取根节点 Shell 操作接口
	LPSHELLFOLDER pDesktopFolder;
	hr = SHGetDesktopFolder(&pDesktopFolder);

	if (SUCCEEDED(hr)) {

		info.pParentFolder = pDesktopFolder;
		info.pidlFQ = info.pidlRel;

		hr = DisplayFolder(&info);
		pDesktopFolder->Release();
	}

	m_pShellManager->FreeItem(info.pidlFQ);
	return hr;
}

HRESULT CFolderEnum::DisplayFolder(LPAFX_SHELLITEMINFO pItemInfo)
{
	HRESULT hr = E_FAIL;

	if (m_pShellManager == nullptr) {
		ASSERT(FALSE);
		return hr;
	}

	if (pItemInfo != nullptr) {
		ReleaseCurrFolder();

		hr = LockCurrentFolder(pItemInfo);
		if (FAILED(hr)) {
			return hr;
		}
	}

	OnDisplayFolderInit();

	if (m_psfCurFolder != nullptr) {
		CWaitCursor wait;

		OnDisplayFolderBefore();
		{
			hr = EnumObjects(m_psfCurFolder, m_pidlCurFQ);
		}
		OnDisplayFolderAfter();
	}

	return hr;
}

HRESULT CFolderEnum::LockCurrentFolder(LPAFX_SHELLITEMINFO pItemInfo)
{
	ASSERT_VALID(m_pShellManager);

	HRESULT hr = E_FAIL;
	m_pidlCurFQ = nullptr;

	if (pItemInfo != nullptr && pItemInfo->pParentFolder != nullptr) {
		ENSURE(pItemInfo->pidlRel != nullptr);

		// 获取当前操作目录的 Shell 接口
		hr = pItemInfo->pParentFolder->BindToObject(pItemInfo->pidlRel, nullptr, IID_IShellFolder, (LPVOID*)&m_psfCurFolder);
		m_bIsDesktop = FALSE;

	} else {
		hr = SHGetDesktopFolder(&m_psfCurFolder);
		m_bIsDesktop = TRUE;
	}

	if (SUCCEEDED(hr) && pItemInfo != nullptr) {
		m_pidlCurFQ = m_pShellManager->CopyItem(pItemInfo->pidlFQ);
	}

	return hr;
}

void CFolderEnum::ReleaseCurrFolder()
{
	ASSERT_VALID(m_pShellManager);

	if (m_psfCurFolder != nullptr) {
		m_psfCurFolder->Release();
		m_psfCurFolder = nullptr;

		m_pShellManager->FreeItem(m_pidlCurFQ);
		m_pidlCurFQ = nullptr;
	}
}

CString CFolderEnum::GetItemText(LPAFX_SHELLITEMINFO pItem)
{
	ENSURE(pItem != nullptr);

	SHFILEINFO sfi;

	if (SHGetFileInfo((LPCTSTR)pItem->pidlFQ, 0, &sfi, sizeof(sfi), SHGFI_PIDL | SHGFI_DISPLAYNAME)) {
		return sfi.szDisplayName;
	}

	return _T("");
}

HRESULT CFolderEnum::EnumObjects(LPSHELLFOLDER pParentFolder, LPITEMIDLIST pidlParent)
{
	ASSERT_VALID(m_pShellManager);

	LPENUMIDLIST pEnum = nullptr;
	HRESULT hRes = pParentFolder->EnumObjects(nullptr, m_tfEnumConfig, &pEnum);

	if (SUCCEEDED(hRes) && pEnum != nullptr) {

		DWORD dwFetched = 1;
		LPITEMIDLIST pidlTemp;
		LPAFX_SHELLITEMINFO pItem;

		while (pEnum->Next(1, &pidlTemp, &dwFetched) == S_OK && dwFetched) {

			pParentFolder->AddRef();

			pItem = (LPAFX_SHELLITEMINFO)GlobalAlloc(GPTR, sizeof(AFX_SHELLITEMINFO));
			pItem->pidlFQ = m_pShellManager->ConcatenateItem(pidlParent, pidlTemp);
			pItem->pParentFolder = pParentFolder;
			pItem->pidlRel = pidlTemp;

			if (!OnEnumObject(pItem)) {
				pParentFolder->Release();
			}
	
			dwFetched = 0;
		}

		pEnum->Release();
	}

	return hRes;
}