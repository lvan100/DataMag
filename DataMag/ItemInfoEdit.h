#pragma once

#include "HiliteBorder.h"

#include <functional>
using namespace std;

/**
 * 定义编辑修改监听器
 */
typedef function<void()> EditChangeListener;

/**
 * 图书或项目信息展示控件
 */
class CItemInfoEdit : public CRichEditCtrl
{
	DECLARE_DYNAMIC(CItemInfoEdit)

public:
	CItemInfoEdit();
	virtual ~CItemInfoEdit();

	/**
	 * 设置内容变化监听器
	 */
	void SetChangeListener(EditChangeListener listener) {
		m_change_listener = listener;
	}

protected:
	/**
	 * 内容变化监听器
	 */
	EditChangeListener m_change_listener;

protected:
	/**
	 * 初始化边框控件
	 */
	BOOL InitHiliteBorder();

protected:
	/**
	 * 边框
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