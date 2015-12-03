#pragma once

#include "IconTitle.h"
#include "FolderList.h"
#include "PrettyButton.h"

#include <vector>
using namespace std;

/**
 * 详情页面
 */
class CDetailPage : public CDialogEx
{
	DECLARE_DYNAMIC(CDetailPage)

public:
	CDetailPage(CWnd* pParent = NULL);
	virtual ~CDetailPage();

	/**
	 * 刷新显示
	 */
	void Refresh(CString strCatalog, CString strPath);

protected:
	/**
	 * 路径
	 */
	CString m_path;

	/**
	 * 类别
	 */
	CString m_catalog;

public:
	/**
	 * 设置标签的图标
	 */
	void SetTagImage(HICON hIcon) {
		m_hTagImage = hIcon;
	}

	/**
	 * 设置源码的图标
	 */
	void SetCodeImage(HICON hIcon) {
		m_hCodeImage = hIcon;
	}

	/**
	 * 设置图书的图标
	 */
	void SetBookImage(HICON hIcon) {
		m_hBookImage = hIcon;
	}

	/**
	 * 设置改名的图标
	 */
	void SetRenameImage(HICON hIcon) {
		m_hRenameImg = hIcon;
	}

	/**
	 * 设置详情的图标
	 */
	void SetInfoImage(HICON hIcon) {
		m_hInfoImage = hIcon;
	}

	/**
	 * 设置删除的图标
	 */
	void SetDeleteImage(HICON hIcon) {
		m_hDeleteImg = hIcon;
	}

protected:
	/**
	 * 获取类别图像
	 */
	HICON GetCatalogImage(CString catalog);

protected:
	HICON m_hTagImage;  /* 标签 */
	HICON m_hCodeImage; /* 源码 */
	HICON m_hBookImage; /* 图书 */
	HICON m_hRenameImg; /* 改名 */
	HICON m_hInfoImage; /* 详情 */
	HICON m_hDeleteImg; /* 删除 */

	/**
	 * 项目描述
	 */
	CStringA m_text;

	/**
	 * 链接列表
	 */
	vector<CString> m_link_value;

	CComboBox m_link_filter;
	CHilitButton m_add_link;
	CIconTitle m_detail_title;
	CHilitButton m_remove_link;
	CRichEditCtrl m_detial_info;
	CFolderListCtrl m_link_list;

protected:
	/**
	 * 关联列表控件事件对象
	 */
	class LinkListEvent : public ListBoxEvent
	{
	public:
		/**
		 * 列表项双击事件
		 */
		virtual void OnDoubleClick();

	} m_link_list_event;

protected:
	virtual BOOL OnInitDialog();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	virtual void DoDataExchange(CDataExchange* pDX);

protected:
	DECLARE_MESSAGE_MAP()
	afx_msg void OnCbnSelchangeLinkFilter();
public:
	afx_msg void OnBnClickedAddLink();
	afx_msg void OnBnClickedRemoveLink();
};