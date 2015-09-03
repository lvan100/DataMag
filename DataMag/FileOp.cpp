#include "StdAfx.h"
#include "FileOp.h"

void DeleteDirectory(CString strDir)
{
	CFileFind fileFind;

	BOOL IsFinded = fileFind.FindFile(strDir + _T("\\*.*"));
	IsFinded = fileFind.FindNextFile();	// .
	IsFinded = fileFind.FindNextFile();	// ..

	while (IsFinded)
	{
		IsFinded = fileFind.FindNextFile();

		CString strFilePath = fileFind.GetFilePath();
		if (PathIsDirectory(strFilePath)) {
			DeleteDirectory(strFilePath);
		} else {
			DeleteFile(strFilePath);
		}
	}

	fileFind.Close();

	RemoveDirectory(strDir);
}

/**
 * 自动释放 COM 接口
 */
template<typename T>
struct AutoRelease {

	AutoRelease(T& ptr)
		: _ptr(ptr){
	}

	~AutoRelease(){
		if (_ptr != nullptr){
			_ptr->Release();
			_ptr = nullptr;
		}		
	}

protected:
	T& _ptr;
};

BOOL GetLinkFilePath(CString& strPath, CString strLink)
{
	HRESULT hResult = S_FALSE;

	IShellLink* pShellLink = nullptr;
	AutoRelease<IShellLink*> unused1(pShellLink);

	IPersistFile* pPersistFile = nullptr;
	AutoRelease<IPersistFile*> unused2(pPersistFile);

	hResult = CoCreateInstance(CLSID_ShellLink
		, nullptr
		, CLSCTX_INPROC_SERVER
		, IID_IShellLink
		, (void**)&pShellLink); 
	if(FAILED(hResult)) {
		return FALSE;
	}

	hResult = pShellLink->QueryInterface(IID_IPersistFile
		, (void**)&pPersistFile);
	if(FAILED(hResult)) {
		return FALSE;
	}

	hResult = pPersistFile->Load(strLink, STGM_READ);
	if(FAILED(hResult)) {
		return FALSE;
	}

	hResult = pShellLink->Resolve(nullptr, SLR_ANY_MATCH);
	if(FAILED(hResult)) {
		return FALSE;
	}

	WCHAR szPath[MAX_PATH + 1] = { 0 };

	hResult = pShellLink->GetPath(szPath, MAX_PATH, nullptr, SLGP_SHORTPATH); 
	if(FAILED(hResult)) {
		return FALSE;
	}

	strPath.SetString(szPath);
	return TRUE; 
}

BOOL CreateFileLink(CString strPath, CString strLink)
{
	HRESULT hResult = S_FALSE;

	IShellLink* pShellLink = nullptr;
	AutoRelease<IShellLink*> unused1(pShellLink);

	IPersistFile* pPersistFile = nullptr;
	AutoRelease<IPersistFile*> unused2(pPersistFile);

	hResult = CoCreateInstance(CLSID_ShellLink
		, nullptr
		, CLSCTX_INPROC_SERVER
		, IID_IShellLink
		, (void**)&pShellLink);  
	if(FAILED(hResult)) {
		return FALSE;
	}

	hResult = pShellLink->SetPath(strPath);
	if(FAILED(hResult)) {
		return FALSE;
	}

	hResult = pShellLink->QueryInterface(IID_IPersistFile
		, (void**)&pPersistFile);
	if(FAILED(hResult)) {
		return FALSE;
	}

	hResult = pPersistFile->Save(strLink, STGM_DIRECT);
	return SUCCEEDED(hResult);   
}