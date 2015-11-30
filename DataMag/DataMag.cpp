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

/**
 * 获取当前目录下的文件
 */
STATIC CString GetCurrentDirectoryFile(CString strFileName)
{
	TCHAR szDir[MAX_PATH] = { 0 };
	GetCurrentDirectory(MAX_PATH, szDir);
	PathAppend(szDir, strFileName);
	return szDir;
}

CDataMagApp::CDataMagApp()
	: m_hAppIcon(nullptr)
	, m_hSearchIcon(nullptr)
{
	m_pszProfileName = _tcsdup(GetCurrentDirectoryFile(_T("\\DataMag.ini")));
}

HICON CDataMagApp::GetAppIcon()
{
	if (m_hAppIcon == nullptr) {
		m_hAppIcon = (HICON)LoadImage(AfxGetInstanceHandle()
			, MAKEINTRESOURCE(IDR_MAINFRAME)
			, IMAGE_ICON, 0, 0, 0);
	}
	return m_hAppIcon;
}

HICON CDataMagApp::GetSearchIcon()
{
	if (m_hSearchIcon == nullptr) {
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

	if (!AfxSocketInit()) {
		ASSERT(FALSE);
		return FALSE;
	}

	if (!AfxInitRichEdit2()) {
		ASSERT(FALSE);
		return FALSE;
	}

	// VS2015 通过函数初始化 afxGlobalData 变量.
	/* AFX_GLOBAL_DATA* p = */ GetGlobalData();

	CMainSearch().DoModal();

	return FALSE;
}

int CDataMagApp::ExitInstance()
{
	// 如果在程序中使用了 CMFCButton 等控件
	// 需要手动释放 CMFCVisualManager 对象.
	CMFCVisualManager::DestroyInstance();

	return CWinApp::ExitInstance();
}

CString CDataMagApp::GetSettingDirectory(CString strType, CString strDefaultPath)
{
	CString strPath = GetCurrentDirectoryFile(strDefaultPath);
	strPath = GetProfileString(_T("Settings"), strType, strPath);

	if (!PathFileExists(strPath)) {
		CreateDirectory(strPath, nullptr);
	}

	return strPath;
}

CString CDataMagApp::GetCodeDir()
{
	return GetSettingDirectory(_T("CodeDir"), _T("..\\源码"));
}

void CDataMagApp::SetCodeDir(CString dir)
{
	if (GetCodeDir().CompareNoCase(dir) != 0)
	{
		WriteProfileString(_T("Settings"), _T("CodeDir"), dir);

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
	return GetSettingDirectory(_T("BookDir"), _T("..\\图书"));
}

void CDataMagApp::SetBookDir(CString dir)
{
	if (GetBookDir().CompareNoCase(dir) != 0)
	{
		WriteProfileString(_T("Settings"), _T("BookDir"), dir);

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
	return GetSettingDirectory(_T("TagDir"), _T("..\\标签"));
}

void CDataMagApp::SetTagDir(CString dir)
{
	if (GetTagDir().CompareNoCase(dir) != 0)
	{
		WriteProfileString(_T("Settings"), _T("TagDir"), dir);

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

			CString strFile = GetProfileString(_T("RecentFile"), recentFileIndex, _T(""));
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

		WriteProfileString(_T("RecentFile"), recentFileIndex, recentFileList.at(i));
	}

	for (auto iter = recentListChangeListener.begin()
		; iter != recentListChangeListener.end()
		; iter++)
	{
		(*iter).second();
	}
}

void CDataMagApp::RemoveRecentFile(CString file)
{
	auto find_iter = recentFileList.begin();

	for (;find_iter != recentFileList.end(); find_iter++) {
		if ((*find_iter).CompareNoCase(file) == 0) {
			break;
		}
	}

	if (find_iter != recentFileList.end()) {
		recentFileList.erase(find_iter);
	} else {
		return;
	}

	for (size_t i = 0; i < recentFileList.size(); i++) {

		CString recentFileIndex;
		recentFileIndex.Format(_T("File%d"), i);

		WriteProfileString(_T("RecentFile"), recentFileIndex, recentFileList.at(i));
	}

	for (size_t i = recentFileList.size(); i < MaxRecentFileCount; i++) {

		CString recentFileIndex;
		recentFileIndex.Format(_T("File%d"), i);

		WriteProfileString(_T("RecentFile"), recentFileIndex, _T(""));
	}

	for (auto iter = recentListChangeListener.begin()
		; iter != recentListChangeListener.end()
		; iter++)
	{
		(*iter).second();
	}
}