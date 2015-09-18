#pragma once

#include "MoreInput.h"
#include "PrettyEdit.h"
#include "SearchEdit.h"
#include "FileListBox.h"
#include "HilitButton.h"
#include "ItemInfoEdit.h"

/**
 * 图书页面
 */
class CBookTab : public CDialogEx, CListBoxEventAdapter
{
	DECLARE_DYNAMIC(CBookTab)

public:
	CBookTab(CWnd* pParent);
	virtual ~CBookTab();

	enum { IDD = IDD_BOOKTAB };

	/**
	 * 响应添加事件
	 */
	void DoCommandAdd();

	/**
	 * 响应搜索事件
	 */
	void DoCommandSearch(CString str);

public:
	/**
	 * 响应图书管理目录改变事件
	 */
	void OnBookMagDirChange(CString dir){
		m_book_list.DisplayFolder(dir);
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

	/**
	 * 销毁自身对话框
	 */
	void DestroyThisWindow();

protected:
	/**
	 * 获取默认的焦点控件
	 */
	virtual CWnd* GetDefaultFocusWnd()
	{ return &m_search_edit; }
	
	/**
	 * 最后获得焦点的控件，不记录焦点在切换过程
	 * 中的变化，只用于窗口失去激活状态前的焦点
	 */
	CWnd* m_pLastFocusWnd;

protected:
	virtual BOOL OnInitDialog();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	virtual void DoDataExchange(CDataExchange* pDX);

protected:
	/**
	 * 按钮控件
	 */
	CHilitButton m_setting;
	CHilitButton m_book_add;
	CHilitButton m_book_delete;
	CHilitButton m_book_rename;
	CHilitButton m_modify_info;
	CHilitButton m_refresh_list;

	/**
	 * 图书列表控件
	 */
	CFileListBox m_book_list;
	CSearchEdit m_search_edit;
	CItemInfoEdit m_item_text;

	/**
	 * 图书详情控件
	 */
	CMoreInput m_more_input;
	CPrettyEdit m_tag_group;
	CStatic m_tag_group_title;
	
	DECLARE_MESSAGE_MAP()
protected:
	afx_msg void OnDestroy();
	afx_msg void OnMove(int x, int y);
	afx_msg void OnBnClickedBookAdd();
	afx_msg void OnBnClickedSetting();
	afx_msg void OnBnClickedBookDelete();
	afx_msg void OnBnClickedBookRename();
	afx_msg void OnBnClickedModifyInfo();
	afx_msg void OnBnClickedBookRefresh();
	afx_msg void OnChangeBookSearchEdit();
	afx_msg void OnSetFocus(CWnd* pOldWnd);
	afx_msg void OnDropFiles(HDROP hDropInfo);
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnActivate(UINT nState, CWnd* pWndOther, BOOL bMinimized);
};