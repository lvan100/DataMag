#pragma once

#include "ShellListCtrl.h"

/**
 * ��Ŀѡ��Ի���
 */
class CProjectSelectDlg : public CDialog, CShellListEventAdapter
{
	DECLARE_DYNAMIC(CProjectSelectDlg)

public:
	CProjectSelectDlg(CWnd* pParent = nullptr);
	virtual ~CProjectSelectDlg();

	enum { IDD = IDD_PROJECTSELECTDLG };

public:
	/**
	 * ��ʼ���б�ؼ�
	 */
	virtual void InitShellList();

	/**
	 * �б���˫���¼�
	 */
	virtual void OnDoubleClick();

public:
	/**
	 * ��ѡ�����Ŀ�б�
	 */
	CStringArray arrProject;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);

protected:
	CEdit m_project_search_edit;
	CShellListCtrl m_project_list;

	DECLARE_MESSAGE_MAP()
protected:
	afx_msg void OnBnClickedOk();
	afx_msg void OnChangeProjectSearchEdit();
};