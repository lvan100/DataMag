#pragma once

#include "resource.h"

class CSetting
{
public:
	CString strMagFolder;

public:
	void Load(CString strFile);
	void Save();

protected:
	CString strIniFile;
};

extern CSetting theSetting;

class CSettingDlg : public CDialog
{
	DECLARE_DYNAMIC(CSettingDlg)

public:
	CSettingDlg(CWnd* pParent = NULL);
	virtual ~CSettingDlg();

	enum { IDD = IDD_SETTINGDLG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);

	DECLARE_MESSAGE_MAP()
public:
	CMFCEditBrowseCtrl m_folder_select;

protected:
	virtual BOOL OnInitDialog();
public:
	afx_msg void OnBnClickedOk();
};
