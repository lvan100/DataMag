#include "stdafx.h"
#include "DataMag.h"
#include "SearchPad.h"

IMPLEMENT_DYNAMIC(CSearchPad, CDialogEx)

CSearchPad::CSearchPad(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_SEARCHPAD, pParent)
	, m_last_pressed_item(make_pair(nullptr, -1))
	, m_last_hovered_item(make_pair(nullptr, -1))
{
}

CSearchPad::~CSearchPad()
{
}

void CSearchPad::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CSearchPad, CDialogEx)
	ON_WM_PAINT()
	ON_WM_LBUTTONUP()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONDBLCLK()
	ON_WM_MOUSEWHEEL()
	ON_WM_ERASEBKGND()
	ON_WM_MOUSEMOVE()
	ON_WM_VSCROLL()
END_MESSAGE_MAP()

BOOL CSearchPad::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	LOGFONT logFont = { 0 };
	afxGlobalData.fontRegular.GetLogFont(&logFont);

	logFont.lfHeight = -15;
	m_text_font = CFont::FromHandle(CreateFontIndirect(&logFont));

	logFont.lfHeight = -17;
	m_title_font = CFont::FromHandle(CreateFontIndirect(&logFont));

	return TRUE;
}

void CSearchPad::Prepare()
{
	if (m_arr_result.size() == 0) {

		if (m_search_filter == _T("标签") || m_search_filter == _T("全部")) {

			CString strNewTag;
			strNewTag.Format(_T("新建标签 %s"), m_search_text);

			SearchItem item;
			item.catalog = _T("标签");
			item.content = _T("new:") + strNewTag;
			m_arr_result.push_back(item);
		}

		if (m_search_filter == _T("源码") || m_search_filter == _T("全部")) {

			CString strNewCode;
			strNewCode.Format(_T("新建源码 %s"), m_search_text);

			SearchItem item;
			item.catalog = _T("源码");
			item.content = _T("new:") + strNewCode;
			m_arr_result.push_back(item);
		}

		if (m_search_filter == _T("图书") || m_search_filter == _T("全部")) {

			CString strNewBook;
			strNewBook.Format(_T("新建图书 %s"), m_search_text);

			SearchItem item;
			item.catalog = _T("图书");
			item.content = _T("new:") + strNewBook;
			m_arr_result.push_back(item);
		}
	}

	ASSERT(m_arr_result.size() > 0);

	CPaintDC dc(this);

	CRect rcClient;
	GetClientRect(rcClient);

	CString lastCatalog = _T("");

	// 记录当前控件的偏移位置
	int offsetX = 4, offsetY = -2;

	CFont* pOldFont = dc.SelectObject(m_text_font);

	for (SearchItem& item : m_arr_result) {

		if (lastCatalog != item.catalog) {
			dc.SelectObject(m_title_font);
			m_title_height = dc.GetTextExtent(item.catalog).cy;
			offsetY += m_title_height + 8;
			dc.SelectObject(m_text_font);
			lastCatalog = item.catalog;
		}

		CSize size = dc.GetTextExtent(item.content);

		item.show_rect.top = offsetY;
		item.show_rect.left = offsetX;
		item.show_rect.bottom = offsetY + size.cy + 8;
		item.show_rect.right = rcClient.right - offsetX;

		offsetY += size.cy + 8;
	}

	dc.SelectObject(pOldFont);

	SCROLLINFO si;
	si.cbSize = sizeof(SCROLLINFO);
	si.fMask = SIF_PAGE | SIF_POS | SIF_RANGE;
	si.nPos = 0;
	si.nMin = 0;
	si.nMax = offsetY + 4;
	si.nPage = rcClient.Height();
	SetScrollInfo(SB_VERT, &si, TRUE);
}

BOOL CSearchPad::OnEraseBkgnd(CDC* pDC)
{
	return TRUE;
}

