#include "StdAfx.h"
#include "FileEnum.h"

CFileEnum::CFileEnum(CShellManager* pShellManager)
	: m_pShellManager(pShellManager)
	, m_psfCurFolder(nullptr)
	, m_pidlCurFQ(nullptr)
	, m_bIsDesktop(FALSE)
{
	m_tfEnumConfig = (SHCONTF)(SHCONTF_FOLDERS | SHCONTF_NONFOLDERS);
}

CFileEnum::~CFileEnum(void)
{
}

CString CFileEnum::GetCurrentFolder()
{
	if (m_pidlCurFQ == nullptr) {
		return _T("");
	}

	TCHAR szPath [MAX_PATH + 1] = { 0 };
	if (SHGetPathFromIDList(m_pidlCurFQ, szPath)) {
		return szPath;
	}
	return _T("");
}

CString CFileEnum::GetCurrentFolderName()
{
	if (m_pidlCurFQ == nullptr) {
		return _T("");
	}

	SHFILEINFO sfi;
	if (SHGetFileInfo((LPCTSTR)m_pidlCurFQ, 0, &sfi, sizeof(sfi), SHGFI_PIDL | SHGFI_DISPLAYNAME))	{
		return sfi.szDisplayName;
	}
	return _T("");
}

BOOL CFileEnum::Refresh()
{
	return SUCCEEDED(DisplayFolder((LPAFX_SHELLITEMINFO) nullptr));
}

BOOL CFileEnum::DisplayParentFolder()
{
	ASSERT_VALID(m_pShellManager);

	if (m_pidlCurFQ == nullptr)	{
		return FALSE;
	}

	AFX_SHELLITEMINFO info;
	int nLevel = m_pShellManager->GetParentItem(m_pidlCurFQ, info.pidlFQ);
	if (nLevel < 0)	{
		return FALSE;
	}

	HRESULT hr = E_FAIL;

	if (nLevel == 0) {
		// 整个文件系统的根节点
		hr = DisplayFolder(&info);

	} else {
		// 获取文件系统根节点的 Shell 操作接口
		LPSHELLFOLDER pDesktopFolder = nullptr;
		hr = SHGetDesktopFolder(&pDesktopFolder);

		if (SUCCEEDED(hr)) {

			info.pParentFolder = pDesktopFolder;
			info.pidlRel = info.pidlFQ;

			hr = DisplayFolder(&info);
			pDesktopFolder->Release();
		}
	}

	m_pShellManager->FreeItem(info.pidlFQ);
	return SUCCEEDED(hr);
}

BOOL CFileEnum::DisplayFolder(LPCTSTR lpszPath)
{
	ENSURE(lpszPath != nullptr);
	ASSERT_VALID(m_pShellManager);

	AFX_SHELLITEMINFO info;
	HRESULT hr = m_pShellManager->ItemFromPath(lpszPath, info.pidlRel);
	if (FAILED(hr))	{
		return FALSE;
	}

	// 获取文件系统根节点 Shell 操作接口
	LPSHELLFOLDER pDesktopFolder = nullptr;
	hr = SHGetDesktopFolder(&pDesktopFolder);

	if (SUCCEEDED(hr)) {

		info.pParentFolder = pDesktopFolder;
		info.pidlFQ = info.pidlRel;

		hr = DisplayFolder(&info);
		pDesktopFolder->Release();
	}

	m_pShellManager->FreeItem(info.pidlRel);
	return SUCCEEDED(hr);
}

BOOL CFileEnum::DisplayFolder(LPAFX_SHELLITEMINFO pItemInfo)
{
	ASSERT_VALID(m_pShellManager);

	HRESULT hr = E_FAIL;

	if (pItemInfo != nullptr) {
		ReleaseCurrFolder();

		hr = LockCurrentFolder(pItemInfo);
		if (FAILED(hr)) {
			return FALSE;
		}
	}

	OnDisplayFolderInit();

	if (m_psfCurFolder != nullptr) {

		OnDisplayFolderBefore();
		{
			hr = EnumObjects(m_psfCurFolder, m_pidlCurFQ);
		}
		OnDisplayFolderAfter();
	}

	return SUCCEEDED(hr);
}

HRESULT CFileEnum::LockCurrentFolder(LPAFX_SHELLITEMINFO pItemInfo)
{
	ASSERT_VALID(m_pShellManager);

	HRESULT hr = E_FAIL;

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

	return SUCCEEDED(hr);
}

void CFileEnum::ReleaseCurrFolder()
{
	ASSERT_VALID(m_pShellManager);

	if (m_psfCurFolder != nullptr) {
		m_psfCurFolder->Release();
		m_psfCurFolder = nullptr;

		m_pShellManager->FreeItem(m_pidlCurFQ);
		m_pidlCurFQ = nullptr;
	}
}

CString CFileEnum::GetItemText(LPAFX_SHELLITEMINFO pItem)
{
	ENSURE(pItem != nullptr);

	SHFILEINFO sfi;

	if (SHGetFileInfo((LPCTSTR)pItem->pidlFQ, 0, &sfi, sizeof(sfi), SHGFI_PIDL | SHGFI_DISPLAYNAME)) {
		return sfi.szDisplayName;
	}

	return _T("");
}

HRESULT CFileEnum::EnumObjects(LPSHELLFOLDER pParentFolder, LPITEMIDLIST pidlParent)
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
				m_pShellManager->FreeItem(pItem->pidlRel);
				m_pShellManager->FreeItem(pItem->pidlFQ);
				GlobalFree((HGLOBAL) pItem);
				pParentFolder->Release();
			}
	
			dwFetched = 0;
		}

		pEnum->Release();
	}

	return hRes;
}