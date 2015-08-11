#pragma once

#include "SearchEdit.h"
#include "FileListBox.h"
#include "HilitButton.h"

/**
 * ��ǩ����ҳ
 */
class CTagTab : public CDialogEx
{
	DECLARE_DYNAMIC(CTagTab)

public:
	/**
	 * �����ʽ:"open","search:xx","add".
	 */
	CTagTab(CString strCommand = _T(""), CWnd* pParent = NULL);
	virtual ~CTagTab();

	enum { IDD = IDD_TAGTAB };

	/**
	 * ��Ӧ��ǩ����Ŀ¼�ı��¼�
	 */
	void OnTagDirChange(CString dir){
		m_tag_list.DisplayFolder(dir);
	}
	
protected:
	/**
	 * ��ǩ�б��¼�����
	 */
	class TagListEvent: public CListBoxEventAdapter
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

	}m_tag_event;

	/**
	 * �����б��¼�����
	 */
	class TagInfoEvent: public CListBoxEventAdapter
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

	}m_tag_info_event;

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

	/**
	 * ����ý���Ŀؼ�������¼�������л�����
	 * �еı仯��ֻ���ڴ���ʧȥ����״̬ǰ�Ľ���
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