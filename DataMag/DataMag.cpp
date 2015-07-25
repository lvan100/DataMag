#include "stdafx.h"
#include "DataMag.h"
#include "MainSearch.h"

/**
 * 全局的应用设置对象
 */
CSetting theSetting;

CString CSetting::GetCodeMagDir()
{
	TCHAR szDir[MAX_PATH];
	GetCurrentDirectory(MAX_PATH, szDir);

	PathAppend(szDir, _T("\\源码"));

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

	PathAppend(szDir, _T("\\图书"));

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

	PathAppend(szDir, _T("\\标签"));

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
 * 全局的应用程序对象
 */
CDataMagApp theApp;

/*
 * 全局的Shell管理器
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

	return FALSE;
}