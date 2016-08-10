#include "StdAfx.h"
#include "WindowsFileNode.h"

WindowsFileNode::WindowsFileNode(CShellManager* pShellManager, wstring path)
	: FileNode(path)
	, m_pShellManager(pShellManager)
{
	ASSERT_VALID(m_pShellManager);
}

WindowsFileNode::~WindowsFileNode(void)
{}

bool WindowsFileNode::Enum(FileEnumConfig config, function<bool(shared_ptr<FileNode>)> filter)
{
	LPITEMIDLIST pidlCurPath = nullptr;
	HRESULT hr = m_pShellManager->ItemFromPath(_path.c_str(), pidlCurPath);
	if (FAILED(hr))	{
		return false;
	}

	LPSHELLFOLDER pDesktopFolder = nullptr;
	hr = SHGetDesktopFolder(&pDesktopFolder);
	if (FAILED(hr))	{
		m_pShellManager->FreeItem(pidlCurPath);
		return false;
	}

	IShellFolder* psfCurFolder = nullptr;
	hr = pDesktopFolder->BindToObject(pidlCurPath, nullptr, IID_IShellFolder, (LPVOID*)& psfCurFolder);
	if (FAILED(hr))	{
		m_pShellManager->FreeItem(pidlCurPath);
		pDesktopFolder->Release();
		return false;
	}

	SHCONTF enumConfig = 0x0000;
	
	if (config & Include_Children) {
		enumConfig |= SHCONTF_CHECKING_FOR_CHILDREN;
	}
	
	if (config & Include_Folder) {
		enumConfig |= SHCONTF_FOLDERS;
	}

	if (config & Include_File) {
		enumConfig |= SHCONTF_NONFOLDERS;
	}

	LPENUMIDLIST pEnum = nullptr;
	HRESULT hRes = psfCurFolder->EnumObjects(nullptr, config, &pEnum);
	if (FAILED(hr))	{
		m_pShellManager->FreeItem(pidlCurPath);
		pDesktopFolder->Release();
		psfCurFolder->Release();
		return false;
	}

	DWORD dwFetched = 1;
	LPITEMIDLIST pidlTemp;
	LPAFX_SHELLITEMINFO pItem;

	while (pEnum->Next(1, &pidlTemp, &dwFetched) == S_OK && dwFetched) {

		psfCurFolder->AddRef();

		pItem = (LPAFX_SHELLITEMINFO)GlobalAlloc(GPTR, sizeof(AFX_SHELLITEMINFO));
		pItem->pidlFQ = m_pShellManager->ConcatenateItem(pidlCurPath, pidlTemp);
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

	pDesktopFolder->Release();


	m_pShellManager->FreeItem(info.pidlRel);

	return hRes;
}