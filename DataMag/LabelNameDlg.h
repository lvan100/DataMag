#pragma once

/**
 * ��ǩ�����Ի���
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
	 * ��������
	 */
	enum {
		New,		/* �½�	*/
		Rename,		/* ������ */
	}Op;

	/**
	 * ��ǩ����
	 */
	CString m_label_name;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);

protected:
	DECLARE_MESSAGE_MAP()
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
};
