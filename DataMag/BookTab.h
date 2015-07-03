#pragma once

#include "PrettyEdit.h"
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
	CPrettyEdit m_search_edit;
	CRichEditCtrl m_item_text;
	CShellListCtrl m_book_list;

	DECLARE_MESSAGE_MAP()
protected:
	afx_msg void OnChangeBookSearchEdit();
};
