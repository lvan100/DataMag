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
	 * ��������б��¼�����
	 */
	class RecentListEvent: public CListBoxEventAdapter
	{
	public:
		/**
		 * �б���˫���¼�
		 */
		virtual void OnDoubleClick();

	}m_recent_list_event;

	/**
	 * ��������б��¼�����
	 */
	class RecommandListEvent: public CListBoxEventAdapter
	{
	public:
		/**
		 * �б���˫���¼�
		 */
		virtual void OnDoubleClick();

	}m_recommand_list_event;

public:
	/**
	 * ��ʼ����
	 */
	void DoSearch();

	/**
	 * ����Ƽ�
	 */
	void DoRecommand();

	/**
	 * ��Ӧ��������б�仯
	 */
	void OnRecentListChange();

public:
	/**
	 * ��ȡ����ɼ�������
	 */
	CRect GetIfVisiableRect() const
	{ return m_rect_if_visiable; }

	/**
	 * ��������ɼ�����
	 */
	void SetIfVisiableRect(CRect rc)
	{ m_rect_if_visiable = rc; }

protected:
	/**
	 * ������ӻ�������
	 */
	CRect m_rect_if_visiable;

	/**
	 * �ƶ����ڵ����ɼ������Դﵽ���ش��ڵ�Ŀ��
	 */
	void MoveToHideWindow(BOOL bHide);

protected:
	/**
	 * ����Ƽ��б�
	 */
	vector<CString> m_recommand_values;

protected:
	/**
	 * ��ȡĬ�Ͻ���ؼ�
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
 * ȫ�ֵ��������Ի������
 */
extern CMainSearch* theMainSearch;