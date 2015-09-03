#pragma once

/**
 * 文件枚举器
 */
class CFileEnum
{
public:
	CFileEnum(CShellManager* pShellManager);
	virtual ~CFileEnum(void);

	/**
	 * 当前目录是否是桌面（指的是根节点的那个“桌面”）
	 *
	 * @return TRUE 表示当前目录是桌面，否则为 FALSE .
	 */
	BOOL IsDesktop() const { 
		return m_bIsDesktop;
	}

	/**
	 * 是否允许枚举内容包含文件夹
	 *
	 * @param enable
	 *        TRUE 表示包含文件夹，FALSE 表示不包含.
	 */
	void EnumFolder(BOOL enable) {
		if (enable) {
			m_tfEnumConfig |= SHCONTF_FOLDERS;
		} else {
			m_tfEnumConfig &= ~SHCONTF_FOLDERS;
		}
	}
		
	/**
	 * 是否允许枚举包含非文件夹内容
	 *
	 * @param enable
	 *        TRUE 表示包含非文件夹，FALSE 表示不包含.
	 */
	void EnumFile(BOOL enable) {
		if (enable) {
			m_tfEnumConfig |= SHCONTF_NONFOLDERS;
		} else {
			m_tfEnumConfig &= ~SHCONTF_NONFOLDERS;
		}
	}

	/**
	 * 获取当前目录地址
	 *
	 * @return 获取当前目录地址
	 */
	CString GetCurrentFolder();

	/**
	 * 获取当前目录的名称
	 *
	 * @return 获取当前目录的名称
	 */
	CString GetCurrentFolderName();

public:
	/**
	 * 刷新当前目录
	 *
	 * @return 成功返回 TRUE，失败返回 FALSE .
	 */
	virtual BOOL Refresh();

	/**
	 * 显示当前目录的父目录
	 *
	 * @return 成功返回 TRUE，失败返回 FALSE .
	 */
	virtual BOOL DisplayParentFolder();

	/**
	 * 显示目录内容
	 *
	 * @param lpszPath
	 *        目录路径
	 * @return 成功返回 TRUE，失败返回 FALSE .
	 */
	virtual BOOL DisplayFolder(LPCTSTR lpszPath);

	/**
	 * 显示目录内容
	 *
	 * @param lpItemInfo
	 *        目录 SHELLITEMINFO
	 * @return 成功返回 TRUE，失败返回 FALSE .
	 */
	virtual BOOL DisplayFolder(LPAFX_SHELLITEMINFO lpItemInfo);

protected:
	/**
	 * 当前目录是否是桌面
	 */
	BOOL m_bIsDesktop;

	/**
	 * 枚举参数
	 */
	SHCONTF m_tfEnumConfig;

	/*
	 * 当前目录 IDLIST
	 */
	LPITEMIDLIST  m_pidlCurFQ;

	/**
	 * 当前目录 Shell 接口
	 */
	IShellFolder* m_psfCurFolder;

	/**
	 * 关联的 Shell 管理器接口
	 */
	CShellManager* m_pShellManager;

protected:
	/**
	 * 锁定当前目录接口
	 */
	HRESULT LockCurrentFolder(LPAFX_SHELLITEMINFO pItemInfo);

	/**
	 * 释放当前目录接口
	 */
	void ReleaseCurrFolder();

protected:
	/**
	 * 浏览目录初始化事件
	 */
	virtual void OnDisplayFolderInit() {}

	/**
	 * 浏览目录开始事件
	 */
	virtual void OnDisplayFolderBefore() {}
	
	/**
	 * 浏览目录结束事件
	 */
	virtual void OnDisplayFolderAfter() {}
	
	/**
	 * 获取列表项的文字
	 */
	virtual CString GetItemText(LPAFX_SHELLITEMINFO pItem);
	
	/**
	 * 枚举到有效内容
	 */
	virtual BOOL OnEnumObject(LPAFX_SHELLITEMINFO pItem) { return FALSE; }

	/**
	 * 枚举当前目录下的文件
	 */
	virtual HRESULT EnumObjects(LPSHELLFOLDER pParentFolder, LPITEMIDLIST pidlParent);
};