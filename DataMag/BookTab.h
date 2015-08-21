#pragma once

#include "AppWnd.h"

#include "MoreInput.h"
#include "SearchEdit.h"
#include "FileListBox.h"
#include "HilitButton.h"
#include "TagItemEdit.h"
#include "ItemInfoEdit.h"

/**
 * ͼ���ǩҳ
 */
class CBookTab : public CAppWnd, CListBoxEventAdapter
{
	DECLARE_DYNAMIC(CBookTab)

public:
	/**
	 * �����ʽ:"open","search:xx","add".
	 */
	CBookTab(CString strCommand = _T(""), CWnd* pParent = nullptr);
	virtual ~CBookTab();

	enum { IDD = IDD_BOOKTAB };

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
	 * ִ������
	 */
	struct Command{
		CString cmd;
		CString arg;
	}m_command;

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

protected:
	/**
	 * ��ȡĬ�Ͻ���ؼ�
	 */
	virtual CWnd* GetDefaultFocusWnd()
	{ return &m_search_edit; }

protected:
	virtual BOOL OnInitDialog();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	virtual void DoDataExchange(CDataExchange* pDX);

protected:
	CHilitButton m_setting;
	CHilitButton m_book_add;
	CHilitButton m_book_delete;
	CHilitButton m_book_rename;
	CHilitButton m_modify_info;
	CHilitButton m_refresh_list;

	CFileListBox m_book_list;
	CSearchEdit m_search_edit;
	CItemInfoEdit m_item_text;

	CMoreInput m_more_input;
	CTagItemEdit m_tag_group;
	CStatic m_tag_group_title;

	DECLARE_MESSAGE_MAP()
protected:
	afx_msg void OnBnClickedBookAdd();
	afx_msg void OnBnClickedSetting();
	afx_msg void OnBnClickedBookDelete();
	afx_msg void OnBnClickedBookRename();
	afx_msg void OnBnClickedModifyInfo();
	afx_msg void OnBnClickedBookRefresh();
	afx_msg void OnChangeBookSearchEdit();
	afx_msg void OnDropFiles(HDROP hDropInfo);
};