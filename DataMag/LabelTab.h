#pragma once

#include "SearchEdit.h"
#include "FileListBox.h"
#include "HilitButton.h"

/**
 * ��ǩ����ҳ
 */
class CLabelTab : public CDialogEx
{
	DECLARE_DYNAMIC(CLabelTab)

public:
	/**
	 * �����ʽ:"open","search:xx","add".
	 */
	CLabelTab(CString strCommand = _T(""), CWnd* pParent = NULL);
	virtual ~CLabelTab();

	enum { IDD = IDD_LABELTAB };

	/**
	 * ��Ӧ��ǩ����Ŀ¼�ı��¼�
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
	 * �����б��¼�����
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

		/**
		 * �б�����仯
		 */
		virtual void OnSelectChanged();

	}m_label_info_event;

	/**
	 * ����ͼ��
	 */
	HICON m_hIcon;

	/**
	 * �����ļ�������
	 */
	CStringA strText;
	
	/**
	 * ִ������
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