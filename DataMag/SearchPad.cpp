#include "stdafx.h"
#include "DataMag.h"
#include "SearchPad.h"

IMPLEMENT_DYNAMIC(CSearchPad, CDialogEx)

const pair<CSearchPad::SearchItem*, int> CSearchPad::NullItem(nullptr, -1);

CSearchPad::CSearchPad(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_SEARCHPAD, pParent)
	, m_last_pressed_item(NullItem)
	, m_last_hovered_item(NullItem)
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
	ON_WM_MOUSEWHEEL()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONDBLCLK()
	ON_WM_ERASEBKGND()
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

BOOL CSearchPad::OnEraseBkgnd(CDC* pDC)
{
	return TRUE;
}

void CSearchPad::Prepare()
{
	if (m_arr_result.size() == 0) {

		auto NewResultItem = [&](CString strCatalog) {

			CString strNewItem;
			strNewItem.Format(_T("新建%s %s"), strCatalog, m_search_text);

			SearchItem item;
			item.catalog = strCatalog;
			item.content = _T("new:") + strNewItem;
			m_arr_result.push_back(item);
		};

		if (m_search_filter == _T("标签") || m_search_filter == _T("全部")) {
			NewResultItem(_T("标签"));
		}

		if (m_search_filter == _T("源码") || m_search_filter == _T("全部")) {
			NewResultItem(_T("源码"));
		}

		if (m_search_filter == _T("图书") || m_search_filter == _T("全部")) {
			NewResultItem(_T("图书"));
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
			offsetY += (m_title_height + m_pedding * 2);
			dc.SelectObject(m_text_font);
			lastCatalog = item.catalog;
		}

		CSize size = dc.GetTextExtent(item.content);

		item.show_rect.top = offsetY;
		item.show_rect.left = offsetX;
		item.show_rect.right = rcClient.right - offsetX;
		item.show_rect.bottom = offsetY + size.cy + m_pedding * 2;

		offsetY += item.show_rect.Height();
	}

	dc.SelectObject(pOldFont);

	SCROLLINFO si;
	si.cbSize = sizeof(SCROLLINFO);
	si.fMask = SIF_PAGE | SIF_POS | SIF_RANGE;
	si.nPos = si.nMin = 0;
	si.nMax = offsetY + 4;
	si.nPage = rcClient.Height();
	SetScrollInfo(SB_VERT, &si, TRUE);
}

HICON CSearchPad::GetCatalogImage(CString catalog)
{
	if (catalog == _T("源码")) {
		return m_hCodeImage;
	} else if (catalog == _T("图书")) {
		return m_hBookImage;
	} else if (catalog == _T("标签")) {
		return m_hTagImage;
	} else {
		return nullptr;
	}
}

int CSearchPad::GetCurrentScroolPos()
{
	SCROLLINFO si;
	si.cbSize = sizeof(SCROLLINFO);
	si.fMask = SIF_POS;
	GetScrollInfo(SB_VERT, &si);
	return si.nPos;
}

