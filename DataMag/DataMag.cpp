#include "stdafx.h"
#include "DataMag.h"
#include "DataMagDlg.h"
#include "SettingDlg.h"

CDataMagApp::CDataMagApp()
{
}

CDataMagApp theApp;

CShellManager* afxShellManager;

BOOL CDataMagApp::InitInstance()
{
	INITCOMMONCONTROLSEX InitCtrls;
	InitCtrls.dwSize = sizeof(InitCtrls);
	InitCtrls.dwICC = ICC_WIN95_CLASSES;
	InitCommonControlsEx(&InitCtrls);

	CWinApp::InitInstance();

	AutoCoInitialize autoCom;

	if (!AfxSocketInit())
	{
		return FALSE;
	}
	
	AfxEnableControlContainer();

	AfxInitRichEdit2();

	CShellManager* pShellManager;
	pShellManager = new CShellManager;
	afxShellManager = pShellManager;

	TCHAR szDir[MAX_PATH];
	GetCurrentDirectory(MAX_PATH, szDir);
	PathAppend(szDir, _T("\\DataMag.ini"));

	theSetting.Load(szDir);

	CDataMagDlg dlg;
	dlg.DoModal();

	if (pShellManager != NULL)
	{
		delete pShellManager;
	}

	return FALSE;
}