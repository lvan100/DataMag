#include "stdafx.h"
#include "DataMag.h"
#include "MainSearch.h"

/**
 * ȫ�ֵ�Ӧ�ó������
 */
CDataMagApp theApp;

/*
 * ȫ�ֵ�Shell������
 */
CShellManager theShellManager;

CDataMagApp::CDataMagApp()
{
	TCHAR szDir[MAX_PATH];
	GetCurrentDirectory(MAX_PATH, szDir);
	PathAppend(szDir, _T("\\DataMag.ini"));

	m_pszProfileName = _tcsdup(szDir);
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

	// ����ڳ�����ʹ���� CMFCButton �ȿؼ�
	// ��Ҫ�ֶ��ͷ� CMFCVisualManager ����.
	CMFCVisualManager::DestroyInstance();

	return FALSE;
}

CString CDataMagApp::GetCodeMagDir()
{
	TCHAR szDir[MAX_PATH];
	GetCurrentDirectory(MAX_PATH, szDir);

	PathAppend(szDir, _T("\\..\\Դ��"));

	CString strPath = theApp.GetProfileString(_T("Settings"), _T("CodeDir"), szDir);

	if (!PathFileExists(strPath))
	{
		CreateDirectory(strPath, NULL);
	}

	return strPath;
}

void CDataMagApp::SetCodeMagDir(CString dir)
{
	if (GetCodeMagDir().CompareNoCase(dir) != 0)
	{
		theApp.WriteProfileString(_T("Settings"), _T("CodeDir"), dir);

		for (auto iter = codeMagDirChangeListener.begin()
				; iter != codeMagDirChangeListener.end()
				; iter++)
		{
			(*iter)(dir);
		}
	}
}

CString CDataMagApp::GetBookMagDir()
{
	TCHAR szDir[MAX_PATH];
	GetCurrentDirectory(MAX_PATH, szDir);

	PathAppend(szDir, _T("\\..\\ͼ��"));

	CString strPath = theApp.GetProfileString(_T("Settings"), _T("BookDir"), szDir);

	if (!PathFileExists(strPath))
	{
		CreateDirectory(strPath, NULL);
	}

	return strPath;
}

void CDataMagApp::SetBookMagDir(CString dir)
{
	if (GetBookMagDir().CompareNoCase(dir) != 0)
	{
		theApp.WriteProfileString(_T("Settings"), _T("BookDir"), dir);

		for (auto iter = bookMagDirChangeListener.begin()
			; iter != bookMagDirChangeListener.end()
			; iter++)
		{
			(*iter)(dir);
		}
	}
}

CString CDataMagApp::GetLabelMagDir()
{
	TCHAR szDir[MAX_PATH];
	GetCurrentDirectory(MAX_PATH, szDir);

	PathAppend(szDir, _T("\\..\\��ǩ"));

	CString strPath = theApp.GetProfileString(_T("Settings"), _T("LabelDir"), szDir);

	if (!PathFileExists(strPath))
	{
		CreateDirectory(strPath, NULL);
	}

	return strPath;
}

void CDataMagApp::SetLabelMagDir(CString dir)
{
	if (GetLabelMagDir().CompareNoCase(dir) != 0)
	{
		theApp.WriteProfileString(_T("Settings"), _T("LabelDir"), dir);

		for (auto iter = labelMagDirChangeListener.begin()
			; iter != labelMagDirChangeListener.end()
			; iter++)
		{
			(*iter)(dir);
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

			CString strFile = theApp.GetProfileString(_T("RecentFile"), recentFileIndex, NULL);
			if (strFile.GetLength() > 0) {
				recentFileList.push_back(strFile);
			}
		}
	}

	return recentFileList;
}

void CDataMagApp::SetRecentFile(CString file)
{
	if (recentFileList.size() == MaxRecentFileCount) {
		recentFileList.pop_back();
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
		(*iter)();
	}
}

