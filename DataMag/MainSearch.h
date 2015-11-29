#pragma once

#include "SearchPad.h"
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
	class RecentListEvent : public ListBoxEvent
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
	class RecommandListEvent : public ListBoxEvent
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

	/**
	* 获取搜索面板显示区域
	*/
	CRect GetSearchPadRect();

	/**
	 * 显示搜索结果面板
	 */
	void ShowSearchPad(bool bShow);

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
	 * 搜索结果面板
	 */
	CSearchPad* m_search_pad;

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
	afx_msg void OnEnChangeMainSearch();
};