#pragma once

#include "SearchEdit.h"
#include "FileListBox.h"

/**
 * 标签管理页
 */
class CLabelTab : public CDialogEx
{
	DECLARE_DYNAMIC(CLabelTab)

public:
	CLabelTab(CString strFilter = _T(""), CWnd* pParent = NULL);
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
	 * 描述文件的内容
	 */
	CStringA strText;
	
	/**
	 * 搜索过滤器
	 */
	CString m_strFilter;

protected:
	virtual BOOL OnInitDialog();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	virtual void DoDataExchange(CDataExchange* pDX);

protected:
	CFileListBox m_label_list;
	CFileListBox m_label_info;
	CRichEditCtrl m_item_text;
	CSearchEdit m_info_search_edit;
	CSearchEdit m_label_search_edit;

	DECLARE_MESSAGE_MAP()
protected:
	afx_msg void OnBnClickedLabelAdd();
	afx_msg void OnBnClickedLabelDelete();
	afx_msg void OnBnClickedLabelRename();
	afx_msg void OnChangeInfoSearchEdit();
	afx_msg void OnChangeLabelSearchEdit();
	afx_msg void OnBnClickedLabelRelateBook();
	afx_msg void OnBnClickedLabelRelateProject();
	afx_msg void OnBnClickedRemoveRelationship();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
};