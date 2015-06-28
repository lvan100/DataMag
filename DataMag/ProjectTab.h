#pragma once

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
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	virtual void DoDataExchange(CDataExchange* pDX);

protected:
	CEdit m_project_search_edit;
	CShellListCtrl m_project_list;

	DECLARE_MESSAGE_MAP()
protected:
	afx_msg void OnChangeProjectSearchEdit();
};