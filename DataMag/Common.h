#pragma once

/**
 * �������ƶ���ָ�����������λ��
 *
 * @param pWnd
 *        ����ָ��
 * @param rect
 *        Ŀ������
 * @param bShowWnd
 *        �Ƿ���ʾ����
 */
void CenterWindowInRect(CWnd* pWnd, CRect rect, BOOL bShowWnd = TRUE);

/** 
 * ͨ����Դ���������ļ���
 *
 * @param szDirectory
 *        �ļ���·��
 * @return �ɹ����� TRUE��ʧ�ܷ��� FALSE��
 */
BOOL OpenFolerInShell(LPCTSTR szDirectory);