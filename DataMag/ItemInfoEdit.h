#pragma once

/**
 * 图书或项目信息展示控件
 */
class CItemInfoEdit : public CRichEditCtrl
{
	DECLARE_DYNAMIC(CItemInfoEdit)

public:
	CItemInfoEdit();
	virtual ~CItemInfoEdit();

protected:
	virtual void Init();
	virtual void PreSubclassWindow();

	DECLARE_MESSAGE_MAP()
protected:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnEnLink(NMHDR *pNMHDR, LRESULT *pResult);
};