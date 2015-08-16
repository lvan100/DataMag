#pragma once

#include "FolderEnum.h"
#include "ListBoxEvent.h"
#include "HiliteBorder.h"

/**
 * 文件列表框
 */
class CFileListBox : public CListBox, public CFolderEnum
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
	virtual BOOL DoDefault(int iItem);

	/**
	 * 浏览目录初始化事件
	 */
	virtual void OnDisplayFolderInit();

	/**
	 * 浏览目录开始事件
	 */
	virtual void OnDisplayFolderBefore();
	
	/**
	 * 浏览目录结束事件
	 */
	virtual void OnDisplayFolderAfter();
	
	/**
	 * 枚举到有效内容
	 */
	virtual BOOL OnEnumObject(LPAFX_SHELLITEMINFO pItem);

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