#pragma once

/**
 * �ļ�ö����
 */
class CFileEnum
{
public:
	CFileEnum(CShellManager* pShellManager);
	virtual ~CFileEnum(void);

	/**
	 * ��ǰĿ¼�Ƿ������棨ָ���Ǹ��ڵ���Ǹ������桱��
	 *
	 * @return TRUE ��ʾ��ǰĿ¼�����棬����Ϊ FALSE .
	 */
	BOOL IsDesktop() const { 
		return m_bIsDesktop;
	}

	/**
	 * �Ƿ�����ö�����ݰ����ļ���
	 *
	 * @param enable
	 *        TRUE ��ʾ�����ļ��У�FALSE ��ʾ������.
	 */
	void EnumFolder(BOOL enable) {
		if (enable) {
			m_tfEnumConfig |= SHCONTF_FOLDERS;
		} else {
			m_tfEnumConfig &= ~SHCONTF_FOLDERS;
		}
	}
		
	/**
	 * �Ƿ�����ö�ٰ������ļ�������
	 *
	 * @param enable
	 *        TRUE ��ʾ�������ļ��У�FALSE ��ʾ������.
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
	CString GetCurrentFolder();

	/**
	 * ��ȡ��ǰĿ¼������
	 *
	 * @return ��ȡ��ǰĿ¼������
	 */
	CString GetCurrentFolderName();

public:
	/**
	 * ˢ�µ�ǰĿ¼
	 *
	 * @return �ɹ����� TRUE��ʧ�ܷ��� FALSE .
	 */
	virtual BOOL Refresh();

	/**
	 * ��ʾ��ǰĿ¼�ĸ�Ŀ¼
	 *
	 * @return �ɹ����� TRUE��ʧ�ܷ��� FALSE .
	 */
	virtual BOOL DisplayParentFolder();

	/**
	 * ��ʾĿ¼����
	 *
	 * @param lpszPath
	 *        Ŀ¼·��
	 * @return �ɹ����� TRUE��ʧ�ܷ��� FALSE .
	 */
	virtual BOOL DisplayFolder(LPCTSTR lpszPath);

	/**
	 * ��ʾĿ¼����
	 *
	 * @param lpItemInfo
	 *        Ŀ¼ SHELLITEMINFO
	 * @return �ɹ����� TRUE��ʧ�ܷ��� FALSE .
	 */
	virtual BOOL DisplayFolder(LPAFX_SHELLITEMINFO lpItemInfo);

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