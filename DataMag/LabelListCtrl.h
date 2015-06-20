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

	void SetFilterString(CString str){
		strFilter = str;
		DisplayFolder(GetCurrentFolder());
	}

protected:	
	virtual void OnSetColumns();
	virtual void InitLabelList();
	virtual void PreSubclassWindow();

	CMFCShellListCtrl* m_pInfoCtrl;

	CString strFilter;

protected:
	DECLARE_MESSAGE_MAP()

protected:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnDblClk(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnLvnItemchanged(NMHDR *pNMHDR, LRESULT *pResult);

protected:
	virtual HRESULT EnumObjects(LPSHELLFOLDER pParentFolder, LPITEMIDLIST pidlParent);
};


