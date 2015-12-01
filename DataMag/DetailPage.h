#pragma once

#include "IconTitle.h"

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
	 * 设置类别
	 */
	void SetCatalog(CString str) {
		m_catalog = str;
	}

	/**
	 * 设置路径
	 */
	void SetPath(CString path) {
		m_path = path;
	}

	/**
	 * 准备工作
	 */
	void Prepare();

protected:
	/**
	 * 类别
	 */
	CString m_catalog;

	/**
	 * 路径
	 */
	CString m_path;

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

	CListBox m_link_list;
	CMFCButton m_add_link;
	CComboBox m_link_filter;
	CMFCButton m_remove_link;
	CIconTitle m_detail_title;
	CRichEditCtrl m_detial_info;

protected:
	virtual BOOL OnInitDialog();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	virtual void DoDataExchange(CDataExchange* pDX);

protected:
	DECLARE_MESSAGE_MAP()
};