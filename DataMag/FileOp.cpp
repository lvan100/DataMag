#include "StdAfx.h"
#include "FileOp.h"

/**
 * 删除目录，采用递归删除文件的方式
 *
 * @param strDir
 *        目录
 */
void DeleteDirectory(CString strDir)
{
	CFileFind fileFind;

	BOOL IsFinded = fileFind.FindFile(strDir + _T("\\*.*"));
	IsFinded = fileFind.FindNextFile();	// .
	IsFinded = fileFind.FindNextFile();	// ..

	while (IsFinded)
	{
		IsFinded = fileFind.FindNextFile();
		DeleteFile(fileFind.GetFilePath());
	}

	fileFind.Close();

	RemoveDirectory(strDir);
}

/**
 * 获取文件快捷方式的目标地址
 *
 * @param strPath
 *        文件地址
 * @param strLink
 *        文件快捷方式的地址
 * @return 成功返回 TRUE，失败返回 FALSE。
 */
BOOL GetLinkFilePath(CString& strPath, CString strLink)
{
	HRESULT hResult = S_FALSE;

	IShellLink* pShellLink = nullptr;
	AutoRelease<IShellLink*> tmp1(pShellLink);

	IPersistFile* pPersistFile = nullptr;
	AutoRelease<IPersistFile*> tmp2(pPersistFile);

	hResult = CoCreateInstance(CLSID_ShellLink
		, nullptr
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

	hResult = pShellLink->Resolve(nullptr, SLR_ANY_MATCH);
	if(FAILED(hResult))
	{
		return FALSE;
	}

	WCHAR szPath[MAX_PATH];

	hResult = pShellLink->GetPath(szPath, MAX_PATH, nullptr, SLGP_SHORTPATH); 
	if(FAILED(hResult))
	{
		return FALSE;
	}

	strPath.SetString(szPath);

	return TRUE; 
}

/**
 * 创建文件快捷方式
 *
 * @param strPath
 *        文件地址
 * @param strLink
 *        文件快捷方式的地址
 * @return 成功返回 TRUE，失败返回 FALSE。
 */
BOOL CreateFileLink(CString strPath, CString strLink)
{
	HRESULT hResult = S_FALSE;

	IShellLink* pShellLink = nullptr;
	AutoRelease<IShellLink*> tmp1(pShellLink);

	IPersistFile* pPersistFile = nullptr;
	AutoRelease<IPersistFile*> tmp2(pPersistFile);

	hResult = CoCreateInstance(CLSID_ShellLink
		, nullptr
		, CLSCTX_INPROC_SERVER
		, IID_IShellLink
		, (void**)&pShellLink);  
	if(FAILED(hResult))
	{
		return FALSE;
	}

	pShellLink->SetPath(strPath);

	hResult = pShellLink->QueryInterface(IID_IPersistFile
		, (void**)&pPersistFile);
	if(FAILED(hResult))
	{
		return FALSE;
	}

	hResult = pPersistFile->Save(strLink, STGM_DIRECT);

	return TRUE;   
}