#pragma once

#include "ShellListCtrl.h"

/**
 * ���Ի���
 */
class CDataMagDlg : public CDialogEx
{
public:
	CDataMagDlg(CWnd* pParent = NULL);

	enum { IDD = IDD_DATAMAG_DIALOG };

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
		 * �б�����仯
		 */
		virtual void OnSelectChanged();

		/**
		 * �б���˫���¼�
		 */
		virtual void OnDoubleClick();

	protected:
		/**
		 * �ļ�����
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
 * ȫ�ֵ����Ի������
 */
extern CDataMagDlg* theDataMagDlg;