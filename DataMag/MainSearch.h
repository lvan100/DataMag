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
	 * 窗体图标
	 */
	HICON m_hIcon;

protected:
	/**
	 * 移动窗口到不可见区域，以达到隐藏窗口的目的
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
	afx_msg void OnBnClickedAddBook();
	afx_msg void OnBnClickedAddLabel();
	afx_msg void OnBnClickedAddProject();
	afx_msg void OnActivate(UINT nState, CWnd* pWndOther, BOOL bMinimized);
};