void CSearchPad::OnPaint()
{
	SCROLLINFO si;
	si.cbSize = sizeof(SCROLLINFO);
	si.fMask = SIF_POS;
	GetScrollInfo(SB_VERT, &si);

	CRect rcClient;
	GetClientRect(rcClient);

	CPaintDC dc(this);
	CMemDC memDC(dc, rcClient);
	CDC& actualDC = memDC.GetDC();

	// 背景透明化
	actualDC.SetBkMode(TRANSPARENT);

	actualDC.FillRect(rcClient, &afxGlobalData.brBtnFace);

	// 绘制被点击按钮的背景
	if (m_last_pressed_item.first != nullptr) {
		CRect rcHilite = m_last_pressed_item.first->show_rect;
		rcHilite.OffsetRect(0, -si.nPos);
		actualDC.FillRect(rcHilite, &afxGlobalData.brHilite);
	}

	CString lastCatalog = _T("");

	CFont* pOldFont = actualDC.SelectObject(m_text_font);

	for (SearchItem& item : m_arr_result) {

		if (lastCatalog != item.catalog) {
			actualDC.SelectObject(m_title_font);
			actualDC.TextOut(item.show_rect.left, item.show_rect.top - m_title_height - si.nPos, item.catalog);
			actualDC.SelectObject(m_text_font);
			lastCatalog = item.catalog;
		}

		CRect iconRect = item.show_rect;
		iconRect.bottom -= 2;
		iconRect.left += 2;
		iconRect.top += 2;
		iconRect.right = iconRect.left + iconRect.Height();
		iconRect.OffsetRect(0, -si.nPos);

		if (lastCatalog == _T("源码")) {
			DrawIconEx(actualDC.GetSafeHdc(), iconRect.left, iconRect.top
				, m_hCodeImage, iconRect.Width(), iconRect.Height()
				, 0, nullptr, DI_NORMAL);
		} else if (lastCatalog == _T("图书")) {
			DrawIconEx(actualDC.GetSafeHdc(), iconRect.left, iconRect.top
				, m_hBookImage, iconRect.Width(), iconRect.Height()
				, 0, nullptr, DI_NORMAL);
		} else if (lastCatalog == _T("标签")) {
			DrawIconEx(actualDC.GetSafeHdc(), iconRect.left, iconRect.top
				, m_hTagImage, iconRect.Width(), iconRect.Height()
				, 0, nullptr, DI_NORMAL);
		}

		CRect textRect(item.show_rect);
		textRect.left += textRect.Height();
		textRect.OffsetRect(0, -si.nPos);

		int colonIndex = item.content.Find(':');
		int lastSlashIndex = item.content.ReverseFind('\\');		

		int midIndex = max(lastSlashIndex, colonIndex);
		CString strText = item.content.Mid(midIndex + 1);
		actualDC.DrawText(strText, textRect, DT_LEFT | DT_VCENTER | DT_SINGLELINE);

		if (item.content.Left(colonIndex) == _T("path") && item.is_pressed) {

			CPoint point;
			GetCursorPos(&point);
			ScreenToClient(&point);

			CRect deleteRect(item.show_rect);
			deleteRect.top += 2;
			deleteRect.right -= 2;
			deleteRect.bottom -= 2;			
			deleteRect.left = deleteRect.right - deleteRect.Height();
			deleteRect.OffsetRect(0, -si.nPos);
			
			CRect copyDeleteRect(deleteRect);
			if (item.is_downing && copyDeleteRect.PtInRect(point)) {
				copyDeleteRect.OffsetRect(1, 1);
			}

			DrawIconEx(actualDC.GetSafeHdc(), copyDeleteRect.left, copyDeleteRect.top
				, m_hDeleteImg, copyDeleteRect.Width(), copyDeleteRect.Height()
				, 0, nullptr, DI_NORMAL);

			CRect editRect(deleteRect);
			editRect.left -= editRect.Height() + 2;
			editRect.right -= editRect.Height() + 2;

			CRect copyEditRect(editRect);
			if (item.is_downing && copyEditRect.PtInRect(point)) {
				copyEditRect.OffsetRect(1, 1);
			}

			DrawIconEx(actualDC.GetSafeHdc(), copyEditRect.left, copyEditRect.top
				, m_hEditImage, copyEditRect.Width(), copyEditRect.Height()
				, 0, nullptr, DI_NORMAL);

			CRect renameRect(editRect);
			renameRect.left -= renameRect.Height() + 2;
			renameRect.right -= renameRect.Height() + 2;

			CRect copyRenameRect(renameRect);
			if (item.is_downing && copyRenameRect.PtInRect(point)) {
				copyRenameRect.OffsetRect(1, 1);
			}

			DrawIconEx(actualDC.GetSafeHdc(), copyRenameRect.left, copyRenameRect.top
				, m_hRenameImg, copyRenameRect.Width(), copyRenameRect.Height()
				, 0, nullptr, DI_NORMAL);
		}
	}

	actualDC.SelectObject(pOldFont);
}

