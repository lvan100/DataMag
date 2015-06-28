#pragma once

#include "ShellListCtrl.h"

/**
 * ͼ��ѡ��Ի���
 */
class CBookSelectDlg : public CDialog, CShellListEventAdapter
{
	DECLARE_DYNAMIC(CBookSelectDlg)

public:
	CBookSelectDlg(CWnd* pParent = nullptr);
	virtual ~CBookSelectDlg();

	enum { IDD = IDD_BOOKSELECTDLG };

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
	virtual void DoDataExchange(CDataExchange* pDX);

protected:
	CEdit m_book_search_edit;
	CShellListCtrl m_book_list;

	DECLARE_MESSAGE_MAP()
protected:
	afx_msg void OnBnClickedOk();
	afx_msg void OnChangeBookSearchEdit();
};