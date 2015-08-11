#pragma once

#include "RecentList.h"
#include "SearchEdit.h"
#include "PrettyButton.h"

class CMainSearch : public CDialogEx
{
	DECLARE_DYNAMIC(CMainSearch)

public:
	CMainSearch(CWnd* pParent = NULL);
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

public:
	/**
	 * 响应最近访问列表变化
	 */
	void OnRecentListChange();

protected:
	/**
	 * 窗体图标
	 */
	HICON m_hIcon;

public:
	/**
	 * 获取如果可见的区域
	 */
	CRect GetIfVisiableRect() const
	{ return m_rect_if_visiable; }

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
	CStatic m_recent_group;
	CRecentList m_recent_list;

	CPrettyButton m_add_tag;
	CPrettyButton m_add_book;
	CPrettyButton m_add_project;

	CSearchEdit m_tag_search;
	CSearchEdit m_book_search;
	CSearchEdit m_project_search;

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
	afx_msg void OnActivate(UINT nState, CWnd* pWndOther, BOOL bMinimized);
};

/**
 * 全局的主搜索对话框对象
 */
extern CMainSearch* theMainSearch;