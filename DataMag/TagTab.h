#pragma once

#include "AppWnd.h"

#include "SearchEdit.h"
#include "FileListBox.h"
#include "HilitButton.h"
#include "ItemInfoEdit.h"

/**
 * ��ǩҳ��
 */
class CTagTab : public CAppWnd
{
	DECLARE_DYNAMIC(CTagTab)

public:
	CTagTab(CTagTab*& pointer, CWnd* pParent);
	virtual ~CTagTab();

	enum { IDD = IDD_TAGTAB };

	/**
	 * ��Ӧ����¼�
	 */
	void DoCommandAdd();

	/**
	 * ��Ӧ�����¼�
	 */
	void DoCommandSearch(CString str);

public:
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

	} m_tag_event;

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

	} m_tag_info_event;

	/**
	 * �����ļ�������
	 */
	CStringA strText;

protected:
	/**
	 * �Ƿ�������Ϣ���Ա༭
	 */
	void EnableInfoEidt(BOOL enable);
	
	/**
	 * ��������Ի���
	 */
	void DestroyThisWindow();

protected:
	/**
	 * ��ȡĬ�ϵĽ���ؼ�
	 */
	virtual CWnd* GetDefaultFocusWnd()
	{ return &m_tag_search_edit; }

protected:
	virtual BOOL OnInitDialog();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	virtual void DoDataExchange(CDataExchange* pDX);

protected:
	/**
	 * ָ�������ָ��
	 */
	CTagTab*& _self;

	/**
	 * ��ť�ؼ�
	 */
	CHilitButton m_setting;
	CHilitButton m_tag_add;
	CHilitButton m_tag_delete;
	CHilitButton m_tag_rename;
	CHilitButton m_relate_book;
	CHilitButton m_remove_relate;
	CHilitButton m_relate_project;

	/**
	 * �б�ؼ�
	 */
	CFileListBox m_tag_list;
	CFileListBox m_tag_info;
	CItemInfoEdit m_item_text;
	CSearchEdit m_tag_search_edit;
	CSearchEdit m_info_search_edit;

	DECLARE_MESSAGE_MAP()
protected:
	afx_msg void OnBnClickedTagAdd();
	afx_msg void OnMove(int x, int y);
	afx_msg void OnBnClickedSetting();
	afx_msg void OnBnClickedTagDelete();
	afx_msg void OnBnClickedTagRename();
	afx_msg void OnChangeTagSearchEdit();
	afx_msg void OnChangeInfoSearchEdit();
	afx_msg void OnBnClickedTagRelateBook();
	afx_msg void OnBnClickedTagRelateProject();
	afx_msg void OnBnClickedRemoveRelationship();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
};