#pragma once

/**
 * 带有图标的标题控件
 */
class CIconTitle : public CStatic
{
	DECLARE_DYNAMIC(CIconTitle)

public:
	CIconTitle();
	virtual ~CIconTitle();

	/**
	 * 设置图标
	 */
	void SetIcon(HICON hIcon) {
		m_hIcon = hIcon;
	}

protected:
	/**
	 * 图标
	 */
	HICON m_hIcon;

protected:
	DECLARE_MESSAGE_MAP()
	afx_msg void OnPaint();
};