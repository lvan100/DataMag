#pragma once

#include "PrettyEdit.h"

/**
 * 命名对话框
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
	 * 对话框标题
	 */
	CString m_title;

	/**
	 * 标签名称
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