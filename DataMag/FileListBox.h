#pragma once

#include "FileEnum.h"
#include "ListBoxEvent.h"
#include "HiliteBorder.h"

/**
 * �ļ��б��
 */
class CFileListBox : public CListBox, public CFileEnum
{
	DECLARE_DYNAMIC(CFileListBox)

public:
	/**
	 * �����б��ؼ��¼�ʵ�ֽӿ�
	 */
	void SetListBoxEvent(ListBoxEvent* event){
		m_event = event;
	}

	/**
	 * ִ��Ĭ�ϵ��б���˫������
	 */
	void DoDoubleClick(int iItem) {
		DoDefault(iItem);
	}

protected:
	/**
	 * �ͻ���ʵ�ֵ��¼�����
	 */
	ListBoxEvent* m_event;

public:
	CFileListBox(CShellManager* pShellManager);
	virtual ~CFileListBox();

	/**
	 * ��ȡ��һ��ѡ����б���
	 */
	int GetLastSelItem() {
		return m_nLastSelItem;
	}

	/**
	 * ��ȡ�б���ĵ�ַ
	 *
	 * @param iItem
	 *        �б��������
	 * @return �б���ĵ�ַ
	 */
	CString GetItemPath(int iItem);

	/**
	 * ���ù����ַ���
	 *
	 * @param str
	 *        �����ַ���
	 */
	void SetFilterString(CString str) {
		if (m_filter.Compare(str) != 0) {
			m_filter = str;
			Refresh();
		}
	}

protected:
	/**
	 * �����ַ���
	 */
	CString m_filter;

	/**
	 * ��һ��ѡ����б���
	 */
	int m_nLastSelItem;

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
	/**
	 * ִ��Ĭ�϶���
	 */
	virtual BOOL DoDefault(int iItem);

	/**
	 * ���Ŀ¼��ʼ���¼�
	 */
	virtual void OnDisplayFolderInit();

	/**
	 * ���Ŀ¼��ʼ�¼�
	 */
	virtual void OnDisplayFolderBefore();
	
	/**
	 * ���Ŀ¼�����¼�
	 */
	virtual void OnDisplayFolderAfter();
	
	/**
	 * ö�ٵ���Ч����
	 */
	virtual BOOL OnEnumObject(LPAFX_SHELLITEMINFO pItem);

protected:
	/**
	 * ���໯�ؼ�
	 */
	virtual void PreSubclassWindow();

	DECLARE_MESSAGE_MAP()
protected:
	afx_msg void OnDestroy();
	afx_msg void OnLbnDblclk();
	afx_msg void OnLbnSelchange();
	afx_msg void OnSetFocus(CWnd* pOldWnd);
	afx_msg void OnKillFocus(CWnd* pNewWnd);
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void DeleteItem(LPDELETEITEMSTRUCT lpDeleteItemStruct);
	afx_msg int CompareItem(LPCOMPAREITEMSTRUCT lpCompareItemStruct);
};