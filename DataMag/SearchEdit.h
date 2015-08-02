#pragma once

#include "PrettyEdit.h"

/**
 * 搜索编辑框
 */
class CSearchEdit : public CPrettyEdit
{
	DECLARE_DYNAMIC(CSearchEdit)

public:
	CSearchEdit();
	virtual ~CSearchEdit();

	/**
	 * 设置搜索图标
	 *
	 * @param hIcon
	 *        图标
	 */
	void SetSearchIcon(HICON hIcon) {
		m_search_icon = hIcon;
	}

	/**
	 * 设置提示文字
	 *
	 * @param strHint
	 *        提示文字
	 */
	void SetHintText(CString strHint){
		m_strHint = strHint;
		InvalidateWithHint();
	}

protected:
	/**
	 * 搜索图标
	 */
	HICON m_search_icon;

	/**
	 * 设置提示文字
	 */
	CString m_strHint;

	/**
	 * 是否处于焦点状态
	 */
	BOOL m_bFocused;

	/**
	 * 当前是否提示文字
	 */
	BOOL m_bTextIsHint;

protected:
	/**
	 * 刷新 Hint 字符串
	 */
	void InvalidateWithHint();

protected:
	virtual void Init();

	DECLARE_MESSAGE_MAP()
protected:
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnSetFocus(CWnd* pOldWnd);
	afx_msg void OnKillFocus(CWnd* pNewWnd);
	afx_msg HBRUSH CtlColor(CDC* pDC, UINT nCtlColor);
};