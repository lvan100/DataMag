#pragma once

/**
 * �����Ի���
 */
class CNameDlg : public CDialog
{
	DECLARE_DYNAMIC(CNameDlg)

public:
	CNameDlg(CWnd* pParent = nullptr);
	virtual ~CNameDlg();

	enum { IDD = IDD_NAMEDLG };

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
	CString m_name;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);

	DECLARE_MESSAGE_MAP()
protected:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
};