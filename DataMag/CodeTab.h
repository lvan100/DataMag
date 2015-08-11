#pragma once

#include "SearchEdit.h"
#include "FileListBox.h"
#include "HilitButton.h"

/**
 * ��Ŀ��ǩҳ
 */
class CCodeTab : public CDialogEx, CListBoxEventAdapter
{
	DECLARE_DYNAMIC(CCodeTab)

public:
	/**
	 * �����ʽ:"open","search:xx","add".
	 */
	CCodeTab(CString strCommand = _T(""), CWnd* pParent = NULL);
	virtual ~CCodeTab();

	enum { IDD = IDD_CODETAB };

	/**
	 * ��Ӧ��Ŀ����Ŀ¼�ı��¼�
	 */
	void OnCodeMagDirChange(CString dir){
		m_project_list.DisplayFolder(dir);
	}

public:
	/**
	 * ��ʼ���б���ؼ�
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
	CHilitButton m_project_add;
	CHilitButton m_project_delete;
	CHilitButton m_project_rename;
	CHilitButton m_project_refresh;

	CSearchEdit m_search_edit;
	CRichEditCtrl m_item_text;
	CFileListBox m_project_list;

	DECLARE_MESSAGE_MAP()
protected:
	afx_msg void OnBnClickedSetting();
	afx_msg void OnBnClickedProjectAdd();
	afx_msg void OnBnClickedProjectDelete();
	afx_msg void OnBnClickedProjectRename();
	afx_msg void OnBnClickedProjectRefresh();
	afx_msg void OnChangeProjectSearchEdit();
	afx_msg void OnDropFiles(HDROP hDropInfo);
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnActivate(UINT nState, CWnd* pWndOther, BOOL bMinimized);
};