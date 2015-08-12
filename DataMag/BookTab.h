#pragma once

#include "AppWnd.h"

#include "SearchEdit.h"
#include "FileListBox.h"
#include "HilitButton.h"

/**
 * ͼ���ǩҳ
 */
class CBookTab : public CAppWnd, CListBoxEventAdapter
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

	/**
	 * ����ý���Ŀؼ�������¼�������л�����
	 * �еı仯��ֻ���ڴ���ʧȥ����״̬ǰ�Ľ���
	 */
	CWnd* m_pLastFocusWnd;

protected:
	/**
	 * ִ������������
	 */
	CString RenameBook(function<CString()> getSelName, function<CString()> getSelPath);

protected:
	virtual BOOL OnInitDialog();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	virtual void DoDataExchange(CDataExchange* pDX);

protected:
	CHilitButton m_setting;
	CHilitButton m_book_add;
	CHilitButton m_book_delete;
	CHilitButton m_book_rename;
	CHilitButton m_refresh_list;

	CFileListBox m_book_list;
	CSearchEdit m_search_edit;
	CRichEditCtrl m_item_text;

	DECLARE_MESSAGE_MAP()
protected:
	afx_msg void OnBnClickedBookAdd();
	afx_msg void OnBnClickedSetting();
	afx_msg void OnBnClickedBookDelete();
	afx_msg void OnBnClickedBookRename();
	afx_msg void OnBnClickedBookRefresh();
	afx_msg void OnChangeBookSearchEdit();
	afx_msg void OnDropFiles(HDROP hDropInfo);
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnActivate(UINT nState, CWnd* pWndOther, BOOL bMinimized);
};