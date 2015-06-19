#pragma once

class CLabelInfoCtrl : public CMFCShellListCtrl
{
	DECLARE_DYNAMIC(CLabelInfoCtrl)

public:
	CLabelInfoCtrl();
	virtual ~CLabelInfoCtrl();

	CString GetItemPath(int iItem){
		CString strPath;
		CMFCShellListCtrl::GetItemPath(strPath, iItem);
		return strPath;
	}

protected:
	virtual void OnSetColumns();
	virtual void PreSubclassWindow();

protected:
	DECLARE_MESSAGE_MAP()
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnLvnItemchanged(NMHDR *pNMHDR, LRESULT *pResult);
};


