#pragma once

#include "ProjectListCtrl.h"
#include "afxwin.h"

class CProjectListDlg : public CDialog
{
	DECLARE_DYNAMIC(CProjectListDlg)

public:
	CProjectListDlg(CWnd* pParent = NULL);
	virtual ~CProjectListDlg();

	enum { IDD = IDD_PROJECTLISTDLG };

	CStringArray arrProject;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);

	DECLARE_MESSAGE_MAP()

public:
	CProjectListCtrl m_project_list;

protected:
	afx_msg void OnBnClickedOk();
public:
	afx_msg void OnBnClickedProjectSearchButton();
	CEdit m_project_search_edit;
	virtual BOOL OnInitDialog();
};