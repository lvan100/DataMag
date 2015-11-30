#pragma once

#include "ListBoxEvent.h"
#include "HiliteBorder.h"

/**
 * 文件夹列表控件，显示用户提供对的列表
 */
class CFolderListCtrl : public CListBox
{
	DECLARE_DYNAMIC(CFolderListCtrl)

public:
	CFolderListCtrl(CShellManager* pShellManager);
	virtual ~CFolderListCtrl();

	/**
	 * 设置列表框事件实现接口
	 */
	void SetListBoxEvent(ListBoxEvent* event){
		m_event = event;
	}

	/**
	 * 执行默认的双击操作
	 */
	void DoDoubleClick(int iItem) {
		DoDefault(iItem);
	}

	/**
	 * 获取测量之后的项的高度
	 */
	int GetMeasuredItemHeight();

public:
	/**
	 * 获取列表项的文件地址
	 *
	 * @param iItem
	 *        列表项的索引
	 * @return 列表项的地址
	 */
	CString GetItemPath(int iItem);

protected:
	/**
	 * 客户端实现的事件对象
	 */
	ListBoxEvent* m_event;

	/**
	 * 关联的 Shell 管理器接口
	 */
	CShellManager* m_pShellManager;

public:
	void SetTagImage(HICON hIcon)
	{ m_hTagImage = hIcon; }

	void SetCodeImage(HICON hIcon)
	{ m_hCodeImage = hIcon; }

	void SetBookImage(HICON hIcon)
	{ m_hBookImage = hIcon; }

protected:
	HICON m_hTagImage;
	HICON m_hCodeImage;
	HICON m_hBookImage;

protected:
	/*
	 * 背景已经被清除过
	 */
	BOOL m_isBkgndCleared;

protected:
	/**
	 * 初始化边框控件
	 */
	BOOL InitHiliteBorder();

protected:
	/**
	 * 边框控件
	 */
	CHiliteBorder* m_pHiliteBorder;

protected:
	virtual void PreSubclassWindow();
	virtual BOOL DoDefault(int iItem);
	virtual void DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct);
	virtual void MeasureItem(LPMEASUREITEMSTRUCT lpMeasureItemStruct);

	DECLARE_MESSAGE_MAP()
protected:
	afx_msg void OnDestroy();
	afx_msg void OnLbnDblclk();
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnSetFocus(CWnd* pOldWnd);
	afx_msg void OnKillFocus(CWnd* pNewWnd);
	afx_msg HBRUSH CtlColor(CDC* pDC, UINT nCtlColor);
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
};