#pragma once

/**
 * ��ǩ�����Ի���
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

	DECLARE_MESSAGE_MAP()
protected:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
};