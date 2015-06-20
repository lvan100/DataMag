#pragma once

#include "BookListCtrl.h"
#include "afxwin.h"

class CBookListDlg : public CDialog
{
	DECLARE_DYNAMIC(CBookListDlg)

public:
	CBookListDlg(CWnd* pParent = NULL);
	virtual ~CBookListDlg();

	enum { IDD = IDD_BOOKLISTDLG };

	CStringArray arrBook;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);

	DECLARE_MESSAGE_MAP()

public:
	CBookListCtrl m_book_list;

protected:
	afx_msg void OnBnClickedOk();
public:
	afx_msg void OnBnClickedBookSearchButton();
	CEdit m_book_search_edit;
	virtual BOOL OnInitDialog();
};
