#pragma once

#include "ShellListCtrl.h"

/**
 * 项目选择对话框
 */
class CProjectSelectDlg : public CDialog, CShellListEventAdapter
{
	DECLARE_DYNAMIC(CProjectSelectDlg)

public:
	CProjectSelectDlg(CWnd* pParent = nullptr);
	virtual ~CProjectSelectDlg();

	enum { IDD = IDD_PROJECTSELECTDLG };

public:
	/**
	 * 初始化列表控件
	 */
	virtual void InitShellList();

	/**
	 * 列表项双击事件
	 */
	virtual void OnDoubleClick();

public:
	/**
	 * 被选择的项目列表
	 */
	CStringArray arrProject;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);

protected:
	CEdit m_project_search_edit;
	CShellListCtrl m_project_list;

	DECLARE_MESSAGE_MAP()
protected:
	afx_msg void OnBnClickedOk();
	afx_msg void OnChangeProjectSearchEdit();
};