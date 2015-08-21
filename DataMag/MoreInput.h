#pragma once

/**
 * “还可以再输入多少字”的控件
 */
class CMoreInput : public CStatic
{
	DECLARE_DYNAMIC(CMoreInput)

public:
	CMoreInput();
	virtual ~CMoreInput();
	
	/**
	 * 设置还可以再输入多少字符
	 */
	void SetMoreInputCount(int count);

	/**
	 * 设置最多可以显示多少字符
	 */
	void SetMaxInputCount(int count);

protected:
	int m_nMaxInputCount;
	int m_nMoreInputCount;

	DECLARE_MESSAGE_MAP()
protected:
	afx_msg void OnPaint();
};