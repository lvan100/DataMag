#pragma once

#include "ListBoxEvent.h"
#include "HiliteBorder.h"

/**
 * �ļ����б�ؼ�����ʾ�û��ṩ�Ե��б�
 */
class CFolderListCtrl : public CListBox
{
	DECLARE_DYNAMIC(CFolderListCtrl)

public:
	CFolderListCtrl(CShellManager* pShellManager);
	virtual ~CFolderListCtrl();

	/**
	 * �����б���¼�ʵ�ֽӿ�
	 */
	void SetListBoxEvent(ListBoxEvent* event){
		m_event = event;
	}

	/**
	 * ִ��Ĭ�ϵ�˫������
	 */
	void DoDoubleClick(int iItem) {
		DoDefault(iItem);
	}

	/**
	 * ��ȡ����֮�����ĸ߶�
	 */
	int GetMeasuredItemHeight();

public:
	/**
	 * ��ȡ�б�����ļ���ַ
	 *
	 * @param iItem
	 *        �б��������
	 * @return �б���ĵ�ַ
	 */
	CString GetItemPath(int iItem);

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
	 * �����Ѿ��������
	 */
	BOOL m_isBkgndCleared;

protected:
	/**
	 * ��ʼ���߿�ؼ�
	 */
	BOOL InitHiliteBorder();

protected:
	/**
	 * �߿�ؼ�
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