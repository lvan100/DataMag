#pragma once

class CBookListCtrl : public CMFCShellListCtrl
{
	DECLARE_DYNAMIC(CBookListCtrl)

public:
	CBookListCtrl();
	virtual ~CBookListCtrl();

protected:
	virtual void InitBookList();
	virtual void OnSetColumns();

protected:
	DECLARE_MESSAGE_MAP()

protected:
	virtual void PreSubclassWindow();
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnDblClk(NMHDR* pNMHDR, LRESULT* pResult);
};


