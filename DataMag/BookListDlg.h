#pragma once

#include "ShellListCtrl.h"

/**
 * ͼ��ѡ��Ի���
 */
class CBookListDlg : public CDialog, CShellListEventAdapter
{
	DECLARE_DYNAMIC(CBookListDlg)

public:
	CBookListDlg(CWnd* pParent = NULL);
	virtual ~CBookListDlg();

	enum { IDD = IDD_BOOKLISTDLG };

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
	 * ��ѡ���ͼ���б�
	 */
	CStringArray arrBook;

protected:
	virtual BOOL OnInitDialog();
	virtual void DoDataExchange(CDataExchange* pDX);

protected:
	CEdit m_book_search_edit;
	CShellListCtrl m_book_list;

protected:
	DECLARE_MESSAGE_MAP()
	afx_msg void OnBnClickedOk();
	afx_msg void OnChangeBookSearchEdit();
};
