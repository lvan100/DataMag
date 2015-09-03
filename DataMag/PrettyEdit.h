#pragma once

/**
 * 漂亮的文本编辑框，边框带高亮效果
 * NOTE : 需要设置多行文本样式支持.
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
	 * 是否处于焦点状态
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