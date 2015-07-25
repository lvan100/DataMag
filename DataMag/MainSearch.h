#pragma once

#include "SearchEdit.h"

class CMainSearch : public CDialogEx
{
	DECLARE_DYNAMIC(CMainSearch)

public:
	CMainSearch(CWnd* pParent = NULL);
	virtual ~CMainSearch();

	enum { IDD = IDD_MAINSEARCH };

protected:
	/**
	 * ´°ÌåÍ¼±ê
	 */
	HICON m_hIcon;

protected:
	CMFCButton m_open_book;
	CMFCButton m_open_label;
	CMFCButton m_open_project;

	CSearchEdit m_book_search;
	CSearchEdit m_label_search;
	CSearchEdit m_project_search;

protected:
	virtual BOOL OnInitDialog();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	virtual void DoDataExchange(CDataExchange* pDX);

	DECLARE_MESSAGE_MAP()
protected:
	afx_msg void OnBnClickedOpenBook();
	afx_msg void OnBnClickedOpenLabel();
	afx_msg void OnBnClickedOpenProject();
};