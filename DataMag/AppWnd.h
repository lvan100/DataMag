#pragma once

/**
 * ���ϵ�ǰӦ�÷��Ĵ���
 */
class CAppWnd : public CDialogEx
{
	DECLARE_DYNAMIC(CAppWnd)

public:
	CAppWnd(UINT nIDTemplate, CWnd* pParent);
	virtual ~CAppWnd();

protected:
	/**
	 * ��ȡĬ�ϵĽ���ؼ�
	 */
	virtual CWnd* GetDefaultFocusWnd()
	{ return nullptr; }

protected:
	/**
	 * Ӧ��ͼ��
	 */
	HICON m_hIcon;

	/**
	 * ����ý���Ŀؼ�������¼�������л�����
	 * �еı仯��ֻ���ڴ���ʧȥ����״̬ǰ�Ľ���
	 */
	CWnd* m_pLastFocusWnd;

protected:
	virtual BOOL OnInitDialog();
	virtual void DoDataExchange(CDataExchange* pDX);

	DECLARE_MESSAGE_MAP()
protected:
	afx_msg void OnActivate(UINT nState, CWnd* pWndOther, BOOL bMinimized);
};