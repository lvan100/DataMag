#pragma once

/**
 * ��ǩ��Ϣ�༭�ؼ�����Ҫ���з��
 */
class CTagItemEdit : public CEdit
{
	DECLARE_DYNAMIC(CTagItemEdit)

public:
	CTagItemEdit();
	virtual ~CTagItemEdit();

protected:
	virtual void Init();

protected:
	virtual void PreSubclassWindow();

	DECLARE_MESSAGE_MAP()
protected:
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnSetFocus(CWnd* pOldWnd);
	afx_msg void OnKillFocus(CWnd* pNewWnd);
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
};