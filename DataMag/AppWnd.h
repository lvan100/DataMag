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
	 * ����ͼ��
	 */
	HICON m_hIcon;

protected:
	virtual BOOL OnInitDialog();
	virtual void DoDataExchange(CDataExchange* pDX);

	DECLARE_MESSAGE_MAP()
protected:
};