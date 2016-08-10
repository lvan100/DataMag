#pragma once

#include <functional>
using namespace std;

/**
 * 带高亮边框效果的按钮控件
 */
class CHilitButton : public CMFCButton
{
	DECLARE_DYNAMIC(CHilitButton)

public:
	CHilitButton();
	virtual ~CHilitButton();

	void enableDoubleClick(BOOL enable) {
		m_bEnableDoubleClick = enable;
	}

	void setOnClicked(function<void()> onClicked){
		this->onClicked = onClicked;
	}

	void setOnDoubleClicked(function<void()> onDoubleClicked){
		this->onDoubleClicked = onDoubleClicked;
	}

protected:
	function<void()> onDoubleClicked;
	function<void()> onClicked;
	BOOL m_bEnableDoubleClick;
	BOOL m_bWasDoublelClick;

protected:
	virtual void PreSubclassWindow();

protected:
	virtual int GetVertMargin() const { return 3; }
	virtual void OnFillBackground(CDC* pDC, const CRect& rectClient);

	DECLARE_MESSAGE_MAP()
protected:
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
};