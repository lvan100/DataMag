#pragma once

class CProjectListCtrl : public CMFCShellListCtrl
{
	DECLARE_DYNAMIC(CProjectListCtrl)

public:
	CProjectListCtrl();
	virtual ~CProjectListCtrl();

	CString GetCurrentFolder(){
		CString strPath;
		CMFCShellListCtrl::GetCurrentFolder(strPath);
		return strPath;
	}

	void SetFilterString(CString str){
		strFilter = str;
		DisplayFolder(GetCurrentFolder());
	}

protected:
	CString strFilter;

protected:
	virtual void OnSetColumns();
	virtual void InitProjectList();
	virtual void PreSubclassWindow();

protected:
	DECLARE_MESSAGE_MAP()

protected:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnDblClk(NMHDR* pNMHDR, LRESULT* pResult);

protected:
	virtual HRESULT EnumObjects(LPSHELLFOLDER pParentFolder, LPITEMIDLIST pidlParent);
};


