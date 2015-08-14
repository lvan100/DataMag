#pragma once

#include "PrettyEdit.h"

#include <functional>
using namespace std;

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

	/**
	 * ������ʾ����
	 *
	 * @param strHint
	 *        ��ʾ����
	 */
	void SetHintText(CString strHint){
		m_strHint = strHint;
		InvalidateWithHint();
	}

	/**
	 * �Ƿ�ʹ��������ť
	 */
	void EnableSearchButton(BOOL enable){
		m_bEnableSearchBtn = enable;
	}

	/**
	 * ����������ť�����¼���Ӧ
	 */
	void SetClickEvent(function<void()> event) {
		m_click_event = event;
	}

	/**
	 * ����������ť˫���¼���Ӧ
	 */
	void SetDClickEvent(function<void()> event){
		m_dclick_event = event;
	}

protected:
	/**
	 * ����ͼ��
	 */
	HICON m_search_icon;

	/**
	 * ������ʾ����
	 */
	CString m_strHint;

	/**
	 * �Ƿ��ڽ���״̬
	 */
	BOOL m_bFocused;

	/**
	 * ��ǰ�Ƿ���ʾ����
	 */
	BOOL m_bTextIsHint;

	/**
	 * ��꾭��������ť����
	 */
	BOOL m_bMouseHover;

	/**
	 * �Ƿ�ʹ��������ť
	 */
	BOOL m_bEnableSearchBtn;

	/**
	 * ������ť�����¼���Ӧ
	 */
	function<void()> m_click_event;

	/**
	 * ������ť˫���¼���Ӧ
	 */
	function<void()> m_dclick_event;

protected:
	/**
	 * ˢ�� Hint �ַ���
	 */
	void InvalidateWithHint();

	/**
	 * ��ȡ������ť����
	 */
	CRect GetSearchBtnRect();

	/**
	 * �Ƿ����������ť
	 */
	BOOL HitSearchButton();

protected:
	virtual void Init();

	DECLARE_MESSAGE_MAP()
protected:
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnSetFocus(CWnd* pOldWnd);
	afx_msg void OnKillFocus(CWnd* pNewWnd);
	afx_msg HBRUSH CtlColor(CDC* pDC, UINT nCtlColor);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
	afx_msg BOOL OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message);
};