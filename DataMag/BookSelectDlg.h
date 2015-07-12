#pragma once

#include "SearchEdit.h"
#include "FileListBox.h"

/**
 * 图书选择对话框
 */
class CBookSelectDlg : public CDialog, CListBoxEventAdapter
{
	DECLARE_DYNAMIC(CBookSelectDlg)

public:
	CBookSelectDlg(CWnd* pParent = nullptr);
	virtual ~CBookSelectDlg();

	enum { IDD = IDD_BOOKSELECTDLG };

public:
	/**
	 * 初始化列表框控件
	 */
	virtual void InitListBox();

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
	CFileListBox m_book_list;
	CSearchEdit m_search_edit;

	DECLARE_MESSAGE_MAP()
protected:
	afx_msg void OnBnClickedOk();
	afx_msg void OnChangeBookSearchEdit();
};