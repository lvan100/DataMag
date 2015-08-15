#pragma once

#include "ListBoxEvent.h"
#include "HiliteBorder.h"

/**
 * 文件列表框
 */
class CFileListBox : public CListBox
{
	DECLARE_DYNAMIC(CFileListBox)

public:
	/**
	 * 设置列表框事件实现接口
	 */
	void SetListEvent(ListBoxEvent* event){
		m_event = event;
	}

	/**
	 * 执行默认的双击操作
	 */
	void DoDefaultDClick(int iItem) {
		DoDefault(iItem);
	}

protected:
	/**
	 * 客户端实现的事件对象
	 */
	ListBoxEvent* m_event;

public:
	CFileListBox(CShellManager* pShellManager);
	virtual ~CFileListBox();
	
	/**
	 * 当前目录是否是桌面
	 *
	 * @return TRUE 表示当前目录是桌面，否则为 FALSE.
	 */
	BOOL IsDesktop() const { return m_bIsDesktop; }

protected:
	/**
	 * 当前目录是否是桌面
	 */
	BOOL m_bIsDesktop;

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

public:
	/**
	 * 获取列表项的地址
	 *
	 * @param iItem
	 *        列表项的索引
	 * @return 列表项的地址
	 */
	CString GetItemPath(int iItem){
		CString strPath;
		GetItemPath(strPath, iItem);
		return strPath;
	}

	/**
	 * 获取列表项的地址
	 *
	 * @param strPath
	 *        列表项的地址
	 * @param iItem
	 *        列表项的索引
	 * @return 成功返回 TRUE，失败返回 FALSE.
	 */
	BOOL GetItemPath(CString& strPath, int iItem);

	/**
	 * 获取当前目录地址
	 *
	 * @return 获取当前目录地址
	 */
	CString GetCurrentFolder(){
		CString strPath;
		GetCurrentFolder(strPath);
		return strPath;
	}

	/**
	 * 获取当前目录地址
	 *
	 * @param strPath
	 *        目录地址
	 * @return 成功返回 TRUE，失败返回 FALSE.
	 */
	BOOL GetCurrentFolder(CString& strPath);

	/**
	 * 获取当前目录的名称
	 *
	 * @return 获取当前目录的名称
	 */
	CString GetCurrentFolderName(){
		CString strName;
		GetCurrentFolderName(strName);
		return strName;
	}

	/**
	 * 获取当前目录的名称
	 *
	 * @param strName
	 *        当前目录的名称
	 * @return 成功返回 TRUE，失败返回 FALSE.
	 */
	BOOL GetCurrentFolderName(CString& strName);

	/**
	 * 设置过滤字符串
	 *
	 * @param str
	 *        过滤字符串
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
	 * 过滤字符串
	 */
	CString m_filter;

public:
	/**
	 * 刷新当前目录
	 *
	 * @return 成功返回 TRUE，失败返回 FALSE.
	 */
	virtual HRESULT Refresh();
	
	/**
	 * 显示当前目录的父目录
	 *
	 * @return 成功返回 TRUE，失败返回 FALSE.
	 */
	virtual HRESULT DisplayParentFolder();

	/**
	 * 显示目录内容
	 *
	 * @param lpszPath
	 *        目录路径
	 * @return 成功返回 TRUE，失败返回 FALSE.
	 */
	virtual HRESULT DisplayFolder(LPCTSTR lpszPath);

	/**
	 * 显示目录内容
	 *
	 * @param lpItemInfo
	 *        目录 SHELLITEMINFO
	 * @return 成功返回 TRUE，失败返回 FALSE.
	 */
	virtual HRESULT DisplayFolder(LPAFX_SHELLITEMINFO lpItemInfo);

protected:
	/**
	 * 释放当前目录接口
	 */
	void ReleaseCurrFolder();

	/**
	 * 锁定当前目录接口
	 */
	HRESULT LockCurrentFolder(LPAFX_SHELLITEMINFO pItemInfo);

protected:
	/**
	 * 初始化边框控件
	 */
	BOOL InitBorder();

protected:
	/**
	 * 边框
	 */
	CHiliteBorder* m_pHiliteBorder;

protected:
	/**
	 * 执行默认动作
	 */
	virtual void DoDefault(int iItem);

	/**
	 * 获取列表项的文字
	 */
	virtual CString OnGetItemText(LPAFX_SHELLITEMINFO pItem);

	/**
	 * 枚举当前目录下的文件
	 */
	virtual HRESULT EnumObjects(LPSHELLFOLDER pParentFolder, LPITEMIDLIST pidlParent);

protected:
	/**
	 * 子类化控件
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