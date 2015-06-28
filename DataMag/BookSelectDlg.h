#pragma once

#include "ShellListCtrl.h"

/**
 * 图书选择对话框
 */
class CBookSelectDlg : public CDialog, CShellListEventAdapter
{
	DECLARE_DYNAMIC(CBookSelectDlg)

public:
	CBookSelectDlg(CWnd* pParent = nullptr);
	virtual ~CBookSelectDlg();

	enum { IDD = IDD_BOOKSELECTDLG };

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
	 * 被选择的图书列表
	 */
	CStringArray arrBook;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);

protected:
	CEdit m_book_search_edit;
	CShellListCtrl m_book_list;

	DECLARE_MESSAGE_MAP()
protected:
	afx_msg void OnBnClickedOk();
	afx_msg void OnChangeBookSearchEdit();
};