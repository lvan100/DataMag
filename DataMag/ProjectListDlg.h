#pragma once

#include "ShellListCtrl.h"

/**
 * ��Ŀѡ��Ի���
 */
class CProjectListDlg : public CDialog, CShellListEventAdapter
{
	DECLARE_DYNAMIC(CProjectListDlg)

public:
	CProjectListDlg(CWnd* pParent = NULL);
	virtual ~CProjectListDlg();

	enum { IDD = IDD_PROJECTLISTDLG };
	
	enum{
		Display,
		Releate,
	}Op;

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
	virtual BOOL OnInitDialog();
	virtual void DoDataExchange(CDataExchange* pDX);

protected:
	CEdit m_project_search_edit;
	CShellListCtrl m_project_list;

protected:
	DECLARE_MESSAGE_MAP()
	afx_msg void OnBnClickedOk();
	afx_msg void OnChangeProjectSearchEdit();
};