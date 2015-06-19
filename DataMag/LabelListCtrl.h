#pragma once

class CLabelListCtrl : public CMFCShellListCtrl
{
	DECLARE_DYNAMIC(CLabelListCtrl)

public:
	CLabelListCtrl();
	virtual ~CLabelListCtrl();

	CString GetItemPath(int iItem){
		CString strPath;
		CMFCShellListCtrl::GetItemPath(strPath, iItem);
		return strPath;
	}

	CString GetCurrentFolder(){
		CString strPath;
		CMFCShellListCtrl::GetCurrentFolder(strPath);
		return strPath;
	}

	void SetInfoCtrl(CMFCShellListCtrl* pCtrl){
		m_pInfoCtrl = pCtrl;
	}

protected:	
	virtual void OnSetColumns();
	virtual void InitLabelList();
	virtual void PreSubclassWindow();

	CMFCShellListCtrl* m_pInfoCtrl;

protected:
	DECLARE_MESSAGE_MAP()

protected:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnDblClk(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnLvnItemchanged(NMHDR *pNMHDR, LRESULT *pResult);
};


