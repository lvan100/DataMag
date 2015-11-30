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
	 * ������������
	 */
	void SetSearchFilter(CString str) {
		m_search_filter = str;
	}

	/**
	 * ������������
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
		m_last_pressed_item = NullItem;
		m_last_hovered_item = NullItem;
	}

protected:

	/**
	* �������
	*/
	struct SearchItem {

		CString catalog; /* ��� */
		CString content; /* ���� */

		bool is_focused; /* ���� */
		bool is_hovered; /* �ȵ� */
		bool is_downing; /* ״̬ */

		CRect show_rect; /* ���� */

		SearchItem()
			: is_focused(false)
			, is_hovered(false)
			, is_downing(false)
		{}
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

protected:
	/**
	 * ��ȡ��ǰ�Ĺ���λ��
	 */
	int GetCurrentScroolPos();

protected:
	/**
	 * �ؼ����ݺͱ߿�ľ���
	 */
	const int m_pedding = 4;

protected:
	/**
	* ���Ե��**��
	*/
	pair<SearchItem*, int> HitTest(CPoint pt);

	/**
	 * ��һ�α����µ�**��
	 */
	pair<SearchItem*,int> m_last_pressed_item;

	/**
	* ��һ�α��ȵ��**��
	*/
	pair<SearchItem*, int> m_last_hovered_item;

	/**
	 * �յ�**��
	 */
	static const pair<SearchItem*, int> NullItem;

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
	/**
	 * �������������
	 */
	enum HitAction {
		Delete, Info, Rename, Click,
	};

	/**
	 * ���Ե������
	 */
	HitAction HitTestAction(CPoint point);

	/**
	 * �����ͻ���
	 */
	void ScrollClient(int nBar, int nPos);

	/**
	 * ��ȡ������λ��|
	 */
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