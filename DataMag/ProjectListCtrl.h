#pragma once

class CProjectListCtrl : public CMFCShellListCtrl
{
	DECLARE_DYNAMIC(CProjectListCtrl)

public:
	CProjectListCtrl();
	virtual ~CProjectListCtrl();

protected:
	virtual void OnSetColumns();
	virtual void InitProjectList();
	virtual void PreSubclassWindow();

protected:
	DECLARE_MESSAGE_MAP()

protected:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnDblClk(NMHDR* pNMHDR, LRESULT* pResult);
};


