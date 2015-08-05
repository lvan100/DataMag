#pragma once

/**
 * 对 DDX_Control 的封装，用于解决 VS2010 开始的  
 * CMFCButton 等在控件绑定过程中产生的内存泄漏问题
 */
void MFC_DDX_Control(CDataExchange* pDX, int nIDC, CWnd& rControl);