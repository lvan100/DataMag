#pragma once

#include "ListBoxEvent.h"

class CFolderList : public CListBox
{
	DECLARE_DYNAMIC(CFolderList)

public:
	CFolderList(CShellManager* pShellManager);
	virtual ~CFolderList();

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

public:
	/**
	 * ��ȡ�б���ĵ�ַ
	 *
	 * @param iItem
	 *        �б��������
	 * @return �б���ĵ�ַ
	 */
	CString GetItemPath(int iItem){
		CString strPath;
		GetItemPath(strPath, iItem);
		return strPath;
	}

	/**
	 * ��ȡ�б���ĵ�ַ
	 *
	 * @param strPath
	 *        �б���ĵ�ַ
	 * @param iItem
	 *        �б��������
	 * @return �ɹ����� TRUE��ʧ�ܷ��� FALSE.
	 */
	BOOL GetItemPath(CString& strPath, int iItem);

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