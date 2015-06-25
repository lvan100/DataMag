#pragma once

#include "ShellListCtrl.h"

/**
 * 主对话框
 */
class CDataMagDlg : public CDialogEx
{
public:
	CDataMagDlg(CWnd* pParent = NULL);

	enum { IDD = IDD_DATAMAG_DIALOG };

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
		 * 列表项发生变化
		 */
		virtual void OnSelectChanged();

		/**
		 * 列表项双击事件
		 */
		virtual void OnDoubleClick();

	protected:
		/**
		 * 文件内容
		 */
		CStringA strText;

	}m_label_info_event;

public:
	CEdit m_info_search_edit;
	CEdit m_label_search_edit;
	CRichEditCtrl m_item_text;
	CShellListCtrl m_label_list;
	CShellListCtrl m_label_info;

protected:
	HICON m_hIcon;

protected:
	virtual BOOL OnInitDialog();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	virtual void DoDataExchange(CDataExchange* pDX);

protected:
	DECLARE_MESSAGE_MAP()
	afx_msg void OnBnClickedSetting();
	afx_msg void OnBnClickedLabelAdd();
	afx_msg void OnBnClickedLabelDelete();
	afx_msg void OnBnClickedLabelRename();
	afx_msg void OnChangeInfoSearchEdit();
	afx_msg void OnChangeLabelSearchEdit();
	afx_msg void OnBnClickedLabelRelateBook();
	afx_msg void OnBnClickedLabelRelateProject();
};

/**
 * 全局的主对话框对象
 */
extern CDataMagDlg* theDataMagDlg;