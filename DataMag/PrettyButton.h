#pragma once

#include "HilitButton.h"

/**
 * 漂亮的按钮控件，图片在左，文字在右，并
 * 且支持多行文字显示，支持注释性文字显示.
 */
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