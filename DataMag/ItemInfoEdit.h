#pragma once

#include "HiliteBorder.h"

#include <functional>
using namespace std;

/**
 * ����༭�޸ļ�����
 */
typedef function<void()> EditChangeListener;

/**
 * ͼ�����Ŀ��Ϣչʾ�ؼ�
 */
class CItemInfoEdit : public CRichEditCtrl
{
	DECLARE_DYNAMIC(CItemInfoEdit)

public:
	CItemInfoEdit();
	virtual ~CItemInfoEdit();

	/**
	 * �������ݱ仯������
	 */
	void SetChangeListener(EditChangeListener listener) {
		m_change_listener = listener;
	}

protected:
	/**
	 * ���ݱ仯������
	 */
	EditChangeListener m_change_listener;

protected:
	/**
	 * ��ʼ���߿�ؼ�
	 */
	BOOL InitHiliteBorder();

protected:
	/**
	 * �߿�
	 */
	CHiliteBorder* m_pHiliteBorder;

protected:
	virtual void InitCtrl();
	virtual void PreSubclassWindow();

	DECLARE_MESSAGE_MAP()
protected:
	afx_msg void OnDestroy();
	afx_msg void OnEnChange();
	afx_msg void OnSetFocus(CWnd* pOldWnd);
	afx_msg void OnKillFocus(CWnd* pNewWnd);
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnEnLink(NMHDR *pNMHDR, LRESULT *pResult);
};