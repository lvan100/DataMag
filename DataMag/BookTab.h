#pragma once

#include "AppWnd.h"

#include "SearchEdit.h"
#include "FileListBox.h"
#include "HilitButton.h"
#include "ItemInfoEdit.h"

/**
 * 图书标签页
 */
class CBookTab : public CAppWnd, CListBoxEventAdapter
{
	DECLARE_DYNAMIC(CBookTab)

public:
	/**
	 * 命令格式:"open","search:xx","add".
	 */
	CBookTab(CString strCommand = _T(""), CWnd* pParent = nullptr);
	virtual ~CBookTab();

	enum { IDD = IDD_BOOKTAB };

	/**
	 * 响应图书管理目录改变事件
	 */
	void OnBookMagDirChange(CString dir){
		m_book_list.DisplayFolder(dir);
	}

public:
	/**
	 * 初始化列表框控件
	 */
	virtual void InitListBox();

	/**
	 * 列表项双击事件
	 */
	virtual void OnDoubleClick();

	/**
	 * 列表项发生变化
	 */
	virtual void OnSelectChanged();

protected:
	/**
	 * 描述文件的内容
	 */
	CStringA strText;

	/**
	 * 执行命令
	 */
	struct Command{
		CString cmd;
		CString arg;
	}m_command;

protected:
	/**
	 * 执行重命名操作
	 */
	CString RenameBook(function<CString()> getSelName, function<CString()> getSelPath);

protected:
	/**
	 * 获取默认焦点控件
	 */
	virtual CWnd* GetDefaultFocusWnd()
	{ return &m_search_edit; }

protected:
	virtual BOOL OnInitDialog();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	virtual void DoDataExchange(CDataExchange* pDX);

protected:
	CHilitButton m_setting;
	CHilitButton m_book_add;
	CHilitButton m_book_delete;
	CHilitButton m_book_rename;
	CHilitButton m_refresh_list;

	CFileListBox m_book_list;
	CSearchEdit m_search_edit;
	CItemInfoEdit m_item_text;

	DECLARE_MESSAGE_MAP()
protected:
	afx_msg void OnBnClickedBookAdd();
	afx_msg void OnBnClickedSetting();
	afx_msg void OnBnClickedBookDelete();
	afx_msg void OnBnClickedBookRename();
	afx_msg void OnBnClickedBookRefresh();
	afx_msg void OnChangeBookSearchEdit();
	afx_msg void OnDropFiles(HDROP hDropInfo);
};