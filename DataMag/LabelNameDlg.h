#pragma once

/**
 * 标签命名对话框
 */
class CLabelNameDlg : public CDialog
{
	DECLARE_DYNAMIC(CLabelNameDlg)

public:
	CLabelNameDlg(CWnd* pParent = NULL);
	virtual ~CLabelNameDlg();

	enum { IDD = IDD_LABELNAMEDLG };

public:
	/**
	 * 操作类型
	 */
	enum {
		New,		/* 新建	*/
		Rename,		/* 重命名 */
	}Op;

	/**
	 * 标签名称
	 */
	CString m_label_name;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);

protected:
	DECLARE_MESSAGE_MAP()
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
};
