#pragma once

#include <map>
#include <vector>
using namespace std;

/**
 * 搜素结果面板
 */
class CSearchPad : public CDialogEx
{
	DECLARE_DYNAMIC(CSearchPad)

public:
	CSearchPad(CWnd* pParent = NULL);
	virtual ~CSearchPad();

	/**
	 * 搜索类型
	 */
	void SetSearchFilter(CString str) {
		m_search_filter = str;
	}

	/**
	 * 搜索内容
	 */
	void SetSearchText(CString str) {
		m_search_text = str;
	}

protected:
	/**
	* 搜索内容
	*/
	CString m_search_text;

	/**
	* 搜索类型
	*/
	CString m_search_filter;

public:
	/**
	 * 添加标签
	 */
	void AddTag(vector<CString> arrResult) {
		for (CString strTag : arrResult) {
			SearchItem item;
			item.catalog = _T("标签");
			item.content = _T("path:") + strTag;
			m_arr_result.push_back(item);
		}
	}

	/**
	 * 添加源码
	 */
	void AddCode(vector<CString> arrResult) {
		for (CString strCode : arrResult) {
			SearchItem item;
			item.catalog = _T("源码");
			item.content = _T("path:") + strCode;
			m_arr_result.push_back(item);
		}
	}

	/**
	 * 添加图书
	 */
	void AddBook(vector<CString> arrResult) {
		for (CString strBook : arrResult) {
			SearchItem item;
			item.catalog = _T("图书");
			item.content = _T("path:") + strBook;
			m_arr_result.push_back(item);
		}
	}

	/**
	 * 清除搜索结果
	 */
	void ClearResult() {
		m_arr_result.clear();
		m_last_dwon_item = make_pair(nullptr, -1);
	}

protected:

	/**
	* 搜索结果
	*/
	struct SearchItem {
		CString catalog; /* 类别 */
		CString content; /* 内容 */
		bool is_pressed; /* 按下 */
		CRect show_rect; /* 区域 */
	};

	/**
	* 搜索结果**项列表
	*/
	vector<SearchItem> m_arr_result;

public:

	/**
	 * 准备工作
	 */
	void Prepare();

	/**
	 * 选中下一个项目
	 */
	void SelectNextItem();

	/**
	 * 选中上一个项目
	 */
	void SelectPrevItem();

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

protected:
	HICON m_hTagImage;  /* 标签 */
	HICON m_hCodeImage; /* 源码 */
	HICON m_hBookImage; /* 图书 */

protected:
	/**
	* 测试点击**项
	*/
	pair<SearchItem*, int> HitTest(CPoint pt);

	/**
	 * 上一次被按下的**项
	 */
	pair<SearchItem*,int> m_last_dwon_item;

protected:
	/**
	 * 内容使用的字体
	 */
	CFont* m_text_font;

	/**
	 * 标题使用的字体
	 */
	CFont* m_title_font;

	/**
	 * 标题的高度
	 */
	int m_title_height;
	
protected:
	void ScrollClient(int nBar, int nPos);
	int GetScrollPos(int nBar, UINT nSBCode);

protected:
	virtual BOOL OnInitDialog();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	virtual void DoDataExchange(CDataExchange* pDX);

protected:
	DECLARE_MESSAGE_MAP()
	afx_msg void OnPaint();
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
	afx_msg BOOL OnMouseWheel(UINT nFlags, short zDelta, CPoint pt);
	afx_msg void OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
};