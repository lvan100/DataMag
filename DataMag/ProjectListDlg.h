#pragma once

#include "ShellListCtrl.h"

/**
 * 项目选择对话框
 */
class CProjectListDlg : public CDialog, CShellListEventAdapter
{
	DECLARE_DYNAMIC(CProjectListDlg)

public:
	CProjectListDlg(CWnd* pParent = NULL);
	virtual ~CProjectListDlg();

	enum { IDD = IDD_PROJECTLISTDLG };
	
	enum{
		Display,
		Releate,
	}Op;

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
	virtual BOOL OnInitDialog();
	virtual void DoDataExchange(CDataExchange* pDX);

protected:
	CEdit m_project_search_edit;
	CShellListCtrl m_project_list;

protected:
	DECLARE_MESSAGE_MAP()
	afx_msg void OnBnClickedOk();
	afx_msg void OnChangeProjectSearchEdit();
};