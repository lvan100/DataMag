#pragma once

/**
 * 带高亮边框效果的按钮控件
 */
class CHilitButton : public CMFCButton
{
	DECLARE_DYNAMIC(CHilitButton)

public:
	CHilitButton();
	virtual ~CHilitButton();

protected:
	virtual void PreSubclassWindow();

protected:
	virtual int GetVertMargin() const { return 3; }
	virtual void OnFillBackground(CDC* pDC, const CRect& rectClient);

	DECLARE_MESSAGE_MAP()
protected:
};