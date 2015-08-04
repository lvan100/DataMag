#pragma once

class CHilitButton : public CMFCButton
{
	DECLARE_DYNAMIC(CHilitButton)

public:
	CHilitButton();
	virtual ~CHilitButton();

protected:
	virtual void PreSubclassWindow();

protected:
	virtual void OnFillBackground(CDC* pDC, const CRect& rectClient);

	DECLARE_MESSAGE_MAP()
protected:
};