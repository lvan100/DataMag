#pragma once

#include "PrettyEdit.h"

/**
 * ËÑË÷±à¼­¿ò
 */
class CSearchEdit : public CPrettyEdit
{
	DECLARE_DYNAMIC(CSearchEdit)

public:
	CSearchEdit();
	virtual ~CSearchEdit();

	/**
	 * ÉèÖÃËÑË÷Í¼±ê
	 *
	 * @param hIcon
	 *        Í¼±ê
	 */
	void SetSearchIcon(HICON hIcon) {
		m_search_icon = hIcon;
	}

protected:
	/**
	 * ËÑË÷Í¼±ê
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