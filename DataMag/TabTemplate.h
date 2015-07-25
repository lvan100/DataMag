#pragma once

#include <map>
#include <vector>
using namespace std;

/**
 * Tab标签页适配器接口
 */
class TabItemAdapter
{
public:
	/**
	 * Tab标签页的数量
	 *
	 * @return 返回将要添加的标签页数量
	 */
	virtual UINT GetCount() = 0;

	/**
	 * 获取Tab标签页文字
	 *
	 * @param nItem
	 *        标签页索引
	 * @return 返回标签页文字
	 */
	virtual LPCTSTR GetItemText(UINT nItem) = 0;

	/**
	 * 获取Tab标签页对象
	 *
	 * @param nItem
	 *        标签页索引
	 * @param parent
	 *        父控件
	 * @return 返回创建成功的标签页
	 */
	virtual CWnd* GetItem(UINT nItem, CWnd* parent) = 0;
};

/**
 * Tab控件模板
 */
class CTabTemplate : public CTabCtrl
{
	DECLARE_DYNAMIC(CTabTemplate)

public:
	CTabTemplate();
	virtual ~CTabTemplate();

public:
	/**
	 * 设置适配器接口
	 *
	 * @param adapter
	 *        适配器接口对象
	 */
	void SetAdapter(TabItemAdapter* adapter){
		m_adpter = adapter;
	}
	
	/**
	 * 选择标签页
	 *
	 * @patam nItem
	 *        标签页索引
	 * @return 返回选择的标签页内容
	 */
	CWnd* SelectTab(UINT nItem);
	
	/**
	 * 删除标签页
	 *
	 * @patam nItem
	 *        标签页索引
	 * @return 成功返回TRUE，失败返回FALSE。
	 */
	BOOL DeleteTab(UINT nItem);

	/**
	 * 删除所有标签页
	 */
	void DeleteAllTab();

protected:
	/**
	 * 适配器接口
	 */
	TabItemAdapter* m_adpter;

	/**
	 * 标签页列表
	 */
	vector<CWnd*> m_content;

protected:
	/**
	 * 标签页宽高
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
	 * 初始化控件
	 */
	virtual BOOL Init();

	/**
	 * 重新计算内容大小
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