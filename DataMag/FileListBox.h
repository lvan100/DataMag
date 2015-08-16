#pragma once

#include "FolderEnum.h"
#include "ListBoxEvent.h"
#include "HiliteBorder.h"

/**
 * �ļ��б��
 */
class CFileListBox : public CListBox, public CFolderEnum
{
	DECLARE_DYNAMIC(CFileListBox)

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

public:
	CFileListBox(CShellManager* pShellManager);
	virtual ~CFileListBox();

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

	/**
	 * ���ù����ַ���
	 *
	 * @param str
	 *        �����ַ���
	 */
	void SetFilterString(CString str){
		if (m_filter.CompareNoCase(str) != 0) {

			m_filter = str;
			m_filter.MakeLower();

			DisplayFolder(GetCurrentFolder());
		}
	}

protected:
	/**
	 * �����ַ���
	 */
	CString m_filter;

protected:
	/**
	 * ��ʼ���߿�ؼ�
	 */
	BOOL InitBorder();

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