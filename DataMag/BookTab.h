#pragma once

#include "PrettyEdit.h"
#include "ShellListCtrl.h"

/**
 * ͼ���ǩҳ
 */
class CBookTab : public CDialogEx, CShellListEventAdapter
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
	virtual void InitShellList();

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
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	virtual void DoDataExchange(CDataExchange* pDX);

protected:
	CPrettyEdit m_search_edit;
	CRichEditCtrl m_item_text;
	CShellListCtrl m_book_list;

	DECLARE_MESSAGE_MAP()
protected:
	afx_msg void OnChangeBookSearchEdit();
};
