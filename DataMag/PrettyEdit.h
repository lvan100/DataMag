#pragma once

/**
 * Ư�����ı��༭�򣬱߿������Ч��
 * NOTE : ��Ҫ���ö����ı���ʽ֧��.
 */
class CPrettyEdit : public CEdit
{
	DECLARE_DYNAMIC(CPrettyEdit)

public:
	CPrettyEdit();
	virtual ~CPrettyEdit();

protected:
	virtual void Init();

protected:
	/**
	 * �Ƿ��ڽ���״̬
	 */
	BOOL m_bFocused;

protected:
	virtual void PreSubclassWindow();

	DECLARE_MESSAGE_MAP()
protected:
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnSetFocus(CWnd* pOldWnd);
	afx_msg void OnKillFocus(CWnd* pNewWnd);
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
};