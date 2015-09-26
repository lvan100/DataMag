#pragma once

#include <vector>
#include <algorithm>
#include <functional>
using namespace std;

#include "HilitButton.h"

/**
 * 反射自身的按钮点击事件
 */
typedef function<void(CMFCButton*)> ReflactClickEvent;

/**
 * 链接路径控件的子按钮
 */
class CPathLinkButton : public CHilitButton
{
	DECLARE_DYNAMIC(CPathLinkButton)

public:
	CPathLinkButton();
	virtual ~CPathLinkButton();

	/**
	 * 按钮点击事件
	 */
	ReflactClickEvent clickEvent;

	DECLARE_MESSAGE_MAP()
protected:
	afx_msg void OnBnDoubleclicked();
};

/**
 * 路径发生变化的事件
 */
typedef function<void(CString)> PathChangedEvent;

/**
 * 标签路径控件
 */
class CTagPathLink : public CStatic
{
	DECLARE_DYNAMIC(CTagPathLink)

public:
	CTagPathLink();
	virtual ~CTagPathLink();

	/**
	 * 设置链接路径
	 */
	void SetLinkPath(CString strPath);

	/**
	 * 子按钮事件响应
	 */
	void OnPathBtnClicked(CMFCButton* pBtn);

	/**
	 * 当双击子按钮的时候触发该事件
	 */
	PathChangedEvent mPathChangedEvent;

protected:
	/**
	 * 销毁所有子按钮
	 */
	void ClearPathLinkButtons();

protected:
	/**
	 * 路径按钮
	 */
	vector<CWnd*> m_path_btns;

protected:
	virtual BOOL OnInitControl();
	virtual void PreSubclassWindow();

	DECLARE_MESSAGE_MAP()
protected:
	afx_msg void OnDestroy();
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
};