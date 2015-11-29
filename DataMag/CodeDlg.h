#pragma once

#include "SearchEdit.h"
#include "FileListBox.h"

/**
 * ��Ŀѡ��Ի���
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
	 * ��ʼ���б��ؼ�
	 */
	virtual void InitListBox();

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
	CSearchEdit m_search_edit;
	CFileListBox m_project_list;

	DECLARE_MESSAGE_MAP()
protected:
	afx_msg void OnBnClickedOk();
	afx_msg void OnChangeProjectSearchEdit();
};