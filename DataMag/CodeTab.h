#pragma once

#include "AppWnd.h"

#include "MoreInput.h"
#include "SearchEdit.h"
#include "FileListBox.h"
#include "TagItemEdit.h"
#include "HilitButton.h"
#include "ItemInfoEdit.h"

/**
 * 项目标签页
 */
class CCodeTab : public CAppWnd, CListBoxEventAdapter
{
	DECLARE_DYNAMIC(CCodeTab)

public:
	/**
	 * 命令格式:"open","search:xx","add".
	 */
	CCodeTab(CString strCommand = _T(""), CWnd* pParent = nullptr);
	virtual ~CCodeTab();

	enum { IDD = IDD_CODETAB };

	/**
	 * 响应项目管理目录改变事件
	 */
	void OnCodeMagDirChange(CString dir){
		m_project_list.DisplayFolder(dir);
	}

	/**
	 * 响应内容输入框变化事件
	 */
	void OnItemEditChange();

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

	/**
	 * 编辑和禁止编辑图标
	 */
	HICON m_hCanEditIcon;
	HICON m_hNotEditIcon;

protected:
	/**
	 * 执行重命名操作
	 */
	CString RenameBook(function<CString()> getSelName, function<CString()> getSelPath);

	/**
	 * 是否允许信息可以编辑
	 */
	void EnableInfoEidt(BOOL enable);

protected:
	/**
	 * 获取默认焦点控件
	 */
	virtual CWnd* GetDefaultFocusWnd()
	{ return &m_search_edit; }

protected:
	virtual BOOL OnInitDialog();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	virtual void DoDataExchange(CDataExchange* pDX);

protected:
	CHilitButton m_setting;
	CHilitButton m_project_add;
	CHilitButton m_modify_info;
	CHilitButton m_project_delete;
	CHilitButton m_project_rename;
	CHilitButton m_project_refresh;

	CSearchEdit m_search_edit;
	CItemInfoEdit m_item_text;
	CFileListBox m_project_list;

	CMoreInput m_more_input;
	CTagItemEdit m_tag_group;
	CStatic m_tag_group_title;

	DECLARE_MESSAGE_MAP()
protected:
	afx_msg void OnDestroy();
	afx_msg void OnMove(int x, int y);
	afx_msg void OnBnClickedSetting();
	afx_msg void OnBnClickedProjectAdd();
	afx_msg void OnBnClickedModifyInfo();
	afx_msg void OnBnClickedProjectDelete();
	afx_msg void OnBnClickedProjectRename();
	afx_msg void OnBnClickedProjectRefresh();
	afx_msg void OnChangeProjectSearchEdit();
	afx_msg void OnDropFiles(HDROP hDropInfo);
};