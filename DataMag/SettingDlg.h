#pragma once

#include "resource.h"

/**
 * �����ļ�
 */
class CSetting
{
public:
	/**
	 * �������Ŀ¼
	 */
	CString strMagFolder;

public:
	/**
	 * ���������ļ�
	 *
	 * @param strFile
	 *        �����ļ��ģ����ԣ�·��
	 */
	void Load(CString strFile);

	/**
	 * ���������ļ�
	 */
	void Save();

protected:
	/**
	 * �����ļ�·��
	 */
	CString strIniFile;
};

/**
 * ����ȫ�ֵ����ö���
 */
extern CSetting theSetting;

/**
 * ���öԻ���
 */
class CSettingDlg : public CDialog
{
	DECLARE_DYNAMIC(CSettingDlg)

public:
	CSettingDlg(CWnd* pParent = NULL);
	virtual ~CSettingDlg();

	enum { IDD = IDD_SETTINGDLG };

public:
	CMFCEditBrowseCtrl m_folder_select;

protected:
	virtual BOOL OnInitDialog();
	virtual void DoDataExchange(CDataExchange* pDX);

protected:
	DECLARE_MESSAGE_MAP()
	afx_msg void OnBnClickedOk();
};