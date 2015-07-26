#pragma once

class CPrettyButton : public CMFCButton
{
	DECLARE_DYNAMIC(CPrettyButton)

public:
	CPrettyButton();
	virtual ~CPrettyButton();

protected:
	virtual void PreSubclassWindow();

protected:
	virtual void OnFillBackground(CDC* pDC, const CRect& rectClient);
	virtual void OnDrawText(CDC* pDC, const CRect& rect, const CString& strText, UINT uiDTFlags, UINT uiState);

	DECLARE_MESSAGE_MAP()
protected:
};