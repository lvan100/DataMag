#include "stdafx.h"
#include "DataMag.h"
#include "MainSearch.h"

/**
 * 全局的应用程序对象
 */
CDataMagApp theApp;

/*
 * 全局的Shell管理器
 */
CShellManager theShellManager;

CDataMagApp::CDataMagApp()
	: m_hSearchIcon(nullptr)
{
	TCHAR szDir[MAX_PATH];
	GetCurrentDirectory(MAX_PATH, szDir);
	PathAppend(szDir, _T("\\DataMag.ini"));

	m_pszProfileName = _tcsdup(szDir);
}

HICON CDataMagApp::GetSearchIcon()
{
	if (m_hSearchIcon == nullptr)
	{
		m_hSearchIcon = (HICON)LoadImage(AfxGetInstanceHandle()
			, MAKEINTRESOURCE(IDI_SEARCH)
			, IMAGE_ICON, 0, 0, 0);
	}
	return m_hSearchIcon;
}

BOOL CDataMagApp::InitInstance()
{
	INITCOMMONCONTROLSEX InitCtrls;
	InitCtrls.dwSize = sizeof(InitCtrls);
	InitCtrls.dwICC = ICC_WIN95_CLASSES;
	InitCommonControlsEx(&InitCtrls);

	CWinApp::InitInstance();

	AfxEnableControlContainer();

	AutoCoInitialize autoCom;

	if (!AfxSocketInit())
	{
		return FALSE;
	}

	AfxInitRichEdit2();

	CMainSearch dlg;
	dlg.DoModal();

	// 如果在程序中使用了 CMFCButton 等控件
	// 需要手动释放 CMFCVisualManager 对象.
	CMFCVisualManager::DestroyInstance();

	return FALSE;
}

CString CDataMagApp::GetCodeDir()
{
	TCHAR szDir[MAX_PATH];
	GetCurrentDirectory(MAX_PATH, szDir);

	PathAppend(szDir, _T("\\..\\源码"));

	CString strPath = theApp.GetProfileString(_T("Settings"), _T("CodeDir"), szDir);

	if (!PathFileExists(strPath))
	{
		CreateDirectory(strPath, nullptr);
	}

	return strPath;
}

void CDataMagApp::SetCodeDir(CString dir)
{
	if (GetCodeDir().CompareNoCase(dir) != 0)
	{
		theApp.WriteProfileString(_T("Settings"), _T("CodeDir"), dir);

		for (auto iter = codeDirChangeListener.begin()
				; iter != codeDirChangeListener.end()
				; iter++)
		{
			(*iter).second(dir);
		}
	}
}

CString CDataMagApp::GetBookDir()
{
	TCHAR szDir[MAX_PATH];
	GetCurrentDirectory(MAX_PATH, szDir);

	PathAppend(szDir, _T("\\..\\图书"));

	CString strPath = theApp.GetProfileString(_T("Settings"), _T("BookDir"), szDir);

	if (!PathFileExists(strPath))
	{
		CreateDirectory(strPath, nullptr);
	}

	return strPath;
}

void CDataMagApp::SetBookDir(CString dir)
{
	if (GetBookDir().CompareNoCase(dir) != 0)
	{
		theApp.WriteProfileString(_T("Settings"), _T("BookDir"), dir);

		for (auto iter = bookDirChangeListener.begin()
			; iter != bookDirChangeListener.end()
			; iter++)
		{
			(*iter).second(dir);
		}
	}
}

CString CDataMagApp::GetTagDir()
{
	TCHAR szDir[MAX_PATH];
	GetCurrentDirectory(MAX_PATH, szDir);

	PathAppend(szDir, _T("\\..\\标签"));

	CString strPath = theApp.GetProfileString(_T("Settings"), _T("TagDir"), szDir);

	if (!PathFileExists(strPath))
	{
		CreateDirectory(strPath, nullptr);
	}

	return strPath;
}

void CDataMagApp::SetTagDir(CString dir)
{
	if (GetTagDir().CompareNoCase(dir) != 0)
	{
		theApp.WriteProfileString(_T("Settings"), _T("TagDir"), dir);

		for (auto iter = tagDirChangeListener.begin()
			; iter != tagDirChangeListener.end()
			; iter++)
		{
			(*iter).second(dir);
		}
	}
}

const vector<CString>& CDataMagApp::GetRecentFileList()
{
	if (recentFileList.size() == 0)
	{
		for (int i = 0; i < MaxRecentFileCount; i++)
		{
			CString recentFileIndex;
			recentFileIndex.Format(_T("File%d"), i);

			CString strFile = theApp.GetProfileString(_T("RecentFile"), recentFileIndex, nullptr);
			if (strFile.GetLength() > 0) {
				recentFileList.push_back(strFile);
			}
		}
	}

	return recentFileList;
}

void CDataMagApp::SetRecentFile(CString file)
{
	auto find_iter = recentFileList.begin();

	for (;find_iter != recentFileList.end(); find_iter++) {
		if ((*find_iter).CompareNoCase(file) == 0) {
			break;
		}
	}
	
	if (find_iter == recentFileList.end()) {
		if (recentFileList.size() == MaxRecentFileCount) {
			recentFileList.pop_back();
		}
	} else {
		recentFileList.erase(find_iter);
	}

	recentFileList.insert(recentFileList.begin(), file);

	for (size_t i = 0; i < recentFileList.size(); i++) {

		CString recentFileIndex;
		recentFileIndex.Format(_T("File%d"), i);

		theApp.WriteProfileString(_T("RecentFile"), recentFileIndex, recentFileList.at(i));
	}

	for (auto iter = recentListChangeListener.begin()
		; iter != recentListChangeListener.end()
		; iter++)
	{
		(*iter).second();
	}
}