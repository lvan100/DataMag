#pragma once

#include "AppWnd.h"
#include "TagTab.h"
#include "BookTab.h"
#include "CodeTab.h"
#include "FolderList.h"
#include "SearchEdit.h"
#include "PrettyButton.h"

/**
 * 主搜索对话框
 */
class CSearch : public CAppWnd
{
	DECLARE_DYNAMIC(CSearch)

public:
	CSearch(CWnd* pParent = nullptr);
	virtual ~CSearch();

	enum { IDD = IDD_MAINSEARCH };

protected:
	/**
	 * 最近访问列表控件事件对象
	 */
	class RecentListEvent: public CListBoxEventAdapter
	{
	public:
		/**
		 * 列表项双击事件
		 */
		virtual void OnDoubleClick();

	} m_recent_list_event;

	/**
	 * 最近访问列表控件事件对象
	 */
	class RecommandListEvent: public CListBoxEventAdapter
	{
	public:
		/**
		 * 列表项双击事件
		 */
		virtual void OnDoubleClick();

	} m_recommand_list_event;

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
	
	/**
	 * 移动窗口到不可见区域，以实现隐藏窗口
	 */
	void MoveToHideWindow(BOOL bHide);

protected:
	/**
	 * 如果可视化的区域
	 */
	CRect m_rect_if_visiable;

protected:
	/**
	 * 随机推荐项目或图书列表
	 */
	vector<CString> m_recommand_values;

protected:
	/**
	 * 获取默认的焦点控件
	 */
	virtual CWnd* GetDefaultFocusWnd()
	{ return &m_code_search; }

	/**
	 * 显示标签页面
	 */
	void ShowTagTab();

	/**
	 * 显示源码页面
	 */
	void ShowCodeTab();

	/**
	 * 显示图书页面
	 */
	void ShowBookTab();

protected:
	/**
	 * 页面对话框
	 */
	CTagTab* m_tag_tab;
	CCodeTab* m_code_tab;
	CBookTab* m_book_tab;

	/**
	 * 按钮控件
	 */
	CPrettyButton m_add_tag;
	CPrettyButton m_add_book;
	CPrettyButton m_add_project;

	/**
	 * 搜索框
	 */
	CSearchEdit m_tag_search;
	CSearchEdit m_book_search;
	CSearchEdit m_code_search;

	/**
	 * 列表控件
	 */
	CStatic m_recent_group;
	CStatic m_recommand_group;
	CFolderListCtrl m_recent_list;
	CFolderListCtrl m_recommand_list;

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