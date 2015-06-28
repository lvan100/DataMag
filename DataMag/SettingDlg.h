#pragma once

/**
 * 应用设置对话框
 */
class CSettingDlg : public CDialog
{
	DECLARE_DYNAMIC(CSettingDlg)

public:
	CSettingDlg(CWnd* pParent = nullptr);
	virtual ~CSettingDlg();

	enum { IDD = IDD_SETTINGDLG };

protected:
	virtual BOOL OnInitDialog();
	virtual void DoDataExchange(CDataExchange* pDX);

public:
	CMFCEditBrowseCtrl m_book_browser;
	CMFCEditBrowseCtrl m_project_browser;

	DECLARE_MESSAGE_MAP()
protected:
	afx_msg void OnBnClickedOk();
};