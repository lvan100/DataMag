#pragma once

#include "HiliteBorder.h"

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
	/**
	 * 初始化边框控件
	 */
	BOOL InitBorder();

protected:
	/**
	 * 边框
	 */
	CHiliteBorder* m_pHiliteBorder;

protected:
	virtual void InitCtrl();
	virtual void PreSubclassWindow();

	DECLARE_MESSAGE_MAP()
protected:
	afx_msg void OnDestroy();
	afx_msg void OnSetFocus(CWnd* pOldWnd);
	afx_msg void OnKillFocus(CWnd* pNewWnd);
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnEnLink(NMHDR *pNMHDR, LRESULT *pResult);
};