pair<CSearchPad::SearchItem*, int> CSearchPad::HitTest(CPoint pt)
{
	SCROLLINFO si;
	si.cbSize = sizeof(SCROLLINFO);
	si.fMask = SIF_POS;
	GetScrollInfo(SB_VERT, &si);

	pt.Offset(0, si.nPos);

	for (size_t i = 0; i < m_arr_result.size(); i++) {
		if (m_arr_result[i].show_rect.PtInRect(pt)) {
			return make_pair(&m_arr_result[i], i);
		}
	}
	return make_pair(nullptr, -1);
}

void CSearchPad::DoClientScroolOrMouseMove()
{
	CPoint point;
	GetCursorPos(&point);
	ScreenToClient(&point);

	if (m_last_hovered_item.first != nullptr) {
		m_last_hovered_item.first->is_hovered = false;
	}

	m_last_hovered_item = HitTest(point);
	if (m_last_hovered_item.first != nullptr) {
		m_last_hovered_item.first->is_hovered = true;
	}

	Invalidate(TRUE);
}

void CSearchPad::OnMouseMove(UINT nFlags, CPoint point)
{
	CDialogEx::OnMouseMove(nFlags, point);

	// DoClientScroolOrMouseMove();
}

void CSearchPad::OnLButtonUp(UINT nFlags, CPoint point)
{
	CDialogEx::OnLButtonUp(nFlags, point);

	if (m_last_pressed_item.first != nullptr) {
		m_last_pressed_item.first->is_downing = false;

		SCROLLINFO si;
		si.cbSize = sizeof(SCROLLINFO);
		si.fMask = SIF_POS;
		GetScrollInfo(SB_VERT, &si);

		CRect deleteRect(m_last_pressed_item.first->show_rect);

		deleteRect.top += 2;
		deleteRect.right -= 2;
		deleteRect.bottom -= 2;
		deleteRect.left = deleteRect.right - deleteRect.Height();
		deleteRect.OffsetRect(0, -si.nPos);

		if (deleteRect.PtInRect(point)) {
			// DO DELETE
		}

		CRect editRect(deleteRect);
		editRect.left -= editRect.Height() + 2;
		editRect.right -= editRect.Height() + 2;

		if (editRect.PtInRect(point)) {
			// DO EDIT
		}

		CRect renameRect(editRect);
		renameRect.left -= renameRect.Height() + 2;
		renameRect.right -= renameRect.Height() + 2;

		if (renameRect.PtInRect(point)) {
			// DO RENAME
		}
	}

	// 将焦点立即传送给父窗口
	GetParent()->SetFocus();

	Invalidate(TRUE);
}

void CSearchPad::OnLButtonDown(UINT nFlags, CPoint point)
{
	CDialogEx::OnLButtonDown(nFlags, point);

	if (m_last_pressed_item.first != nullptr) {
		m_last_pressed_item.first->is_pressed = false;
	}

	m_last_pressed_item = HitTest(point);
	if (m_last_pressed_item.first != nullptr) {
		m_last_pressed_item.first->is_pressed = true;
		m_last_pressed_item.first->is_downing = true;
	}

	Invalidate(TRUE);
}

void CSearchPad::OnLButtonDblClk(UINT nFlags, CPoint point)
{
	CDialogEx::OnLButtonDblClk(nFlags, point);

	if (m_last_pressed_item.first != nullptr) {
		CString strContent = m_last_pressed_item.first->content;

		int colonIndex = strContent.Find(':');
		CString strPathOrNew = strContent.Left(colonIndex);

		if (strPathOrNew == _T("path")) {
			CString strPath = strContent.Mid(colonIndex + 1);
			ShellExecute(NULL, _T("open"), strPath, NULL, strPath, SW_SHOWMAXIMIZED);
		} else if (strPathOrNew == _T("new")) {

		} else {
			// DO NOTHING
		}
	}
}

