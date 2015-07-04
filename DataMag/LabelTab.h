#pragma once

#include "SearchEdit.h"
#include "ShellListCtrl.h"

/**
 * ��ǩ��ǩҳ
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
	 * ��ǩ�б��¼�����
	 */
	class LabelListEvent: public CShellListEventAdapter
	{
	public:
		/**
		 * ��ʼ���б�ؼ�
		 */
		virtual void InitShellList();

		/**
		 * �б�����仯
		 */
		virtual void OnSelectChanged();

	}m_label_event;

	/**
	 * ��ǩ��Ϣ�¼�����
	 */
	class LabelInfoEvent: public CShellListEventAdapter
	{
	public:
		/**
		 * ��ʼ���б�ؼ�
		 */
		virtual void InitShellList();

		/**
		 * �б���˫���¼�
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