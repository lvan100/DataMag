#pragma once

/**
 * 符合当前应用风格的窗口
 */
class CAppWnd : public CDialogEx
{
	DECLARE_DYNAMIC(CAppWnd)

public:
	CAppWnd(UINT nIDTemplate, CWnd* pParent);
	virtual ~CAppWnd();

protected:
	/**
	 * 窗体图标
	 */
	HICON m_hIcon;

protected:
	virtual BOOL OnInitDialog();
	virtual void DoDataExchange(CDataExchange* pDX);

	DECLARE_MESSAGE_MAP()
protected:
};