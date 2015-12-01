#pragma once

#include "IconTitle.h"

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
	}

	/**
	 * ����Դ���ͼ��
	 */
	void SetCodeImage(HICON hIcon) {
		m_hCodeImage = hIcon;
	}

	/**
	 * ����ͼ���ͼ��
	 */
	void SetBookImage(HICON hIcon) {
		m_hBookImage = hIcon;
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