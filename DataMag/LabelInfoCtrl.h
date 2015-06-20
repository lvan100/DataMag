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
	CStringA strText;

	CString strFilter;

protected:
	virtual void OnSetColumns();
	virtual void PreSubclassWindow();

protected:
	DECLARE_MESSAGE_MAP()
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnLvnItemchanged(NMHDR *pNMHDR, LRESULT *pResult);

protected:
	virtual HRESULT EnumObjects(LPSHELLFOLDER pParentFolder, LPITEMIDLIST pidlParent);
};


