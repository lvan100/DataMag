#pragma once

/**
 * 为控件增加高亮边框效果的控件
 */
class CHiliteBorder : public CStatic
{
	DECLARE_DYNAMIC(CHiliteBorder)

public:
	CHiliteBorder();
	virtual ~CHiliteBorder();

	/**
	 * 是否显示高亮效果
	 */
	void Hilite(BOOL bHilite) {
		m_bHilited = bHilite;
		Invalidate(TRUE);
	}

protected:
	/**
	 * 是否显示高亮效果
	 */
	BOOL m_bHilited;

	/**
	 * 灰色画刷，非高亮
	 */
	CBrush m_gray_brush;

protected:
	virtual void DrawItem(LPDRAWITEMSTRUCT);

protected:
	DECLARE_MESSAGE_MAP()
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
};