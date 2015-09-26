#pragma once

#include "SearchEdit.h"
#include "FileListBox.h"
#include "HilitButton.h"
#include "TagPathLink.h"
#include "ItemInfoEdit.h"

/**
 * 标签页面
 */
class CTagTab : public CDialogEx
{
	DECLARE_DYNAMIC(CTagTab)

public:
	CTagTab(CWnd* pParent);
	virtual ~CTagTab();

	enum { IDD = IDD_TAGTAB };

	/**
	 * 响应添加事件
	 */
	void DoCommandAdd();

	/**
	 * 响应搜索事件
	 */
	void DoCommandSearch(CString str);

public:
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

	} m_tag_event;

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

	} m_tag_info_event;

	/**
	 * 描述文件的内容
	 */
	CStringA strText;

protected:
	/**
	 * 是否允许信息可以编辑
	 */
	void EnableInfoEidt(BOOL enable);
	
	/**
	 * 销毁自身对话框
	 */
	void DestroyThisWindow();

protected:
	/**
	 * 获取默认的焦点控件
	 */
	virtual CWnd* GetDefaultFocusWnd()
	{ return &m_tag_search_edit; }
	
	/**
	 * 最后获得焦点的控件，不记录焦点在切换过程
	 * 中的变化，只用于窗口失去激活状态前的焦点
	 */
	CWnd* m_pLastFocusWnd;

protected:
	virtual BOOL OnInitDialog();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	virtual void DoDataExchange(CDataExchange* pDX);

protected:
	/**
	 * 按钮控件
	 */
	CHilitButton m_setting;
	CHilitButton m_tag_add;
	CHilitButton m_tag_delete;
	CHilitButton m_tag_rename;
	CHilitButton m_relate_book;
	CHilitButton m_remove_relate;
	CHilitButton m_relate_project;

	/**
	 * 列表控件
	 */
	CFileListBox m_tag_list;
	CFileListBox m_tag_info;
	CItemInfoEdit m_item_text;
	CSearchEdit m_tag_search_edit;
	CSearchEdit m_info_search_edit;

	/**
	 * 标签路径链接控件
	 */
	CTagPathLink m_tag_path_link;

	DECLARE_MESSAGE_MAP()
protected:
	afx_msg void OnBnClickedTagAdd();
	afx_msg void OnMove(int x, int y);
	afx_msg void OnBnClickedSetting();
	afx_msg void OnBnClickedTagDelete();
	afx_msg void OnBnClickedTagRename();
	afx_msg void OnChangeTagSearchEdit();
	afx_msg void OnChangeInfoSearchEdit();
	afx_msg void OnSetFocus(CWnd* pOldWnd);
	afx_msg void OnBnClickedTagRelateBook();
	afx_msg void OnBnClickedTagRelateProject();
	afx_msg void OnBnClickedRemoveRelationship();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnActivate(UINT nState, CWnd* pWndOther, BOOL bMinimized);
};