#pragma once

/**
 * ö���ļ�������
 */
class CFolderEnum
{
public:
	CFolderEnum(CShellManager* pShellManager);
	virtual ~CFolderEnum(void);

	/**
	 * ��ǰĿ¼�Ƿ�������
	 *
	 * @return TRUE ��ʾ��ǰĿ¼�����棬����Ϊ FALSE.
	 */
	BOOL IsDesktop() const { 
		return m_bIsDesktop;
	}

	/**
	 * �Ƿ�����ö���ļ���
	 */
	void EnumFolder(BOOL enable) {
		if (enable) {
			m_tfEnumConfig |= SHCONTF_FOLDERS;
		} else {
			m_tfEnumConfig &= ~SHCONTF_FOLDERS;
		}
	}
		
	/**
	 * �Ƿ�����ö�ٷ��ļ�������
	 */
	void EnumFile(BOOL enable) {
		if (enable) {
			m_tfEnumConfig |= SHCONTF_NONFOLDERS;
		} else {
			m_tfEnumConfig &= ~SHCONTF_NONFOLDERS;
		}
	}

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
	 * ��ǰĿ¼�Ƿ�������
	 */
	BOOL m_bIsDesktop;

	/**
	 * ö�ٲ���
	 */
	SHCONTF m_tfEnumConfig;

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

protected:
	/**
	 * ������ǰĿ¼�ӿ�
	 */
	HRESULT LockCurrentFolder(LPAFX_SHELLITEMINFO pItemInfo);

	/**
	 * �ͷŵ�ǰĿ¼�ӿ�
	 */
	void ReleaseCurrFolder();

protected:
	/**
	 * ���Ŀ¼��ʼ���¼�
	 */
	virtual void OnDisplayFolderInit() {}

	/**
	 * ���Ŀ¼��ʼ�¼�
	 */
	virtual void OnDisplayFolderBefore() {}
	
	/**
	 * ���Ŀ¼�����¼�
	 */
	virtual void OnDisplayFolderAfter() {}
	
	/**
	 * ��ȡ�б��������
	 */
	virtual CString GetItemText(LPAFX_SHELLITEMINFO pItem);
	
	/**
	 * ö�ٵ���Ч����
	 */
	virtual BOOL OnEnumObject(LPAFX_SHELLITEMINFO pItem) { return FALSE; }

	/**
	 * ö�ٵ�ǰĿ¼�µ��ļ�
	 */
	virtual HRESULT EnumObjects(LPSHELLFOLDER pParentFolder, LPITEMIDLIST pidlParent);
};