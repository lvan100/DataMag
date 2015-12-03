#pragma once

#include "IconTitle.h"
#include "FolderList.h"
#include "PrettyButton.h"

#include <vector>
using namespace std;

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
	 * ˢ����ʾ
	 */
	void Refresh(CString strCatalog, CString strPath);

protected:
	/**
	 * ·��
	 */
	CString m_path;

	/**
	 * ���
	 */
	CString m_catalog;

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

	/**
	 * �����б�
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
	 * �����б�ؼ��¼�����
	 */
	class LinkListEvent : public ListBoxEvent
	{
	public:
		/**
		 * �б���˫���¼�
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