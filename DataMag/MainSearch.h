#pragma once

#include "FolderList.h"
#include "SearchEdit.h"

/**
 * 主搜索对话框
 */
class CMainSearch : public CDialogEx
{
	DECLARE_DYNAMIC(CMainSearch)

public:
	CMainSearch(CWnd* pParent = NULL);
	virtual ~CMainSearch();
	
protected:
	/**
	* 最近访问列表控件事件对象
	*/
	class RecentListEvent : public CListBoxEventAdapter
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
	class RecommandListEvent : public CListBoxEventAdapter
	{
	public:
		/**
		* 列表项双击事件
		*/
		virtual void OnDoubleClick();

	} m_recommand_list_event;

protected:
	/**
	* 完成推荐
	*/
	void DoRecommand();

protected:
	/**
	* 随机推荐项目或图书列表
	*/
	vector<CString> m_recommand_values;

protected:
	/**
	 * 搜索控件
	 */
	CSearchEdit m_search_edit;
	CComboBox m_search_filter;

	/**
	 * 最近访问列表控件
	 */
	CStatic m_recent_label;
	CFolderListCtrl m_recent_list;

	/**
	 * 随机推荐列表
	 */
	CStatic m_recommand_label;
	CFolderListCtrl m_recommand_list;

protected:
	virtual BOOL OnInitDialog();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	virtual void DoDataExchange(CDataExchange* pDX);

protected:
	DECLARE_MESSAGE_MAP()
};