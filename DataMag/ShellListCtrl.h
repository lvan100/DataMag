#pragma once

/**
 * Shell列表事件接口
 */
class ShellListEvent
{
public:
	/**
	 * 初始化列表控件
	 */
	virtual void InitShellList() = 0;

	/**
	 * 列表项双击事件
	 */
	virtual void OnDoubleClick() = 0;

	/**
	 * 列表项发生变化
	 */
	virtual void OnSelectChanged() = 0;
};

/**
 * Shell列表事件接口适配器
 */
class CShellListEventAdapter : public ShellListEvent{

public:
	/**
	 * 初始化列表控件
	 */
	virtual void InitShellList() {}
	
	/**
	 * 列表项双击事件
	 */
	virtual void OnDoubleClick() {}

	/**
	 * 列表项发生变化
	 */
	virtual void OnSelectChanged() {}
};

/**
 * Shell列表控件
 */
class CShellListCtrl : public CMFCShellListCtrl
{
	DECLARE_DYNAMIC(CShellListCtrl)

public:
	/**
	 * 设置列表事件实现接口
	 */
	void SetListEvent(ShellListEvent* event){
		m_event = event;
	}

	/**
	 * 执行默认的双击操作
	 */
	void DoDefaultDClick();

protected:
	/**
	 * 客户端实现的事件对象
	 */
	ShellListEvent* m_event;

public:
	CShellListCtrl();
	virtual ~CShellListCtrl();

	/**
	 * 获取列表项的地址
	 *
	 * @param iItem
	 *        列表项的索引
	 * @return 列表项的地址
	 */
	CString GetItemPath(int iItem){
		CString strPath;
		CMFCShellListCtrl::GetItemPath(strPath, iItem);
		return strPath;
	}

	/**
	 * 获取当前目录地址
	 *
	 * @return 获取当前目录地址
	 */
	CString GetCurrentFolder(){
		CString strPath;
		CMFCShellListCtrl::GetCurrentFolder(strPath);
		return strPath;
	}

	/**
	 * 设置过滤字符串
	 *
	 * @param str
	 *        过滤字符串
	 */
	void SetFilterString(CString str){
		m_filter = str;
		m_filter.MakeLower();

		DisplayFolder(GetCurrentFolder());
	}

	/**
	 * 设置是否允许双击打开文件夹
	 *
	 * @param enable
	 *        是否允许打开文件夹
	 */
	void EnableDClickOpenFolder(BOOL enable)
	{
		m_bDClickOpenFolder = enable;
	}

protected:
	/**
	 * 过滤字符串
	 */
	CString m_filter;

	/**
	 * 双击打开文件夹
	 */
	BOOL m_bDClickOpenFolder;

	/**
	 * 当前目录
	 */
	CString m_strCurrentFolder;

protected:
	/**
	 * 设置列
	 */
	virtual void OnSetColumns();

	/**
	 * 子类化控件
	 */
	virtual void PreSubclassWindow();

	DECLARE_MESSAGE_MAP()
protected:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnDoubleClick(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnLvnItemchanged(NMHDR *pNMHDR, LRESULT *pResult);

public:
	/*
	 * 浏览指定目录
	 *
	 * @param lpszPath
	 *        指定目录
	 * @result 成功返回TRUE,否则返回FALSE
	 */
	virtual HRESULT DisplayFolder(LPCTSTR lpszPath)
	{
		if (PathFileExists(lpszPath))
		{
			m_strCurrentFolder = lpszPath;
			return CMFCShellListCtrl::DisplayFolder(lpszPath);
		}
		return FALSE;
	}

protected:
	virtual HRESULT EnumObjects(LPSHELLFOLDER pParentFolder
		, LPITEMIDLIST pidlParent);
	
	virtual BOOL InitList();

	virtual void DoDefault(int iItem);
};