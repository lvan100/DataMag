#pragma once

class CLabelNameDlg : public CDialog
{
	DECLARE_DYNAMIC(CLabelNameDlg)

public:
	CLabelNameDlg(CWnd* pParent = NULL);
	virtual ~CLabelNameDlg();

	enum { IDD = IDD_LABELNAMEDLG };

	enum {
		New,
		Rename,
	}Op;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV Ö§³Ö

	DECLARE_MESSAGE_MAP()

public:
	CString m_label_name;
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
};
