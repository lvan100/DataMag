#pragma once

class CBookListCtrl : public CMFCShellListCtrl
{
	DECLARE_DYNAMIC(CBookListCtrl)

public:
	CBookListCtrl();
	virtual ~CBookListCtrl();

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
	virtual void InitBookList();
	virtual void OnSetColumns();

protected:
	DECLARE_MESSAGE_MAP()

protected:
	virtual void PreSubclassWindow();
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnDblClk(NMHDR* pNMHDR, LRESULT* pResult);

protected:
	virtual HRESULT EnumObjects(LPSHELLFOLDER pParentFolder, LPITEMIDLIST pidlParent);
};


