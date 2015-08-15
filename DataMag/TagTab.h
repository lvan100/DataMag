#pragma once

#include "AppWnd.h"

#include "SearchEdit.h"
#include "FileListBox.h"
#include "HilitButton.h"
#include "ItemInfoEdit.h"

/**
 * 标签管理页
 */
class CTagTab : public CAppWnd
{
	DECLARE_DYNAMIC(CTagTab)

public:
	/**
	 * 命令格式:"open","search:xx","add".
	 */
	CTagTab(CString strCommand = _T(""), CWnd* pParent = nullptr);
	virtual ~CTagTab();

	enum { IDD = IDD_TAGTAB };

	/**
	 * 响应标签管理目录改变事件
	 */
	void OnTagDirChange(CString dir){
		m_tag_list.DisplayFolder(dir);
	}
	
protected:
	/**
	 * 标签列表事件对象
	 */
	class TagListEvent: public CListBoxEventAdapter
	{
	public:
		/**
		 * 初始化列表框控件
		 */
		virtual void InitListBox();

		/**
		 * 列表项发生变化
		 */
		virtual void OnSelectChanged();

	}m_tag_event;

	/**
	 * 关联列表事件对象
	 */
	class TagInfoEvent: public CListBoxEventAdapter
	{
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

	}m_tag_info_event;

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
	 * 获取默认焦点控件
	 */
	virtual CWnd* GetDefaultFocusWnd()
	{ return &m_tag_search_edit; }

protected:
	virtual BOOL OnInitDialog();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	virtual void DoDataExchange(CDataExchange* pDX);

protected:
	CHilitButton m_setting;
	CHilitButton m_tag_add;
	CHilitButton m_tag_delete;
	CHilitButton m_tag_rename;
	CHilitButton m_relate_book;
	CHilitButton m_remove_relate;
	CHilitButton m_relate_project;

	CFileListBox m_tag_list;
	CFileListBox m_tag_info;
	CItemInfoEdit m_item_text;
	CSearchEdit m_tag_search_edit;
	CSearchEdit m_info_search_edit;

	DECLARE_MESSAGE_MAP()
protected:
	afx_msg void OnBnClickedSetting();
	afx_msg void OnBnClickedTagAdd();
	afx_msg void OnBnClickedTagDelete();
	afx_msg void OnBnClickedTagRename();
	afx_msg void OnChangeInfoSearchEdit();
	afx_msg void OnChangeTagSearchEdit();
	afx_msg void OnBnClickedTagRelateBook();
	afx_msg void OnBnClickedTagRelateProject();
	afx_msg void OnBnClickedRemoveRelationship();
};