#pragma once

#include "MoreInput.h"
#include "PrettyEdit.h"
#include "SearchEdit.h"
#include "FileListBox.h"
#include "HilitButton.h"
#include "ItemInfoEdit.h"

/**
 * ͼ��ҳ��
 */
class CBookTab : public CDialogEx, CListBoxEventAdapter
{
	DECLARE_DYNAMIC(CBookTab)

public:
	CBookTab(CWnd* pParent);
	virtual ~CBookTab();

	enum { IDD = IDD_BOOKTAB };

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
	 * ��Ӧͼ�����Ŀ¼�ı��¼�
	 */
	void OnBookMagDirChange(CString dir){
		m_book_list.DisplayFolder(dir);
	}

	/**
	 * ��Ӧ���������仯�¼�
	 */
	void OnItemEditChange();

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

protected:
	/**
	 * �����ļ�������
	 */
	CStringA strText;

	/**
	 * �༭�ͽ�ֹ�༭ͼ��
	 */
	HICON m_hCanEditIcon;
	HICON m_hNotEditIcon;

protected:
	/**
	 * ִ������������
	 */
	CString RenameBook(function<CString()> getSelName, function<CString()> getSelPath);

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
	{ return &m_search_edit; }
	
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
	/**
	 * ��ť�ؼ�
	 */
	CHilitButton m_setting;
	CHilitButton m_book_add;
	CHilitButton m_book_delete;
	CHilitButton m_book_rename;
	CHilitButton m_modify_info;
	CHilitButton m_refresh_list;

	/**
	 * ͼ���б�ؼ�
	 */
	CFileListBox m_book_list;
	CSearchEdit m_search_edit;
	CItemInfoEdit m_item_text;

	/**
	 * ͼ������ؼ�
	 */
	CMoreInput m_more_input;
	CPrettyEdit m_tag_group;
	CStatic m_tag_group_title;
	
	DECLARE_MESSAGE_MAP()
protected:
	afx_msg void OnDestroy();
	afx_msg void OnMove(int x, int y);
	afx_msg void OnBnClickedBookAdd();
	afx_msg void OnBnClickedSetting();
	afx_msg void OnBnClickedBookDelete();
	afx_msg void OnBnClickedBookRename();
	afx_msg void OnBnClickedModifyInfo();
	afx_msg void OnBnClickedBookRefresh();
	afx_msg void OnChangeBookSearchEdit();
	afx_msg void OnSetFocus(CWnd* pOldWnd);
	afx_msg void OnDropFiles(HDROP hDropInfo);
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnActivate(UINT nState, CWnd* pWndOther, BOOL bMinimized);
};