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
	 * 获取默认焦点控件
	 */
	virtual CWnd* GetDefaultFocusWnd()
	{ return NULL; }

protected:
	/**
	 * 应用图标
	 */
	HICON m_hIcon;

	/**
	 * 最后获得焦点的控件，不记录焦点在切换过程
	 * 中的变化，只用于窗口失去激活状态前的焦点
	 */
	CWnd* m_pLastFocusWnd;

protected:
	virtual BOOL OnInitDialog();
	virtual void DoDataExchange(CDataExchange* pDX);

	DECLARE_MESSAGE_MAP()
protected:
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnActivate(UINT nState, CWnd* pWndOther, BOOL bMinimized);
};