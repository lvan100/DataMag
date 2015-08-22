#pragma once

#include "AppWnd.h"

#include "MoreInput.h"
#include "SearchEdit.h"
#include "FileListBox.h"
#include "TagItemEdit.h"
#include "HilitButton.h"
#include "ItemInfoEdit.h"

/**
 * ��Ŀ��ǩҳ
 */
class CCodeTab : public CAppWnd, CListBoxEventAdapter
{
	DECLARE_DYNAMIC(CCodeTab)

public:
	/**
	 * �����ʽ:"open","search:xx","add".
	 */
	CCodeTab(CString strCommand = _T(""), CWnd* pParent = nullptr);
	virtual ~CCodeTab();

	enum { IDD = IDD_CODETAB };

	/**
	 * ��Ӧ��Ŀ����Ŀ¼�ı��¼�
	 */
	void OnCodeMagDirChange(CString dir){
		m_project_list.DisplayFolder(dir);
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
	CHilitButton m_project_add;
	CHilitButton m_modify_info;
	CHilitButton m_project_delete;
	CHilitButton m_project_rename;
	CHilitButton m_project_refresh;

	CSearchEdit m_search_edit;
	CItemInfoEdit m_item_text;
	CFileListBox m_project_list;

	CMoreInput m_more_input;
	CTagItemEdit m_tag_group;
	CStatic m_tag_group_title;

	DECLARE_MESSAGE_MAP()
protected:
	afx_msg void OnDestroy();
	afx_msg void OnMove(int x, int y);
	afx_msg void OnBnClickedSetting();
	afx_msg void OnBnClickedProjectAdd();
	afx_msg void OnBnClickedModifyInfo();
	afx_msg void OnBnClickedProjectDelete();
	afx_msg void OnBnClickedProjectRename();
	afx_msg void OnBnClickedProjectRefresh();
	afx_msg void OnChangeProjectSearchEdit();
	afx_msg void OnDropFiles(HDROP hDropInfo);
};