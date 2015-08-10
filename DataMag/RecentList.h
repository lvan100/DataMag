#pragma once

#include "ListBoxEvent.h"

class CRecentList : public CListBox
{
	DECLARE_DYNAMIC(CRecentList)

public:
	CRecentList(CShellManager* pShellManager);
	virtual ~CRecentList();

public:
	/**
	 * �����б���¼�ʵ�ֽӿ�
	 */
	void SetListEvent(ListBoxEvent* event){
		m_event = event;
	}

	/**
	 * ִ��Ĭ�ϵ�˫������
	 */
	void DoDefaultDClick(int iItem) {
		DoDefault(iItem);
	}

protected:
	/**
	 * �ͻ���ʵ�ֵ��¼�����
	 */
	ListBoxEvent* m_event;

	/**
	 * ������ Shell �������ӿ�
	 */
	CShellManager* m_pShellManager;

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
	/**
	 * ִ��Ĭ�϶���
	 */
	virtual void DoDefault(int iItem);

protected:
	virtual void DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct);
	virtual void MeasureItem(LPMEASUREITEMSTRUCT lpMeasureItemStruct);

	DECLARE_MESSAGE_MAP()
protected:
	afx_msg void OnLbnDblclk();
	afx_msg HBRUSH CtlColor(CDC* pDC, UINT nCtlColor);
};