#pragma once

#include <map>
#include <vector>
using namespace std;

/**
 * ���ؽ�����
 */
class CSearchPad : public CDialogEx
{
	DECLARE_DYNAMIC(CSearchPad)

public:
	CSearchPad(CWnd* pParent = NULL);
	virtual ~CSearchPad();

	/**
	 * ��������
	 */
	void SetSearchFilter(CString str) {
		m_search_filter = str;
	}

	/**
	 * ��������
	 */
	void SetSearchText(CString str) {
		m_search_text = str;
	}

protected:
	/**
	* ��������
	*/
	CString m_search_text;

	/**
	* ��������
	*/
	CString m_search_filter;

public:
	/**
	 * ��ӱ�ǩ
	 */
	void AddTag(vector<CString> arrResult) {
		for (CString strTag : arrResult) {
			SearchItem item;
			item.catalog = _T("��ǩ");
			item.content = _T("path:") + strTag;
			m_arr_result.push_back(item);
		}
	}

	/**
	 * ���Դ��
	 */
	void AddCode(vector<CString> arrResult) {
		for (CString strCode : arrResult) {
			SearchItem item;
			item.catalog = _T("Դ��");
			item.content = _T("path:") + strCode;
			m_arr_result.push_back(item);
		}
	}

	/**
	 * ���ͼ��
	 */
	void AddBook(vector<CString> arrResult) {
		for (CString strBook : arrResult) {
			SearchItem item;
			item.catalog = _T("ͼ��");
			item.content = _T("path:") + strBook;
			m_arr_result.push_back(item);
		}
	}

	/**
	 * ����������
	 */
	void ClearResult() {
		m_arr_result.clear();
		m_last_dwon_item = make_pair(nullptr, -1);
	}

protected:

	/**
	* �������
	*/
	struct SearchItem {
		CString catalog; /* ��� */
		CString content; /* ���� */
		bool is_pressed; /* ���� */
		CRect show_rect; /* ���� */
	};

	/**
	* �������**���б�
	*/
	vector<SearchItem> m_arr_result;

public:

	/**
	 * ׼������
	 */
	void Prepare();

	/**
	 * ѡ����һ����Ŀ
	 */
	void SelectNextItem();

	/**
	 * ѡ����һ����Ŀ
	 */
	void SelectPrevItem();

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

protected:
	HICON m_hTagImage;  /* ��ǩ */
	HICON m_hCodeImage; /* Դ�� */
	HICON m_hBookImage; /* ͼ�� */

protected:
	/**
	* ���Ե��**��
	*/
	pair<SearchItem*, int> HitTest(CPoint pt);

	/**
	 * ��һ�α����µ�**��
	 */
	pair<SearchItem*,int> m_last_dwon_item;

protected:
	/**
	 * ����ʹ�õ�����
	 */
	CFont* m_text_font;

	/**
	 * ����ʹ�õ�����
	 */
	CFont* m_title_font;

	/**
	 * ����ĸ߶�
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