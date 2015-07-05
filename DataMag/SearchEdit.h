#pragma once

#include "PrettyEdit.h"

/**
 * �����༭��
 */
class CSearchEdit : public CPrettyEdit
{
	DECLARE_DYNAMIC(CSearchEdit)

public:
	CSearchEdit();
	virtual ~CSearchEdit();

	/**
	 * ��������ͼ��
	 *
	 * @param hIcon
	 *        ͼ��
	 */
	void SetSearchIcon(HICON hIcon) {
		m_search_icon = hIcon;
	}

protected:
	/**
	 * ����ͼ��
	 */
	HICON m_search_icon;

protected:
	virtual void Init();

	DECLARE_MESSAGE_MAP()
protected:
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnSetFocus(CWnd* pOldWnd);
	afx_msg void OnKillFocus(CWnd* pNewWnd);
};