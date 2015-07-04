#include "StdAfx.h"
#include "FileOp.h"

/**
 * ɾ��Ŀ¼�����õݹ�ɾ���ļ��ķ�ʽ
 *
 * @param strDir
 *        Ŀ¼
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
 * ��ȡ�ļ���ݷ�ʽ��Ŀ���ַ
 *
 * @param strPath
 *        �ļ���ַ
 * @param strLink
 *        �ļ���ݷ�ʽ�ĵ�ַ
 * @return �ɹ����� TRUE��ʧ�ܷ��� FALSE��
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
 * �����ļ���ݷ�ʽ
 *
 * @param strPath
 *        �ļ���ַ
 * @param strLink
 *        �ļ���ݷ�ʽ�ĵ�ַ
 * @return �ɹ����� TRUE��ʧ�ܷ��� FALSE��
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