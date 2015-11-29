#pragma once

#include "SearchEdit.h"
#include "FileListBox.h"

/**
 * 项目选择对话框
 */
class CCodeDlg : public CDialogEx, ListBoxEvent
{
	DECLARE_DYNAMIC(CCodeDlg)

public:
	CCodeDlg(CWnd* pParent = nullptr);
	virtual ~CCodeDlg();

	enum { IDD = IDD_CODEDLG };

public:
	/**
	 * 初始化列表框控件
	 */
	virtual void InitListBox();

	/**
	 * 列表项双击事件
	 */
	virtual void OnDoubleClick();

public:
	/**
	 * 被选择的项目列表
	 */
	CStringArray arrProject;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);

protected:
	CSearchEdit m_search_edit;
	CFileListBox m_project_list;

	DECLARE_MESSAGE_MAP()
protected:
	afx_msg void OnBnClickedOk();
	afx_msg void OnChangeProjectSearchEdit();
};