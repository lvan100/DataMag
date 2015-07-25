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
	 * @return ����ѡ��ı�ǩҳ����
	 */
	CWnd* SelectTab(UINT nItem);
	
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
		TAB_WIDTH = 108,
		TAB_HEIGHT = 41,
	};

	/**
	 * Pedding
	 */
	enum {
		PEDDING_TOP = 8,
		PEDDING_LEFT = 2,
		PEDDING_RIGHT = 2,
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