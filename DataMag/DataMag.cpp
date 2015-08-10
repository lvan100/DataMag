#include "stdafx.h"
#include "DataMag.h"
#include "MainSearch.h"

/**
 * ȫ�ֵ�Ӧ�����ö���
 */
CSetting theSetting;

CSetting::CSetting()
{
}

CString CSetting::GetCodeMagDir()
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

void CSetting::SetCodeMagDir(CString dir)
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

CString CSetting::GetBookMagDir()
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

void CSetting::SetBookMagDir(CString dir)
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

CString CSetting::GetLabelMagDir()
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

void CSetting::SetLabelMagDir(CString dir)
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

const vector<CString>& CSetting::GetRecentFileList()
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

void CSetting::SetRecentFile(CString file)
{
	if (recentFileList.size() == MaxRecentFileCount) {
		recentFileList.pop_back();
	}

	recentFileList.insert(recentFileList.begin(), file);

	for (int i = 0; i < recentFileList.size(); i++) {

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

/************************************************************************/
/*							  CDataMagApp                               */
/************************************************************************/

CDataMagApp::CDataMagApp()
{
	TCHAR szDir[MAX_PATH];
	GetCurrentDirectory(MAX_PATH, szDir);
	PathAppend(szDir, _T("\\DataMag.ini"));

	m_pszProfileName = _tcsdup(szDir);
}

/**
 * ȫ�ֵ�Ӧ�ó������
 */
CDataMagApp theApp;

/*
 * ȫ�ֵ�Shell������
 */
CShellManager theShellManager;

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