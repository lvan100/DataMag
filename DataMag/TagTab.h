#pragma once

#include "SearchEdit.h"
#include "FileListBox.h"
#include "HilitButton.h"

/**
 * 标签管理页
 */
class CTagTab : public CDialogEx
{
	DECLARE_DYNAMIC(CTagTab)

public:
	/**
	 * 命令格式:"open","search:xx","add".
	 */
	CTagTab(CString strCommand = _T(""), CWnd* pParent = NULL);
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
	 * 窗体图标
	 */
	HICON m_hIcon;

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
	CHilitButton m_setting;
	CHilitButton m_tag_add;
	CHilitButton m_relate_book;
	CHilitButton m_tag_delete;
	CHilitButton m_tag_rename;
	CHilitButton m_remove_relate;
	CHilitButton m_relate_project;

	CFileListBox m_tag_list;
	CFileListBox m_tag_info;
	CRichEditCtrl m_item_text;
	CSearchEdit m_info_search_edit;
	CSearchEdit m_tag_search_edit;

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
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnActivate(UINT nState, CWnd* pWndOther, BOOL bMinimized);
};