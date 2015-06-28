#pragma once

#include "ShellListCtrl.h"

/**
 * 图书标签页
 */
class CBookTab : public CDialogEx, CShellListEventAdapter
{
	DECLARE_DYNAMIC(CBookTab)

public:
	CBookTab(CWnd* pParent = NULL);
	virtual ~CBookTab();

	enum { IDD = IDD_BOOKTAB };

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
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	virtual void DoDataExchange(CDataExchange* pDX);

protected:
	CEdit m_book_search_edit;
	CShellListCtrl m_book_list;

	DECLARE_MESSAGE_MAP()
protected:
	afx_msg void OnChangeBookSearchEdit();
};
