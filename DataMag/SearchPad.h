#pragma once

/**
 * ���ؽ�����
 */
class CSearchPad : public CDialogEx
{
	DECLARE_DYNAMIC(CSearchPad)

public:
	CSearchPad(CWnd* pParent = NULL);
	virtual ~CSearchPad();

protected:
	virtual void DoDataExchange(CDataExchange* pDX);

protected:
	DECLARE_MESSAGE_MAP()
};