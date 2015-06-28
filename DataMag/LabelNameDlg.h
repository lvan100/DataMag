#pragma once

/**
 * 标签命名对话框
 */
class CLabelNameDlg : public CDialog
{
	DECLARE_DYNAMIC(CLabelNameDlg)

public:
	CLabelNameDlg(CWnd* pParent = nullptr);
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

	DECLARE_MESSAGE_MAP()
protected:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
};