void CSearchPad::OnPaint()
{
	CRect rcClient;
	GetClientRect(rcClient);

	CPaintDC dc(this);

	// 使用双层缓冲绘图技术
	CMemDC memDC(dc, rcClient);
	CDC& actualDC = memDC.GetDC();

	// 将文字背景透明化
	actualDC.SetBkMode(TRANSPARENT);

	// 获取当前的窗口滚动位置
	int nScrollPos = GetCurrentScroolPos();

	actualDC.FillRect(rcClient, &afxGlobalData.brBtnFace);

	// 绘制获得焦点状态的按钮的背景
	if (m_last_pressed_item.first != nullptr) {
		CRect rcHilite = m_last_pressed_item.first->show_rect;
		rcHilite.OffsetRect(0, -nScrollPos);
		actualDC.FillRect(rcHilite, &afxGlobalData.brHilite);
	}

	CString lastCatalog = _T("");

	CFont* pOldFont = actualDC.SelectObject(m_text_font);

	for (SearchItem& item : m_arr_result) {

		if (lastCatalog != item.catalog) {

			CRect catalogRect(item.show_rect);
			catalogRect.OffsetRect(0, -catalogRect.Height());

			actualDC.SelectObject(m_title_font);
			actualDC.DrawText(item.catalog, catalogRect, DT_LEFT | DT_VCENTER | DT_SINGLELINE);
			actualDC.SelectObject(m_text_font);

			lastCatalog = item.catalog;
		}

		CRect iconRect = item.show_rect;
		iconRect.DeflateRect(2, 2, 2, 2);
		iconRect.right = iconRect.left + iconRect.Height();
		iconRect.OffsetRect(0, -nScrollPos);

		DrawIconEx(actualDC.GetSafeHdc(), iconRect.left, iconRect.top
			, GetCatalogImage(lastCatalog), iconRect.Width()
			, iconRect.Height(), 0, nullptr, DI_NORMAL);

		CRect textRect(item.show_rect);
		textRect.left += textRect.Height();
		textRect.OffsetRect(0, -nScrollPos);

		int colonIndex = item.content.Find(':');
		int lastSlashIndex = item.content.ReverseFind('\\');		

		int midIndex = max(lastSlashIndex, colonIndex);
		CString strText = item.content.Mid(midIndex + 1);
		actualDC.DrawText(strText, textRect, DT_LEFT | DT_VCENTER | DT_SINGLELINE);

		if (item.content.Left(colonIndex) == _T("path") && item.is_focused) {

			CPoint point;
			GetCursorPos(&point);
			ScreenToClient(&point);

			CRect deleteRect(item.show_rect);
			deleteRect.DeflateRect(2, 2, 2, 2);		
			deleteRect.left = deleteRect.right - deleteRect.Height();
			deleteRect.OffsetRect(0, -nScrollPos);
			
			CRect copyDeleteRect(deleteRect);
			if (item.is_downing && copyDeleteRect.PtInRect(point)) {
				copyDeleteRect.OffsetRect(1, 1);
			}

			DrawIconEx(actualDC.GetSafeHdc(), copyDeleteRect.left, copyDeleteRect.top
				, m_hDeleteImg, copyDeleteRect.Width(), copyDeleteRect.Height()
				, 0, nullptr, DI_NORMAL);

			CRect infoRect(deleteRect);
			infoRect.OffsetRect(-(deleteRect.Height() + 2), 0);

			CRect copyInfoRect(infoRect);
			if (item.is_downing && copyInfoRect.PtInRect(point)) {
				copyInfoRect.OffsetRect(1, 1);
			}

			DrawIconEx(actualDC.GetSafeHdc(), copyInfoRect.left, copyInfoRect.top
				, m_hInfoImage, copyInfoRect.Width(), copyInfoRect.Height()
				, 0, nullptr, DI_NORMAL);

			CRect renameRect(infoRect);
			renameRect.OffsetRect(-(infoRect.Height() + 2), 0);

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
	pt.Offset(0, GetCurrentScroolPos());

	for (size_t i = 0; i < m_arr_result.size(); i++) {
		if (m_arr_result[i].show_rect.PtInRect(pt)) {
			return make_pair(&m_arr_result[i], i);
		}
	}

	return NullItem;
}

CSearchPad::HitAction CSearchPad::HitTestAction(CPoint point)
{
	CRect deleteRect(m_last_pressed_item.first->show_rect);
	deleteRect.DeflateRect(2, 2, 2, 2);

	deleteRect.left = deleteRect.right - deleteRect.Height();
	deleteRect.OffsetRect(0, -GetCurrentScroolPos());

	if (deleteRect.PtInRect(point)) {
		return HitAction::Delete;
	}

	CRect infoRect(deleteRect);
	infoRect.OffsetRect(-(deleteRect.Height() + 2), 0);

	if (infoRect.PtInRect(point)) {
		return HitAction::Info;
	}

	CRect renameRect(infoRect);
	renameRect.OffsetRect(-(infoRect.Height() + 2), 0);

	if (renameRect.PtInRect(point)) {
		return HitAction::Rename;
	}

	return HitAction::Click;
}

/**
 * 递归删除目录下所有文件
 */
STATIC void DeleteDirectory(CString strDir)
{
	CFileFind fileFind;

	BOOL IsFinded = fileFind.FindFile(strDir + _T("\\*.*"));
	IsFinded = fileFind.FindNextFile();	// .
	IsFinded = fileFind.FindNextFile();	// ..

	while (IsFinded) {
		IsFinded = fileFind.FindNextFile();
		CString strFile = fileFind.GetFilePath();
		if (PathIsDirectory(strFile)) {
			DeleteDirectory(strFile);
		} else {
			DeleteFile(strFile);
		}
	}

	fileFind.Close();

	RemoveDirectory(strDir);
}

void CSearchPad::OnLButtonUp(UINT nFlags, CPoint point)
{
	CDialogEx::OnLButtonUp(nFlags, point);

	if (m_last_pressed_item.first != nullptr) {
		m_last_pressed_item.first->is_downing = false;

		CString strCatalog = m_last_pressed_item.first->catalog;
		CString strContent = m_last_pressed_item.first->content;

		int colonIndex = strContent.Find(':');
		int lastSlashIndex = strContent.ReverseFind('\\');
		CString strPath = strContent.Mid(colonIndex + 1);

		switch (HitTestAction(point))
		{
		case HitAction::Delete: {
			DeleteDirectory(strPath);
		}
			break;
		case HitAction::Rename: {
			// DO RENAME
		}
			break;
		case HitAction::Info: {
			if (m_show_detail_event) {
				m_show_detail_event(strCatalog, strPath);
			}
		}
			break;
		default:
			break;
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
		m_last_pressed_item.first->is_focused = false;
	}

	m_last_pressed_item = HitTest(point);
	if (m_last_pressed_item.first != nullptr) {
		m_last_pressed_item.first->is_focused = true;
		m_last_pressed_item.first->is_downing = true;
	}

	Invalidate(TRUE);
}

/**
 * 创建标签
 */
STATIC CString CreateNewTag(CString strTag)
{
	CString strFolder = theApp.GetTagDir();
	strFolder += _T("\\") + strTag;

	if (CreateDirectory(strFolder, nullptr)) {
		CreateDirectory(strFolder + _T("\\关联"), nullptr);
		CreateDirectory(strFolder + _T("\\关联\\标签"), nullptr);
		CreateDirectory(strFolder + _T("\\关联\\项目"), nullptr);
		CreateDirectory(strFolder + _T("\\关联\\图书"), nullptr);

	} else {
		CString strContent;
		strContent.Format(_T("创建标签\"%s\"失败！"), strFolder);
		MessageBox(NULL, strContent, _T("创建失败"), MB_ICONERROR);
	}

	return strFolder;
}

/**
 * 创建源码
 */
STATIC CString  CreateNewCode(CString strCode)
{
	CString strFolder = theApp.GetCodeDir();
	strFolder += _T("\\") + strCode;

	if (CreateDirectory(strFolder, nullptr)) {
		CString strFile = strFolder + _T("\\描述.txt");
		CloseHandle(CreateFile(strFile, 0, 0, nullptr, CREATE_ALWAYS, 0, nullptr));

		CreateDirectory(strFolder + _T("\\源码"), nullptr);

		CreateDirectory(strFolder + _T("\\资料"), nullptr);
		CreateDirectory(strFolder + _T("\\资料\\官网"), nullptr);
		CreateDirectory(strFolder + _T("\\资料\\资料"), nullptr);
	
		CreateDirectory(strFolder + _T("\\关联"), nullptr);
		CreateDirectory(strFolder + _T("\\关联\\标签"), nullptr);
		CreateDirectory(strFolder + _T("\\关联\\项目"), nullptr);
		CreateDirectory(strFolder + _T("\\关联\\图书"), nullptr);
		
		ShellExecute(NULL, _T("open"), strFolder, NULL, strFolder, SW_SHOWMAXIMIZED);
		
		// 添加到最近访问列表
		theApp.SetRecentFile(strFolder);

	} else {
		CString strContent;
		strContent.Format(_T("创建源码\"%s\"失败！"), strFolder);
		MessageBox(NULL, strContent, _T("创建失败"), MB_ICONERROR);
	}

	return strFolder;
}

/**
 * 创建图书
 */
STATIC CString CreateNewBook(CString strBook)
{
	CString strFolder = theApp.GetBookDir();
	strFolder += _T("\\") + strBook;

	if (CreateDirectory(strFolder, nullptr)) {
		CString strFile = strFolder + _T("\\描述.txt");
		CloseHandle(CreateFile(strFile, 0, 0, nullptr, CREATE_ALWAYS, 0, nullptr));

		CreateDirectory(strFolder + _T("\\关联"), nullptr);
		CreateDirectory(strFolder + _T("\\关联\\标签"), nullptr);
		CreateDirectory(strFolder + _T("\\关联\\项目"), nullptr);
		CreateDirectory(strFolder + _T("\\关联\\图书"), nullptr);

		ShellExecute(NULL, _T("open"), strFolder, NULL, strFolder, SW_SHOWMAXIMIZED);

		// 添加到最近访问列表
		theApp.SetRecentFile(strFolder);

	} else {
		CString strContent;
		strContent.Format(_T("创建图书\"%s\"失败！"), strFolder);
		MessageBox(NULL, strContent, _T("创建失败"), MB_ICONERROR);
	}

	return strFolder;
}

void CSearchPad::OnLButtonDblClk(UINT nFlags, CPoint point)
{
	CDialogEx::OnLButtonDblClk(nFlags, point);

	if (m_last_pressed_item.first != nullptr) {
		CString strContent = m_last_pressed_item.first->content;

		if (HitTestAction(point) == HitAction::Click) {

			int colonIndex = strContent.Find(':');
			CString strPathOrNew = strContent.Left(colonIndex);

			if (strPathOrNew == _T("path")) {
				CString strPath = strContent.Mid(colonIndex + 1);
				ShellExecute(NULL, _T("open"), strPath, NULL, strPath, SW_SHOWMAXIMIZED);

			} else if (strPathOrNew == _T("new")) {
				CString strActualPath;
				CString strCatalog = m_last_pressed_item.first->catalog;

				if (strCatalog == _T("标签")) {
					strActualPath = CreateNewTag(m_search_text);
				} else if (strCatalog == _T("源码")) {
					strActualPath = CreateNewCode(m_search_text);
				} else if (strCatalog == _T("图书")) {
					strActualPath = CreateNewBook(m_search_text);
				} else {
					// DO NOTHING!
				}

				if (m_show_detail_event) {
					m_show_detail_event(strCatalog, strActualPath);
				}
			} else {
				// DO NOTHING
			}
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

	pair<SearchItem*, int> resultItem(NullItem);

	if (m_last_pressed_item.first == nullptr) {
		resultItem = make_pair(&m_arr_result[0], 0);
	} else {
		int targetIndex(m_last_pressed_item.second + 1);
		if (targetIndex < (int) m_arr_result.size()) {
			resultItem = make_pair(&m_arr_result[targetIndex], targetIndex);
		}
	}

	ASSERT(resultItem.first != nullptr);

	if (resultItem.first != nullptr) {
		resultItem.first->is_focused = true;
	}

	if (resultItem.second != m_last_pressed_item.second) {
		if (m_last_pressed_item.first != nullptr) {
			m_last_pressed_item.first->is_focused = false;
		}
		m_last_pressed_item = resultItem;
		Invalidate(TRUE);
	}
}

void CSearchPad::SelectPrevItem()
{
	ASSERT(m_arr_result.size() > 0);

	pair<SearchItem*, int> resultItem(NullItem);

	if (m_last_pressed_item.first != nullptr) {
		int targetIndex(m_last_pressed_item.second - 1);
		if (targetIndex >= 0) {
			resultItem = make_pair(&m_arr_result[targetIndex], targetIndex);
		}
	}

	if (resultItem.first != nullptr) {
		resultItem.first->is_focused = true;
	}

	if (resultItem.second != m_last_pressed_item.second) {
		if (m_last_pressed_item.first != nullptr) {
			m_last_pressed_item.first->is_focused = false;
		}
		m_last_pressed_item = resultItem;
		Invalidate(TRUE);
	}
}

BOOL CSearchPad::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt)
{
	SCROLLINFO si;
	si.cbSize = sizeof(SCROLLINFO);
	si.fMask = SIF_POS | SIF_RANGE;
	GetScrollInfo(SB_VERT, &si);

	int nCurrentScrollPos = GetCurrentScroolPos();
	int nTargetScrollPos = nCurrentScrollPos - zDelta / 10;
	nTargetScrollPos = min(max(nTargetScrollPos, si.nMin), si.nMax);

	if (nTargetScrollPos != nCurrentScrollPos) {
		SetScrollPos(SB_VERT, nTargetScrollPos, TRUE);
		ScrollClient(SB_VERT, nTargetScrollPos);
		Invalidate(TRUE);
	}

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