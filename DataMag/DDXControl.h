#pragma once

/**
 * 对 DDX_Control 的封装，用于解决 VS2010 旗舰版的 CMFCButton 等控
 * 件在绑定过程中产生的内存泄漏问题。对这个问题追根溯源的解释参见：
 * http://blog.163.com/lvan100%40yeah/blog/static/681172142010102651640704/
 */
void MFC_DDX_Control(CDataExchange* pDX, int nIDC, CWnd& rControl);