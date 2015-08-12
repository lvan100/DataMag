#pragma once

#include "PrettyEdit.h"

/**
 * �����Ի���
 */
class CNameDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CNameDlg)

public:
	CNameDlg(CWnd* pParent = nullptr);
	virtual ~CNameDlg();

	enum { IDD = IDD_NAMEDLG };

public:
	/**
	 * �Ի������
	 */
	CString m_title;

	/**
	 * ��ǩ����
	 */
	CString m_name;

protected:
	CPrettyEdit m_name_edit;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);

	DECLARE_MESSAGE_MAP()
protected:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
};