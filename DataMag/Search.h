#pragma once

#include "AppWnd.h"
#include "TagTab.h"
#include "BookTab.h"
#include "CodeTab.h"
#include "FolderList.h"
#include "SearchEdit.h"
#include "PrettyButton.h"

/**
 * �������Ի���
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
	 * ��������б�ؼ��¼�����
	 */
	class RecentListEvent: public CListBoxEventAdapter
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
	class RecommandListEvent: public CListBoxEventAdapter
	{
	public:
		/**
		 * �б���˫���¼�
		 */
		virtual void OnDoubleClick();

	} m_recommand_list_event;

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
	
	/**
	 * �ƶ����ڵ����ɼ�������ʵ�����ش���
	 */
	void MoveToHideWindow(BOOL bHide);

protected:
	/**
	 * ������ӻ�������
	 */
	CRect m_rect_if_visiable;

protected:
	/**
	 * ����Ƽ���Ŀ��ͼ���б�
	 */
	vector<CString> m_recommand_values;

protected:
	/**
	 * ��ȡĬ�ϵĽ���ؼ�
	 */
	virtual CWnd* GetDefaultFocusWnd()
	{ return &m_code_search; }

	/**
	 * ��ʾ��ǩҳ��
	 */
	void ShowTagTab();

	/**
	 * ��ʾԴ��ҳ��
	 */
	void ShowCodeTab();

	/**
	 * ��ʾͼ��ҳ��
	 */
	void ShowBookTab();

protected:
	/**
	 * ҳ��Ի���
	 */
	CTagTab* m_tag_tab;
	CCodeTab* m_code_tab;
	CBookTab* m_book_tab;

	/**
	 * ��ť�ؼ�
	 */
	CPrettyButton m_add_tag;
	CPrettyButton m_add_book;
	CPrettyButton m_add_project;

	/**
	 * ������
	 */
	CSearchEdit m_tag_search;
	CSearchEdit m_book_search;
	CSearchEdit m_code_search;

	/**
	 * �б�ؼ�
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