#pragma once

#include "SearchEdit.h"
#include "FileListBox.h"

/**
 * 图书标签页
 */
class CBookTab : public CDialogEx, CListBoxEventAdapter
{
	DECLARE_DYNAMIC(CBookTab)

public:
	CBookTab(CWnd* pParent = NULL);
	virtual ~CBookTab();

	enum { IDD = IDD_BOOKTAB };

	/**
	 * 响应图书管理目录改变事件
	 */
	void OnBookMagDirChange(CString dir){
		m_book_list.DisplayFolder(dir);
	}

public:
	/**
	 * 初始化列表控件
	 */
	virtual void InitListBox();

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
	/**
	 * 重命名图书
	 */
	void OnRenameBook();

protected:
	virtual BOOL OnInitDialog();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	virtual void DoDataExchange(CDataExchange* pDX);

protected:
	CSearchEdit m_search_edit;
	CRichEditCtrl m_item_text;
	CFileListBox m_book_list;

	DECLARE_MESSAGE_MAP()
protected:
	afx_msg void OnChangeBookSearchEdit();
	afx_msg void OnDropFiles(HDROP hDropInfo);
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
};