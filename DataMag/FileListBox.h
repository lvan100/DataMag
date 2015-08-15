#pragma once

#include "ListBoxEvent.h"
#include "HiliteBorder.h"

/**
 * �ļ��б��
 */
class CFileListBox : public CListBox
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
	 * ��ǰĿ¼�Ƿ�������
	 *
	 * @return TRUE ��ʾ��ǰĿ¼�����棬����Ϊ FALSE.
	 */
	BOOL IsDesktop() const { return m_bIsDesktop; }

protected:
	/**
	 * ��ǰĿ¼�Ƿ�������
	 */
	BOOL m_bIsDesktop;

	/*
	 * ��ǰĿ¼ IDLIST
	 */
	LPITEMIDLIST  m_pidlCurFQ;

	/**
	 * ��ǰĿ¼ Shell �ӿ�
	 */
	IShellFolder* m_psfCurFolder;

	/**
	 * ������ Shell �������ӿ�
	 */
	CShellManager* m_pShellManager;

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

	/**
	 * ��ȡ��ǰĿ¼��ַ
	 *
	 * @return ��ȡ��ǰĿ¼��ַ
	 */
	CString GetCurrentFolder(){
		CString strPath;
		GetCurrentFolder(strPath);
		return strPath;
	}

	/**
	 * ��ȡ��ǰĿ¼��ַ
	 *
	 * @param strPath
	 *        Ŀ¼��ַ
	 * @return �ɹ����� TRUE��ʧ�ܷ��� FALSE.
	 */
	BOOL GetCurrentFolder(CString& strPath);

	/**
	 * ��ȡ��ǰĿ¼������
	 *
	 * @return ��ȡ��ǰĿ¼������
	 */
	CString GetCurrentFolderName(){
		CString strName;
		GetCurrentFolderName(strName);
		return strName;
	}

	/**
	 * ��ȡ��ǰĿ¼������
	 *
	 * @param strName
	 *        ��ǰĿ¼������
	 * @return �ɹ����� TRUE��ʧ�ܷ��� FALSE.
	 */
	BOOL GetCurrentFolderName(CString& strName);

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

public:
	/**
	 * ˢ�µ�ǰĿ¼
	 *
	 * @return �ɹ����� TRUE��ʧ�ܷ��� FALSE.
	 */
	virtual HRESULT Refresh();
	
	/**
	 * ��ʾ��ǰĿ¼�ĸ�Ŀ¼
	 *
	 * @return �ɹ����� TRUE��ʧ�ܷ��� FALSE.
	 */
	virtual HRESULT DisplayParentFolder();

	/**
	 * ��ʾĿ¼����
	 *
	 * @param lpszPath
	 *        Ŀ¼·��
	 * @return �ɹ����� TRUE��ʧ�ܷ��� FALSE.
	 */
	virtual HRESULT DisplayFolder(LPCTSTR lpszPath);

	/**
	 * ��ʾĿ¼����
	 *
	 * @param lpItemInfo
	 *        Ŀ¼ SHELLITEMINFO
	 * @return �ɹ����� TRUE��ʧ�ܷ��� FALSE.
	 */
	virtual HRESULT DisplayFolder(LPAFX_SHELLITEMINFO lpItemInfo);

protected:
	/**
	 * �ͷŵ�ǰĿ¼�ӿ�
	 */
	void ReleaseCurrFolder();

	/**
	 * ������ǰĿ¼�ӿ�
	 */
	HRESULT LockCurrentFolder(LPAFX_SHELLITEMINFO pItemInfo);

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
	virtual void DoDefault(int iItem);

	/**
	 * ��ȡ�б��������
	 */
	virtual CString OnGetItemText(LPAFX_SHELLITEMINFO pItem);

	/**
	 * ö�ٵ�ǰĿ¼�µ��ļ�
	 */
	virtual HRESULT EnumObjects(LPSHELLFOLDER pParentFolder, LPITEMIDLIST pidlParent);

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