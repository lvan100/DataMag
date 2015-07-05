#pragma once

#include "SearchEdit.h"
#include "FileListBox.h"

/**
 * ͼ���ǩҳ
 */
class CBookTab : public CDialogEx, CListBoxEventAdapter
{
	DECLARE_DYNAMIC(CBookTab)

public:
	CBookTab(CWnd* pParent = NULL);
	virtual ~CBookTab();

	enum { IDD = IDD_BOOKTAB };

	/**
	 * ��Ӧͼ�����Ŀ¼�ı��¼�
	 */
	void OnBookMagDirChange(CString dir){
		m_book_list.DisplayFolder(dir);
	}

public:
	/**
	 * ��ʼ���б�ؼ�
	 */
	virtual void InitListBox();

	/**
	 * �б���˫���¼�
	 */
	virtual void OnDoubleClick();

	/**
	 * �б�����仯
	 */
	virtual void OnSelectChanged();

protected:
	/**
	 * �����ļ�������
	 */
	CStringA strText;

protected:
	/**
	 * ������ͼ��
	 */
	void OnRenameBook();

protected:
	virtual BOOL OnInitDialog();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	virtual void DoDataExchange(CDataExchange* pDX);

protected:
	CSearchEdit m_search_edit;
	CRichEditCtrl m_item_text;
	CFileListBox m_book_list;

	DECLARE_MESSAGE_MAP()
protected:
	afx_msg void OnChangeBookSearchEdit();
	afx_msg void OnDropFiles(HDROP hDropInfo);
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
};