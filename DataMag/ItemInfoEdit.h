#pragma once

#include "HiliteBorder.h"

/**
 * ͼ�����Ŀ��Ϣչʾ�ؼ�
 */
class CItemInfoEdit : public CRichEditCtrl
{
	DECLARE_DYNAMIC(CItemInfoEdit)

public:
	CItemInfoEdit();
	virtual ~CItemInfoEdit();

protected:
	/**
	 * ��ʼ���߿�ؼ�
	 */
	BOOL InitBorder();

protected:
	/**
	 * �߿�
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