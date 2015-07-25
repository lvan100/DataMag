#pragma once

#include "TabTemplate.h"

/**
 * ����������
 */
class CMainFrame : public CDialogEx
{
	DECLARE_DYNAMIC(CMainFrame)

public:
	/**
	 * ��������
	 */
	enum MFCommand 
	{ 
		OpenLabel,
		SearchLabel,
		OpenProject,
		SearchProject,
		OpenBook,
		SearchBook,
	};

public:
	CMainFrame(MFCommand cmd, CString arg, CWnd* pParent = nullptr);
	virtual ~CMainFrame();

	enum { IDD = IDD_MAINFRAME };

protected:
	/**
	 * ִ������
	 */
	MFCommand cmd;

	/**
	 * �������
	 */
	CString arg;

protected:
	class MainTabAdapter : public TabItemAdapter
	{
	public:
		/**
		 * Tab��ǩҳ������
		 *
		 * @return ���ؽ�Ҫ��ӵı�ǩҳ����
		 */
		virtual UINT GetCount();

		/**
		 * ��ȡTab��ǩҳ����
		 *
		 * @param nItem
		 *        ��ǩҳ����
		 * @return ���ر�ǩҳ����
		 */
		virtual LPCTSTR GetItemText(UINT nItem);

		/**
		 * ��ȡTab��ǩҳ����
		 *
		 * @param nItem
		 *        ��Ҫ�����ı�ǩҳ����
		 * @param parent
		 *        ���ؼ�
		 * @return ���ش����ɹ��ı�ǩҳ
		 */
		virtual CWnd* GetItem(UINT nItem, CWnd* parent);

	}m_main_tab_adapter;

protected:
	/**
	 * ����ͼ��
	 */
	HICON m_hIcon;

protected:
	CTabTemplate m_main_tab;

protected:
	virtual BOOL OnInitDialog();
	virtual void DoDataExchange(CDataExchange* pDX);

	DECLARE_MESSAGE_MAP()
protected:
	afx_msg void OnClickedSetting();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnActivateApp(BOOL bActive, DWORD dwThreadID);
};