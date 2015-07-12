#pragma once

#include "SearchEdit.h"
#include "FileListBox.h"
#include "ShellListCtrl.h"

/**
 * 标签标签页
 */
class CLabelTab : public CDialogEx
{
	DECLARE_DYNAMIC(CLabelTab)

public:
	CLabelTab(CWnd* pParent = NULL);
	virtual ~CLabelTab();

	enum { IDD = IDD_LABELTAB };

	/**
	 * 响应图书管理目录改变事件
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
	 * 标签信息事件对象
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

	}m_label_info_event;

protected:
	virtual BOOL OnInitDialog();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	virtual void DoDataExchange(CDataExchange* pDX);

protected:
	CFileListBox m_label_list;
	CFileListBox m_label_info;
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
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
};