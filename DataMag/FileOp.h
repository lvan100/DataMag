#pragma once

/**
 * 删除目录，采用递归删除文件的方式
 *
 * @param strDir
 *        目录
 */
void DeleteDirectory(CString strDir);

/**
 * 获取文件快捷方式的目标地址
 *
 * @param strPath
 *        文件地址
 * @param strLink
 *        文件快捷方式的地址
 * @return 成功返回 TRUE，失败返回 FALSE。
 */
BOOL GetLinkFilePath(CString& strPath, CString strLink);

/**
 * 创建文件快捷方式
 *
 * @param strPath
 *        文件地址
 * @param strLink
 *        文件快捷方式的地址
 * @return 成功返回 TRUE，失败返回 FALSE。
 */
BOOL CreateFileLink(CString strPath, CString strLink);