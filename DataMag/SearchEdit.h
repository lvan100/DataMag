#pragma once

#include "PrettyEdit.h"

#include <functional>
using namespace std;

/**
 * 定义按钮点击事件类型
 */
typedef function<void()> ClickEvent;

/**
 * 搜索编辑框，可以设置搜索按钮
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

	/**
	 * 是否使用搜索按钮
	 */
	void EnableSearchButton(BOOL enable){
		m_bEnableSearchBtn = enable;
	}

	/**
	 * 设置搜索按钮单击事件响应
	 */
	void SetClickEvent(ClickEvent event) {
		m_click_event = event;
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
	 * 当前是否提示文字
	 */
	BOOL m_bTextIsHint;

	/**
	 * 鼠标经过搜索按钮区域
	 */
	BOOL m_bMouseHover;

	/**
	 * 是否使用搜索按钮
	 */
	BOOL m_bEnableSearchBtn;

	/**
	 * 搜索按钮单击事件响应
	 */
	ClickEvent m_click_event;

protected:
	/**
	 * 重新设置输入框区域
	 */
	void PositionNPRect();

	/**
	 * 刷新 Hint 字符串
	 */
	void InvalidateWithHint();

	/**
	 * 获取搜索按钮区域
	 */
	CRect GetSearchBtnRect();

	/**
	 * 是否击中搜索按钮
	 */
	BOOL HitSearchButton();

protected:
	virtual void Init();

	DECLARE_MESSAGE_MAP()
protected:
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnSetFocus(CWnd* pOldWnd);
	afx_msg void OnKillFocus(CWnd* pNewWnd);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg HBRUSH CtlColor(CDC* pDC, UINT nCtlColor);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg BOOL OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message);
};