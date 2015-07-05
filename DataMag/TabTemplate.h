#pragma once

#include <map>
#include <vector>
using namespace std;

/**
 * Tab��ǩҳ�������ӿ�
 */
class TabItemAdapter
{
public:
	/**
	 * Tab��ǩҳ������
	 *
	 * @return ���ؽ�Ҫ��ӵı�ǩҳ����
	 */
	virtual UINT GetCount() = 0;

	/**
	 * ��ȡTab��ǩҳ����
	 *
	 * @param nItem
	 *        ��ǩҳ����
	 * @return ���ر�ǩҳ����
	 */
	virtual LPCTSTR GetItemText(UINT nItem) = 0;

	/**
	 * ��ȡTab��ǩҳ����
	 *
	 * @param nItem
	 *        ��ǩҳ����
	 * @param parent
	 *        ���ؼ�
	 * @return ���ش����ɹ��ı�ǩҳ
	 */
	virtual CWnd* GetItem(UINT nItem, CWnd* parent) = 0;
};

/**
 * Tab�ؼ�ģ��
 */
class CTabTemplate : public CTabCtrl
{
	DECLARE_DYNAMIC(CTabTemplate)

public:
	CTabTemplate();
	virtual ~CTabTemplate();

public:
	/**
	 * �����������ӿ�
	 *
	 * @param adapter
	 *        �������ӿڶ���
	 */
	void SetAdapter(TabItemAdapter* adapter){
		m_adpter = adapter;
	}
	
	/**
	 * ѡ���ǩҳ
	 *
	 * @patam nItem
	 *        ��ǩҳ����
	 * @return �ɹ�����TRUE��ʧ�ܷ���FALSE��
	 */
	BOOL SelectTab(UINT nItem);
	
	/**
	 * ɾ����ǩҳ
	 *
	 * @patam nItem
	 *        ��ǩҳ����
	 * @return �ɹ�����TRUE��ʧ�ܷ���FALSE��
	 */
	BOOL DeleteTab(UINT nItem);

	/**
	 * ɾ�����б�ǩҳ
	 */
	void DeleteAllTab();

protected:
	/**
	 * �������ӿ�
	 */
	TabItemAdapter* m_adpter;

	/**
	 * ��ǩҳ�б�
	 */
	vector<CWnd*> m_content;

protected:
	/**
	 * ��ǩҳ���
	 */
	enum {
		TAB_WIDTH = 128,
		TAB_HEIGHT = 48
	};

	/**
	 * Pedding
	 */
	enum {
		PEDDING_TOP = 15,
		PEDDING_LEFT = 3,
		PEDDING_RIGHT = 3,
		PEDDING_BOTTOM = 2 
	};

protected:
	/**
	 * ��ʼ���ؼ�
	 */
	virtual BOOL Init();

	/**
	 * ���¼������ݴ�С
	 */
	void ResizeContent();

protected:
	virtual void PreSubclassWindow();

	DECLARE_MESSAGE_MAP()
protected:
	afx_msg void OnDestroy();
	afx_msg void OnSetFocus(CWnd* pOldWnd);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnTcnSelchange(NMHDR *pNMHDR, LRESULT *pResult);
};