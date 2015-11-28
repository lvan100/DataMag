#pragma once

#include "FolderList.h"
#include "SearchEdit.h"

/**
 * �������Ի���
 */
class CMainSearch : public CDialogEx
{
	DECLARE_DYNAMIC(CMainSearch)

public:
	CMainSearch(CWnd* pParent = NULL);
	virtual ~CMainSearch();
	
protected:
	/**
	* ��������б�ؼ��¼�����
	*/
	class RecentListEvent : public CListBoxEventAdapter
	{
	public:
		/**
		* �б���˫���¼�
		*/
		virtual void OnDoubleClick();

	} m_recent_list_event;

	/**
	* ��������б�ؼ��¼�����
	*/
	class RecommandListEvent : public CListBoxEventAdapter
	{
	public:
		/**
		* �б���˫���¼�
		*/
		virtual void OnDoubleClick();

	} m_recommand_list_event;

protected:
	/**
	* ����Ƽ�
	*/
	void DoRecommand();

protected:
	/**
	* ����Ƽ���Ŀ��ͼ���б�
	*/
	vector<CString> m_recommand_values;

protected:
	/**
	 * �����ؼ�
	 */
	CSearchEdit m_search_edit;
	CComboBox m_search_filter;

	/**
	 * ��������б�ؼ�
	 */
	CStatic m_recent_label;
	CFolderListCtrl m_recent_list;

	/**
	 * ����Ƽ��б�
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