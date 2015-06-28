#pragma once

#include "resource.h"

/**
 * 配置文件
 */
class CSetting
{
public:
	/**
	 * 被管理的目录
	 */
	CString strMagFolder;

public:
	/**
	 * 加载配置文件
	 *
	 * @param strFile
	 *        配置文件的（绝对）路径
	 */
	void Load(CString strFile);

	/**
	 * 保存配置文件
	 */
	void Save();

protected:
	/**
	 * 配置文件路径
	 */
	CString strIniFile;
};

/**
 * 定义全局的配置对象
 */
extern CSetting theSetting;

/**
 * 配置对话框
 */
class CSettingDlg : public CDialog
{
	DECLARE_DYNAMIC(CSettingDlg)

public:
	CSettingDlg(CWnd* pParent = nullptr);
	virtual ~CSettingDlg();

	enum { IDD = IDD_SETTINGDLG };

protected:
	virtual BOOL OnInitDialog();
	virtual void DoDataExchange(CDataExchange* pDX);

public:
	CMFCEditBrowseCtrl m_folder_select;

	DECLARE_MESSAGE_MAP()
protected:
	afx_msg void OnBnClickedOk();
};