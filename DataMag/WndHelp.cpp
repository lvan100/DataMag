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

BOOL OpenFoler(LPCTSTR szDirectory)
{
	/* ShellExecute : If the function succeeds, it returns a value greater than 32. */
	return (int(ShellExecute(nullptr, L"open", szDirectory, nullptr, nullptr, SW_MAXIMIZE)) > 32);
}