#pragma once

#include "SearchEdit.h"
#include "FileListBox.h"
#include "HilitButton.h"

/**
 * 标签管理页
 */
class CLabelTab : public CDialogEx
{
	DECLARE_DYNAMIC(CLabelTab)

public:
	/**
	 * 命令格式:"open","search:xx","add".
	 */
	CLabelTab(CString strCommand = _T(""), CWnd* pParent = NULL);
	virtual ~CLabelTab();

	enum { IDD = IDD_LABELTAB };

	/**
	 * 响应标签管理目录改变事件
	 */
	void OnLabelMagDirChange(CString dir){
		m_label_list.DisplayFolder(dir);
	}
	
protected:
	/**
	 * 标签列表事件对象
	 */
	class LabelListEvent: public CListBoxEventAdapter
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

	}m_label_event;

	/**
	 * 关联列表事件对象
	 */
	class LabelInfoEvent: public CListBoxEventAdapter
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

	}m_label_info_event;

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

protected:
	virtual BOOL OnInitDialog();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	virtual void DoDataExchange(CDataExchange* pDX);

protected:
	CHilitButton m_setting;
	CHilitButton m_label_add;
	CHilitButton m_relate_book;
	CHilitButton m_label_delete;
	CHilitButton m_label_rename;
	CHilitButton m_remove_relate;
	CHilitButton m_relate_project;

	CFileListBox m_label_list;
	CFileListBox m_label_info;
	CRichEditCtrl m_item_text;
	CSearchEdit m_info_search_edit;
	CSearchEdit m_label_search_edit;

	DECLARE_MESSAGE_MAP()
protected:
	afx_msg void OnBnClickedSetting();
	afx_msg void OnBnClickedLabelAdd();
	afx_msg void OnBnClickedLabelDelete();
	afx_msg void OnBnClickedLabelRename();
	afx_msg void OnChangeInfoSearchEdit();
	afx_msg void OnChangeLabelSearchEdit();
	afx_msg void OnBnClickedLabelRelateBook();
	afx_msg void OnBnClickedLabelRelateProject();
	afx_msg void OnBnClickedRemoveRelationship();
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
};