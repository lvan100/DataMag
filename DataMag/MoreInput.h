#pragma once

/**
 * ������������������֡��Ŀؼ�
 */
class CMoreInput : public CStatic
{
	DECLARE_DYNAMIC(CMoreInput)

public:
	CMoreInput();
	virtual ~CMoreInput();
	
	/**
	 * ���û���������������ַ�
	 */
	void SetMoreInputCount(int count);

	/**
	 * ������������ʾ�����ַ�
	 */
	void SetMaxInputCount(int count);

protected:
	int m_nMaxInputCount;
	int m_nMoreInputCount;

	DECLARE_MESSAGE_MAP()
protected:
	afx_msg void OnPaint();
};