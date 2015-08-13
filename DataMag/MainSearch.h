#pragma once

#include "AppWnd.h"

#include "RecentList.h"
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

public:
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
	 * ��ȡĬ�Ͻ���ؼ�
	 */
	virtual CWnd* GetDefaultFocusWnd()
	{ return &m_project_search; }

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
};

/**
 * ȫ�ֵ��������Ի������
 */
extern CMainSearch* theMainSearch;