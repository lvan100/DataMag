#pragma once

#include "ListBoxEvent.h"

class CRecentList : public CListBox
{
	DECLARE_DYNAMIC(CRecentList)

public:
	CRecentList();
	virtual ~CRecentList();

public:
	/**
	 * 设置列表框事件实现接口
	 */
	void SetListEvent(ListBoxEvent* event){
		m_event = event;
	}

protected:
	/**
	 * 客户端实现的事件对象
	 */
	ListBoxEvent* m_event;

public:
	void SetLabelImage(HICON hIcon)
	{ m_hLabelImage = hIcon; }

	void SetCodeImage(HICON hIcon)
	{ m_hCodeImage = hIcon; }

	void SetBookImage(HICON hIcon)
	{ m_hBookImage = hIcon; }

protected:
	HICON m_hCodeImage;
	HICON m_hBookImage;
	HICON m_hLabelImage;

protected:
	virtual void DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct);
	virtual void MeasureItem(LPMEASUREITEMSTRUCT lpMeasureItemStruct);

	DECLARE_MESSAGE_MAP()
protected:
	afx_msg void OnLbnDblclk();
	afx_msg HBRUSH CtlColor(CDC* pDC, UINT nCtlColor);
};