#pragma once

#include "TabTemplate.h"

/**
 * 程序主窗口
 */
class CMainFrame : public CDialogEx
{
	DECLARE_DYNAMIC(CMainFrame)

public:
	CMainFrame(CWnd* pParent = nullptr);
	virtual ~CMainFrame();

	enum { IDD = IDD_MAINFRAME };

protected:
	class MainTabAdapter : public TabItemAdapter
	{
	public:
		/**
		 * Tab标签页的数量
		 *
		 * @return 返回将要添加的标签页数量
		 */
		virtual UINT GetCount();

		/**
		 * 获取Tab标签页文字
		 *
		 * @param nItem
		 *        标签页索引
		 * @return 返回标签页文字
		 */
		virtual LPCTSTR GetItemText(UINT nItem);

		/**
		 * 获取Tab标签页对象
		 *
		 * @param nItem
		 *        将要创建的标签页索引
		 * @param parent
		 *        父控件
		 * @return 返回创建成功的标签页
		 */
		virtual CWnd* GetItem(UINT nItem, CWnd* parent);

	}m_main_tab_adapter;

protected:
	/**
	 * 窗体图标
	 */
	HICON m_hIcon;

protected:
	CTabTemplate m_main_tab;

protected:
	virtual BOOL OnInitDialog();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	virtual void DoDataExchange(CDataExchange* pDX);

	DECLARE_MESSAGE_MAP()
protected:
	afx_msg void OnClickedSetting();
};