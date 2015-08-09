#include "StdAfx.h"
#include "WndHelp.h"

void CenterWindowInRect(CWnd* pWnd, CRect rect, BOOL bShow)
{
	CRect rcWnd;
	pWnd->GetWindowRect(rcWnd);

	CPoint pt = rect.CenterPoint();
	pt -= rcWnd.CenterPoint();
	rcWnd.OffsetRect(pt);

	pWnd->MoveWindow(rcWnd);

	int nCmdShow = bShow ? SW_SHOW : SW_HIDE;
	pWnd->ShowWindow(nCmdShow);
}