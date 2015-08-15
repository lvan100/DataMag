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
	 * 是否高亮
	 */
	BOOL m_bHilited;

protected:
	virtual void DrawItem(LPDRAWITEMSTRUCT);

	DECLARE_MESSAGE_MAP()
protected:
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
};