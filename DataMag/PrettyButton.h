#pragma once

#include "HilitButton.h"

class CPrettyButton : public CHilitButton
{
	DECLARE_DYNAMIC(CPrettyButton)

public:
	CPrettyButton();
	virtual ~CPrettyButton();

protected:
	virtual void OnDrawText(CDC* pDC, const CRect& rect, const CString& strText, UINT uiDTFlags, UINT uiState);

	DECLARE_MESSAGE_MAP()
protected:
};