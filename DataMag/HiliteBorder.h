#pragma once

/**
 * Ϊ�ؼ����Ӹ����߿�Ч���Ŀؼ�
 */
class CHiliteBorder : public CStatic
{
	DECLARE_DYNAMIC(CHiliteBorder)

public:
	CHiliteBorder();
	virtual ~CHiliteBorder();

	/**
	 * �Ƿ���ʾ����Ч��
	 */
	void Hilite(BOOL bHilite) {
		m_bHilited = bHilite;
		Invalidate(TRUE);
	}

protected:
	/**
	 * �Ƿ���ʾ����Ч��
	 */
	BOOL m_bHilited;

	/**
	 * ��ɫ��ˢ���Ǹ���
	 */
	CBrush m_gray_brush;

protected:
	virtual void DrawItem(LPDRAWITEMSTRUCT);

protected:
	DECLARE_MESSAGE_MAP()
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
};