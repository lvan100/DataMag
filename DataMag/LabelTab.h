#pragma once

#include "SearchEdit.h"
#include "FileListBox.h"
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

	/**
	 * ��Ӧͼ�����Ŀ¼�ı��¼�
	 */
	void OnLabelMagDirChange(CString dir){
		m_label_list.DisplayFolder(dir);
	}

protected:
	/**
	 * ��ǩ�б��¼�����
	 */
	class LabelListEvent: public CListBoxEventAdapter
	{
	public:
		/**
		 * ��ʼ���б��ؼ�
		 */
		virtual void InitListBox();

		/**
		 * �б�����仯
		 */
		virtual void OnSelectChanged();

	}m_label_event;

	/**
	 * ��ǩ��Ϣ�¼�����
	 */
	class LabelInfoEvent: public CListBoxEventAdapter
	{
	public:
		/**
		 * ��ʼ���б��ؼ�
		 */
		virtual void InitListBox();

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