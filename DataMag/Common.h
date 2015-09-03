#pragma once

/**
 * 将窗口移动到指定区域的中心位置
 *
 * @param pWnd
 *        窗口指针
 * @param rect
 *        目标区域
 * @param bShowWnd
 *        是否显示窗口
 */
void CenterWindowInRect(CWnd* pWnd, CRect rect, BOOL bShowWnd = TRUE);

/** 
 * 通过资源管理器打开文件夹
 *
 * @param szDirectory
 *        文件夹路径
 * @return 成功返回 TRUE，失败返回 FALSE。
 */
BOOL OpenFolerInShell(LPCTSTR szDirectory);