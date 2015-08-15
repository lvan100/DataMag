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
	 * �Ƿ����
	 */
	BOOL m_bHilited;

protected:
	virtual void DrawItem(LPDRAWITEMSTRUCT);

	DECLARE_MESSAGE_MAP()
protected:
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
};