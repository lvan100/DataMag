#pragma once

#include "SearchEdit.h"
#include "FileListBox.h"
#include "HilitButton.h"

/**
 * 项目标签页
 */
class CProjectTab : public CDialogEx, CListBoxEventAdapter
{
	DECLARE_DYNAMIC(CProjectTab)

public:
	/**
	 * 命令格式:"open","search:xx","add".
	 */
	CProjectTab(CString strCommand = _T(""), CWnd* pParent = NULL);
	virtual ~CProjectTab();

	enum { IDD = IDD_PROJECTTAB };

	/**
	 * 响应项目管理目录改变事件
	 */
	void OnCodeMagDirChange(CString dir){
		m_project_list.DisplayFolder(dir);
	}

public:
	/**
	 * 初始化列表框控件
	 */
	virtual void InitListBox();

	/**
	 * 列表项双击事件
	 */
	virtual void OnDoubleClick();
	
	/**
	 * 列表项发生变化
	 */
	virtual void OnSelectChanged();

protected:
	/**
	 * 窗体图标
	 */
	HICON m_hIcon;

	/**
	 * 描述文件的内容
	 */
	CStringA strText;
	
	/**
	 * 执行命令
	 */
	struct Command{
		CString cmd;
		CString arg;
	}m_command;

protected:
	virtual BOOL OnInitDialog();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	virtual void DoDataExchange(CDataExchange* pDX);

protected:
	CHilitButton m_setting;
	CHilitButton m_project_add;
	CHilitButton m_project_delete;
	CHilitButton m_project_rename;
	CHilitButton m_project_refresh;

	CSearchEdit m_search_edit;
	CRichEditCtrl m_item_text;
	CFileListBox m_project_list;

	DECLARE_MESSAGE_MAP()
protected:
	afx_msg void OnBnClickedSetting();
	afx_msg void OnBnClickedProjectAdd();
	afx_msg void OnBnClickedProjectDelete();
	afx_msg void OnBnClickedProjectRename();
	afx_msg void OnBnClickedProjectRefresh();
	afx_msg void OnChangeProjectSearchEdit();
	afx_msg void OnDropFiles(HDROP hDropInfo);
	afx_msg void OnActivate(UINT nState, CWnd* pWndOther, BOOL bMinimized);
};