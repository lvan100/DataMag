#pragma once

#include "PrettyEdit.h"
#include "ShellListCtrl.h"

/**
 * ��Ŀ��ǩҳ
 */
class CProjectTab : public CDialogEx, CShellListEventAdapter
{
	DECLARE_DYNAMIC(CProjectTab)

public:
	CProjectTab(CWnd* pParent = NULL);
	virtual ~CProjectTab();

	enum { IDD = IDD_PROJECTTAB };

	/**
	 * ��Ӧ��Ŀ����Ŀ¼�ı��¼�
	 */
	void OnCodeMagDirChange(CString dir){
		m_project_list.DisplayFolder(dir);
	}

public:
	/**
	 * ��ʼ���б�ؼ�
	 */
	virtual void InitShellList();

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

protected:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	virtual void DoDataExchange(CDataExchange* pDX);

protected:
	CPrettyEdit m_search_edit;
	CRichEditCtrl m_item_text;
	CShellListCtrl m_project_list;

	DECLARE_MESSAGE_MAP()
protected:
	afx_msg void OnChangeProjectSearchEdit();
};