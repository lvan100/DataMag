#pragma once

#include "ShellListCtrl.h"

/**
 * 项目标签页
 */
class CProjectTab : public CDialogEx, CShellListEventAdapter
{
	DECLARE_DYNAMIC(CProjectTab)

public:
	CProjectTab(CWnd* pParent = NULL);
	virtual ~CProjectTab();

	enum { IDD = IDD_PROJECTTAB };

	/**
	 * 响应项目管理目录改变事件
	 */
	void OnCodeMagDirChange(CString dir){
		m_project_list.DisplayFolder(dir);
	}

public:
	/**
	 * 初始化列表控件
	 */
	virtual void InitShellList();

	/**
	 * 列表项双击事件
	 */
	virtual void OnDoubleClick();
	
	/**
	 * 列表项发生变化
	 */
	virtual void OnSelectChanged();

protected:
	/**
	 * 描述文件的内容
	 */
	CStringA strText;

protected:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	virtual void DoDataExchange(CDataExchange* pDX);

protected:
	CRichEditCtrl m_item_text;
	CEdit m_project_search_edit;
	CShellListCtrl m_project_list;

	DECLARE_MESSAGE_MAP()
protected:
	afx_msg void OnChangeProjectSearchEdit();
};