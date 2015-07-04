#pragma once

#include "SearchEdit.h"
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

protected:
	/**
	 * 标签列表事件对象
	 */
	class LabelListEvent: public CShellListEventAdapter
	{
	public:
		/**
		 * 初始化列表控件
		 */
		virtual void InitShellList();

		/**
		 * 列表项发生变化
		 */
		virtual void OnSelectChanged();

	}m_label_event;

	/**
	 * 标签信息事件对象
	 */
	class LabelInfoEvent: public CShellListEventAdapter
	{
	public:
		/**
		 * 初始化列表控件
		 */
		virtual void InitShellList();

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
	CShellListCtrl m_label_list;
	CShellListCtrl m_label_info;
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
};