BOOL CSearchPad::PreTranslateMessage(MSG* pMsg)
{
	if (pMsg->message == WM_KEYDOWN) {
		if (pMsg->wParam == VK_ESCAPE) {
			ShowWindow(SW_HIDE);
			return TRUE;
		}
	}

	return CDialogEx::PreTranslateMessage(pMsg);
}

void CSearchPad::SelectNextItem()
{
	ASSERT(m_arr_result.size() > 0);

	if (m_last_pressed_item.first == nullptr) {
		m_last_pressed_item = make_pair(&m_arr_result[0], 0);
	} else {
		int targetIndex(m_last_pressed_item.second + 1);
		if (targetIndex < (int) m_arr_result.size()) {
			m_last_pressed_item = make_pair(&m_arr_result[targetIndex], targetIndex);
		}
	}

	ASSERT(m_last_pressed_item.first != nullptr);

	if (m_last_pressed_item.first != nullptr) {
		m_last_pressed_item.first->is_pressed = true;
		Invalidate(TRUE);
	}
}

void CSearchPad::SelectPrevItem()
{
	ASSERT(m_arr_result.size() > 0);

	if (m_last_pressed_item.first != nullptr) {
		int targetIndex(m_last_pressed_item.second - 1);
		if (targetIndex >= 0) {
			m_last_pressed_item = make_pair(&m_arr_result[targetIndex], targetIndex);
		}
	}

	if (m_last_pressed_item.first != nullptr) {
		m_last_pressed_item.first->is_pressed = true;
		Invalidate(TRUE);
	}
}

BOOL CSearchPad::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt)
{
	SCROLLINFO si;
	si.cbSize = sizeof(SCROLLINFO);
	si.fMask = SIF_POS;
	GetScrollInfo(SB_VERT, &si);

	if (si.nPos != -1) {
		SetScrollPos(SB_VERT, si.nPos - zDelta / 10, TRUE);
		ScrollClient(SB_VERT, si.nPos - zDelta / 10);
	}

	// DoClientScroolOrMouseMove();

	return CDialogEx::OnMouseWheel(nFlags, zDelta, pt);
}

void CSearchPad::OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	const int scrollPos = GetScrollPos(SB_VERT, nSBCode);
	if (scrollPos != -1) {
		SetScrollPos(SB_VERT, scrollPos, TRUE);
		ScrollClient(SB_VERT, scrollPos);
		Invalidate();
	}

	CDialogEx::OnVScroll(nSBCode, nPos, pScrollBar);
}

void CSearchPad::ScrollClient(int nBar, int nPos)
{
	static int s_prevx = 1;
	static int s_prevy = 1;

	int cx = 0;
	int cy = 0;

	int& delta = (nBar == SB_HORZ ? cx : cy);
	int& prev = (nBar == SB_HORZ ? s_prevx : s_prevy);

	delta = prev - nPos;
	prev = nPos;

	if (cx || cy) {
		ScrollWindow(cx, cy, NULL, NULL);
	}
}

int CSearchPad::GetScrollPos(int nBar, UINT nSBCode)
{
	SCROLLINFO si;
	si.cbSize = sizeof(SCROLLINFO);
	si.fMask = SIF_PAGE | SIF_POS | SIF_RANGE | SIF_TRACKPOS;

	GetScrollInfo(nBar, &si);

	const int minPos = si.nMin;
	const int maxPos = si.nMax - (si.nPage - 1);

	int result = -1;

	switch (nSBCode)
	{
	case SB_LINEUP /*SB_LINELEFT*/:
		result = max(si.nPos - 1, minPos);
		break;
	case SB_LINEDOWN /*SB_LINERIGHT*/:
		result = min(si.nPos + 1, maxPos);
		break;
	case SB_PAGEUP /*SB_PAGELEFT*/:
		result = max(si.nPos - (int)si.nPage, minPos);
		break;
	case SB_PAGEDOWN /*SB_PAGERIGHT*/:
		result = min(si.nPos + (int)si.nPage, maxPos);
		break;
	case SB_THUMBPOSITION:
		// do nothing
		break;
	case SB_THUMBTRACK:
		result = si.nTrackPos;
		break;
	case SB_TOP /*SB_LEFT*/:
		result = minPos;
		break;
	case SB_BOTTOM /*SB_RIGHT*/:
		result = maxPos;
		break;
	case SB_ENDSCROLL:
		// do nothing
		break;
	}

	return result;
}