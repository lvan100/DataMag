#pragma once

#include "LabelListCtrl.h"
#include "LabelInfoCtrl.h"
#include "afxcmn.h"

class CDataMagDlg : public CDialogEx
{
public:
	CDataMagDlg(CWnd* pParent = NULL);

	enum { IDD = IDD_DATAMAG_DIALOG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);

protected:
	HICON m_hIcon;

protected:
	DECLARE_MESSAGE_MAP()
	virtual BOOL OnInitDialog();
	virtual BOOL PreTranslateMessage(MSG* pMsg);

public:
	CRichEditCtrl m_item_text;
	CLabelListCtrl m_label_list;
	CLabelInfoCtrl m_label_info;
	afx_msg void OnBnClickedSetting();
	afx_msg void OnBnClickedLabelAdd();
	afx_msg void OnBnClickedLabelDelete();
	afx_msg void OnBnClickedLabelRename();
	afx_msg void OnBnClickedLabelRelateBook();
	afx_msg void OnBnClickedLabelRelateProject();
};

extern CDataMagDlg* theDataMagDlg;