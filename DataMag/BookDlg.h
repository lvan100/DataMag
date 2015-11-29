#pragma once

#include "SearchEdit.h"
#include "FileListBox.h"

/**
 * ͼ��ѡ��Ի���
 */
class CBookDlg : public CDialogEx, ListBoxEvent
{
	DECLARE_DYNAMIC(CBookDlg)

public:
	CBookDlg(CWnd* pParent = nullptr);
	virtual ~CBookDlg();

	enum { IDD = IDD_BOOKDLG };

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
public:
	afx_msg void OnCbnSelchangeSearchCombo();
};