#pragma once

#include "SearchPad.h"
#include "DetailPage.h"
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
	
	/**
	 * ��Ӧ��������б�仯
	 */
	void OnRecentListChange();

	/**
	 * ��Ӧ��������Ŀ
	 */
	void OnShowDetailPage(CString strCatalog, CString strPath);

protected:
	/**
	* ��������б�ؼ��¼�����
	*/
	class RecentListEvent : public ListBoxEvent
	{
	public:
		/**
		 * ��ʼ���б��ؼ�
		 */
		virtual void InitListBox();

		/**
		* �б���˫���¼�
		*/
		virtual void OnDoubleClick();

	} m_recent_list_event;

	/**
	* ��������б�ؼ��¼�����
	*/
	class RecommandListEvent : public ListBoxEvent
	{
	public:
		/**
		 * ��ʼ���б��ؼ�
		 */
		virtual void InitListBox();

		/**
		* �б���˫���¼�
		*/
		virtual void OnDoubleClick();

	} m_recommand_list_event;

protected:
	/**
	* ����Ƽ�
	*/
	void DoRandomRecommand();

	/**
	* ��ȡ���������ʾ����
	*/
	CRect GetSearchPadRect();

	/**
	* ��ʾ����������
	*/
	void ShowSearchPad(bool bShow);

	/**
	 * ��ʾ��Ŀ�������
	 */
	void ShowDetailPage(bool bShow);

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
	 * ����������
	 */
	CSearchPad* m_search_pad;

	/**
	 * ��Ŀ����ҳ��
	 */
	CDetailPage* m_detail_page;

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
	afx_msg void OnEnChangeMainSearch();
	afx_msg void OnSetFocus(CWnd* pOldWnd);
	afx_msg void OnCbnSelchangeSearchCombo();
};