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
	/**
	 * �����ʽ:"open","search:xx","add".
	 */
	CBookTab(CString strCommand = _T(""), CWnd* pParent = NULL);
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
	 * ��ʼ���б��ؼ�
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
	 * ����ͼ��
	 */
	HICON m_hIcon;

	/**
	 * �����ļ�������
	 */
	CStringA strText;

	/**
	 * ִ������
	 */
	struct Command{
		CString cmd;
		CString arg;
	}m_command;

protected:
	virtual BOOL OnInitDialog();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	virtual void DoDataExchange(CDataExchange* pDX);

protected:
	CFileListBox m_book_list;
	CSearchEdit m_search_edit;
	CRichEditCtrl m_item_text;

	DECLARE_MESSAGE_MAP()
protected:
	afx_msg void OnBnClickedBookAdd();
	afx_msg void OnBnClickedBookDelete();
	afx_msg void OnBnClickedBookRename();
	afx_msg void OnBnClickedBookRefresh();
	afx_msg void OnChangeBookSearchEdit();
	afx_msg void OnDropFiles(HDROP hDropInfo);
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
};