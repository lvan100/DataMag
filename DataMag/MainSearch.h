#pragma once

#include "AppWnd.h"

#include "FolderList.h"
#include "SearchEdit.h"
#include "PrettyButton.h"

class CMainSearch : public CAppWnd
{
	DECLARE_DYNAMIC(CMainSearch)

public:
	CMainSearch(CWnd* pParent = nullptr);
	virtual ~CMainSearch();

	enum { IDD = IDD_MAINSEARCH };

protected:
	/**
	 * 最近访问列表事件对象
	 */
	class RecentListEvent: public CListBoxEventAdapter
	{
	public:
		/**
		 * 列表项双击事件
		 */
		virtual void OnDoubleClick();

	}m_recent_list_event;

	/**
	 * 最近访问列表事件对象
	 */
	class RecommandListEvent: public CListBoxEventAdapter
	{
	public:
		/**
		 * 列表项双击事件
		 */
		virtual void OnDoubleClick();

	}m_recommand_list_event;

public:
	/**
	 * 开始搜索
	 */
	void DoSearch();

	/**
	 * 完成推荐
	 */
	void DoRecommand();

	/**
	 * 响应最近访问列表变化
	 */
	void OnRecentListChange();

public:
	/**
	 * 获取如果可见的区域
	 */
	CRect GetIfVisiableRect() const
	{ return m_rect_if_visiable; }

	/**
	 * 设置如果可见区域
	 */
	void SetIfVisiableRect(CRect rc)
	{ m_rect_if_visiable = rc; }

protected:
	/**
	 * 如果可视化的区域
	 */
	CRect m_rect_if_visiable;

	/**
	 * 移动窗口到不可见区域，以达到隐藏窗口的目的
	 */
	void MoveToHideWindow(BOOL bHide);

protected:
	/**
	 * 随机推荐列表
	 */
	vector<CString> m_recommand_values;

protected:
	/**
	 * 获取默认焦点控件
	 */
	virtual CWnd* GetDefaultFocusWnd()
	{ return &m_code_search; }

protected:
	CStatic m_recent_group;
	CStatic m_recommand_group;
	CFolderListCtrl m_recent_list;
	CFolderListCtrl m_recommand_list;

	CPrettyButton m_add_tag;
	CPrettyButton m_add_book;
	CPrettyButton m_add_project;

	CSearchEdit m_tag_search;
	CSearchEdit m_book_search;
	CSearchEdit m_code_search;

protected:
	virtual BOOL OnInitDialog();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	virtual void DoDataExchange(CDataExchange* pDX);

	DECLARE_MESSAGE_MAP()
protected:
	afx_msg void OnBnClickedAddTag();
	afx_msg void OnMove(int x, int y);
	afx_msg void OnBnClickedAddBook();
	afx_msg void OnBnClickedAddProject();
};

/**
 * 全局的主搜索对话框对象
 */
extern CMainSearch* theMainSearch;