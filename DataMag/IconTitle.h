#pragma once

/**
 * ����ͼ��ı���ؼ�
 */
class CIconTitle : public CStatic
{
	DECLARE_DYNAMIC(CIconTitle)

public:
	CIconTitle();
	virtual ~CIconTitle();

	/**
	 * ����ͼ��
	 */
	void SetIcon(HICON hIcon) {
		m_hIcon = hIcon;
	}

protected:
	/**
	 * ͼ��
	 */
	HICON m_hIcon;

protected:
	DECLARE_MESSAGE_MAP()
	afx_msg void OnPaint();
};