#pragma once

#include "ShellListCtrl.h"

/**
 * 图书选择对话框
 */
class CBookListDlg : public CDialog, CShellListEventAdapter
{
	DECLARE_DYNAMIC(CBookListDlg)

public:
	CBookListDlg(CWnd* pParent = NULL);
	virtual ~CBookListDlg();

	enum { IDD = IDD_BOOKLISTDLG };

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
	 * 被选择的图书列表
	 */
	CStringArray arrBook;

protected:
	virtual BOOL OnInitDialog();
	virtual void DoDataExchange(CDataExchange* pDX);

protected:
	CEdit m_book_search_edit;
	CShellListCtrl m_book_list;

protected:
	DECLARE_MESSAGE_MAP()
	afx_msg void OnBnClickedOk();
	afx_msg void OnChangeBookSearchEdit();
};
