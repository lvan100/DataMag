#pragma once

#include "ListBoxEvent.h"

/**
 * 文件夹列表控件，显示用户输入的文件夹
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
	void DoDefaultDClick(int iItem) {
		DoDefault(iItem);
	}

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

	/*
	 * 背景已经被清除过
	 */
	BOOL m_isBkgndCleared;

protected:
	virtual BOOL DoDefault(int iItem);
	virtual void DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct);
	virtual void MeasureItem(LPMEASUREITEMSTRUCT lpMeasureItemStruct);

	DECLARE_MESSAGE_MAP()
protected:
	afx_msg void OnLbnDblclk();
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnKillFocus(CWnd* pNewWnd);
	afx_msg HBRUSH CtlColor(CDC* pDC, UINT nCtlColor);
};