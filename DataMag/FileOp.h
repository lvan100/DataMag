#pragma once

/**
 * ɾ��Ŀ¼�����õݹ�ɾ���ļ��ķ�ʽ
 *
 * @param strDir
 *        Ŀ¼
 */
void DeleteDirectory(CString strDir);

/**
 * ��ȡ�ļ���ݷ�ʽ��Ŀ���ַ
 *
 * @param strPath
 *        �ļ���ַ
 * @param strLink
 *        �ļ���ݷ�ʽ�ĵ�ַ
 * @return �ɹ����� TRUE��ʧ�ܷ��� FALSE��
 */
BOOL GetLinkFilePath(CString& strPath, CString strLink);

/**
 * �����ļ���ݷ�ʽ
 *
 * @param strPath
 *        �ļ���ַ
 * @param strLink
 *        �ļ���ݷ�ʽ�ĵ�ַ
 * @return �ɹ����� TRUE��ʧ�ܷ��� FALSE��
 */
BOOL CreateFileLink(CString strPath, CString strLink);