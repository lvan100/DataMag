#pragma once

/**
 * 将窗口放置在指定区域的中心
 */
void CenterWindowInRect(CWnd* pWnd, CRect rect, BOOL bShow = TRUE);

/** 
 * 打开文件夹
 */
BOOL OpenFoler(LPCTSTR szDirectory);