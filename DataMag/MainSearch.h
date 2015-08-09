#pragma once

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
	 * ����ͼ��
	 */
	HICON m_hIcon;

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
	CStatic m_blog;

	CPrettyButton m_add_book;
	CPrettyButton m_add_label;
	CPrettyButton m_add_project;

	CSearchEdit m_book_search;
	CSearchEdit m_label_search;
	CSearchEdit m_project_search;

protected:
	virtual BOOL OnInitDialog();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	virtual void DoDataExchange(CDataExchange* pDX);

	DECLARE_MESSAGE_MAP()
protected:
	afx_msg void OnMove(int x, int y);
	afx_msg void OnBnClickedAddBook();
	afx_msg void OnBnClickedAddLabel();
	afx_msg void OnBnClickedAddProject();
	afx_msg void OnActivate(UINT nState, CWnd* pWndOther, BOOL bMinimized);
};

/**
 * ȫ�ֵ��������Ի������
 */
extern CMainSearch* theMainSearch;