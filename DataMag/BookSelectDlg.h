#pragma once

#include "SearchEdit.h"
#include "FileListBox.h"

/**
 * ͼ��ѡ��Ի���
 */
class CBookSelectDlg : public CDialog, CListBoxEventAdapter
{
	DECLARE_DYNAMIC(CBookSelectDlg)

public:
	CBookSelectDlg(CWnd* pParent = nullptr);
	virtual ~CBookSelectDlg();

	enum { IDD = IDD_BOOKSELECTDLG };

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
	 * ��ѡ���ͼ���б�
	 */
	CStringArray arrBook;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);

protected:
	CFileListBox m_book_list;
	CSearchEdit m_search_edit;

	DECLARE_MESSAGE_MAP()
protected:
	afx_msg void OnBnClickedOk();
	afx_msg void OnChangeBookSearchEdit();
};