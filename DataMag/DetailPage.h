#pragma once

#include "IconTitle.h"
#include "FolderList.h"
#include "PrettyButton.h"

/**
 * ����ҳ��
 */
class CDetailPage : public CDialogEx
{
	DECLARE_DYNAMIC(CDetailPage)

public:
	CDetailPage(CWnd* pParent = NULL);
	virtual ~CDetailPage();

	/**
	 * �������
	 */
	void SetCatalog(CString str) {
		m_catalog = str;
	}

	/**
	 * ����·��
	 */
	void SetPath(CString path) {
		m_path = path;
	}

	/**
	 * ׼������
	 */
	void Prepare();

protected:
	/**
	 * ���
	 */
	CString m_catalog;

	/**
	 * ·��
	 */
	CString m_path;

public:
	/**
	 * ���ñ�ǩ��ͼ��
	 */
	void SetTagImage(HICON hIcon) {
		m_hTagImage = hIcon;
		m_link_list.SetTagImage(m_hTagImage);
	}

	/**
	 * ����Դ���ͼ��
	 */
	void SetCodeImage(HICON hIcon) {
		m_hCodeImage = hIcon;
		m_link_list.SetCodeImage(m_hCodeImage);
	}

	/**
	 * ����ͼ���ͼ��
	 */
	void SetBookImage(HICON hIcon) {
		m_hBookImage = hIcon;
		m_link_list.SetBookImage(m_hBookImage);
	}

	/**
	 * ���ø�����ͼ��
	 */
	void SetRenameImage(HICON hIcon) {
		m_hRenameImg = hIcon;
	}

	/**
	 * ���������ͼ��
	 */
	void SetInfoImage(HICON hIcon) {
		m_hInfoImage = hIcon;
	}

	/**
	 * ����ɾ����ͼ��
	 */
	void SetDeleteImage(HICON hIcon) {
		m_hDeleteImg = hIcon;
	}

protected:
	/**
	 * ��ȡ���ͼ��
	 */
	HICON GetCatalogImage(CString catalog);

protected:
	HICON m_hTagImage;  /* ��ǩ */
	HICON m_hCodeImage; /* Դ�� */
	HICON m_hBookImage; /* ͼ�� */
	HICON m_hRenameImg; /* ���� */
	HICON m_hInfoImage; /* ���� */
	HICON m_hDeleteImg; /* ɾ�� */

	/**
	 * ��Ŀ����
	 */
	CStringA m_text;

	CComboBox m_link_filter;
	CHilitButton m_add_link;
	CIconTitle m_detail_title;
	CHilitButton m_remove_link;
	CRichEditCtrl m_detial_info;
	CFolderListCtrl m_link_list;

protected:
	virtual BOOL OnInitDialog();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	virtual void DoDataExchange(CDataExchange* pDX);

protected:
	DECLARE_MESSAGE_MAP()
};