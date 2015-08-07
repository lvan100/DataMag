#include "StdAfx.h"
#include "WndHelp.h"

void CenterWindowInRect(CWnd* pWnd, CRect rect)
{
	CRect rcWnd;
	pWnd->GetWindowRect(rcWnd);

	CPoint pt = rect.CenterPoint();
	pt -= rcWnd.CenterPoint();
	rcWnd.OffsetRect(pt);

	pWnd->MoveWindow(rcWnd);
}