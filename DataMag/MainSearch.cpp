#include "stdafx.h"
#include "DataMag.h"
#include "BookTab.h"
#include "TagTab.h"
#include "CodeTab.h"
#include "MainSearch.h"
#include "DDXControl.h"

/**
 * 全局的主搜索对话框对象
 */
CMainSearch* theMainSearch = nullptr;

IMPLEMENT_DYNAMIC(CMainSearch, CAppWnd)

CMainSearch::CMainSearch(CWnd* pParent /*=nullptr*/)
	: CAppWnd(CMainSearch::IDD, pParent)
	, m_recent_list(&theShellManager)
{
	if (theMainSearch == nullptr) {
		theMainSearch = this;
	} else {
		ASSERT(FALSE);
	}

	HICON hSearchIcon = theApp.GetSearchIcon();
	m_tag_search.SetSearchIcon(hSearchIcon);
	m_book_search.SetSearchIcon(hSearchIcon);
	m_project_search.SetSearchIcon(hSearchIcon);

	m_recent_list.SetListEvent(&m_recent_list_event);

	RecentListChangeListener listener;
	listener = bind(&CMainSearch::OnRecentListChange, this);
	theApp.AddRecentListChangeListener(listener);
}

CMainSearch::~CMainSearch()
{
}

void CMainSearch::DoDataExchange(CDataExchange* pDX)
{
	CAppWnd::DoDataExchange(pDX);
	MFC_DDX_Control(pDX, IDC_ADD_TAG, m_add_tag);
	MFC_DDX_Control(pDX, IDC_ADD_BOOK, m_add_book);
	DDX_Control(pDX, IDC_RECENT_LIST, m_recent_list);
	MFC_DDX_Control(pDX, IDC_ADD_CODE, m_add_project);
	MFC_DDX_Control(pDX, IDC_TAG_SEARCH, m_tag_search);
	MFC_DDX_Control(pDX, IDC_BOOK_SEARCH, m_book_search);
	MFC_DDX_Control(pDX, IDC_RECENT_GROUP, m_recent_group);
	MFC_DDX_Control(pDX, IDC_CODE_SEARCH, m_project_search);
}

BEGIN_MESSAGE_MAP(CMainSearch, CAppWnd)
	ON_BN_CLICKED(IDC_ADD_CODE, &CMainSearch::OnBnClickedAddProject)
	ON_BN_CLICKED(IDC_ADD_BOOK, &CMainSearch::OnBnClickedAddBook)
	ON_BN_CLICKED(IDC_ADD_TAG, &CMainSearch::OnBnClickedAddTag)
	ON_WM_MOVE()
END_MESSAGE_MAP()

void CMainSearch::RecentListEvent::OnDoubleClick()
{
	auto pThis = ((CMainSearch*)((BYTE*)this - offsetof(CMainSearch, m_recent_list_event)));
	pThis->m_recent_list.DoDefaultDClick(pThis->m_recent_list.GetCurSel());
}

BOOL CMainSearch::OnInitDialog()
{
	CAppWnd::OnInitDialog();

	HICON hTagIcon = (HICON)LoadImage(AfxGetInstanceHandle()
		, MAKEINTRESOURCE(IDI_TAG)
		, IMAGE_ICON, 0, 0, 0);
	m_add_tag.SetImage(hTagIcon, FALSE);

	HICON hProjectIcon = (HICON)LoadImage(AfxGetInstanceHandle()
		, MAKEINTRESOURCE(IDI_CODE)
		, IMAGE_ICON, 0, 0, 0);
	m_add_project.SetImage(hProjectIcon, FALSE);

	HICON hBookIcon = (HICON)LoadImage(AfxGetInstanceHandle()
		, MAKEINTRESOURCE(IDI_BOOK)
		, IMAGE_ICON, 0, 0, 0);
	m_add_book.SetImage(hBookIcon, FALSE);

	m_recent_list.SetBookImage(hBookIcon);
	m_recent_list.SetTagImage(hTagIcon);
	m_recent_list.SetCodeImage(hProjectIcon);
	
	m_tag_search.SetHintText(_T("搜索标签"));
	m_book_search.SetHintText(_T("搜索图书"));
	m_project_search.SetHintText(_T("搜索项目"));

	[&](){
		LOGFONT logFont = { 0 };
		afxGlobalData.fontRegular.GetLogFont(&logFont);

		logFont.lfHeight = -16;

		HFONT hFont = CreateFontIndirect(&logFont);
		m_recent_group.SetFont(CFont::FromHandle(hFont));
	}();

	auto& list = theApp.GetRecentFileList();
	for (size_t i = 0; i < list.size(); i++) {
		m_recent_list.AddString(list.at(i));
	}

	return FALSE;
}

BOOL CMainSearch::PreTranslateMessage(MSG* pMsg)
{
	if (pMsg->message == WM_KEYDOWN)
	{
		CWnd* pFocus = GetFocus();

		switch((UINT)pMsg->wParam)
		{
		case VK_RETURN:
			{
				CString strSearchText;
				if (pFocus == &m_tag_search) {
					m_tag_search.GetWindowText(strSearchText);

					MoveToHideWindow(TRUE);
					CTagTab(_T("search:") + strSearchText).DoModal();
					MoveToHideWindow(FALSE);

					m_tag_search.SetWindowText(_T(""));
					m_tag_search.SetFocus();

				} else if (pFocus == &m_project_search) {
					m_project_search.GetWindowText(strSearchText);

					MoveToHideWindow(TRUE);
					CCodeTab(_T("search:") + strSearchText).DoModal();
					MoveToHideWindow(FALSE);

					m_project_search.SetWindowText(_T(""));
					m_project_search.SetFocus();

				} else if (pFocus == &m_book_search) {
					m_book_search.GetWindowText(strSearchText);

					MoveToHideWindow(TRUE);
					CBookTab(_T("search:") + strSearchText).DoModal();
					MoveToHideWindow(FALSE);

					m_book_search.SetWindowText(_T(""));
					m_book_search.SetFocus();
				}
			}
			return TRUE;
		}
	}

	return CAppWnd::PreTranslateMessage(pMsg);
}

void CMainSearch::OnBnClickedAddTag()
{
	MoveToHideWindow(TRUE);
	CTagTab(_T("add")).DoModal();
	MoveToHideWindow(FALSE);

	m_project_search.SetFocus();
}

void CMainSearch::OnBnClickedAddProject()
{
	MoveToHideWindow(TRUE);
	CCodeTab(_T("add")).DoModal();
	MoveToHideWindow(FALSE);

	m_project_search.SetFocus();
}

void CMainSearch::OnBnClickedAddBook()
{
	MoveToHideWindow(TRUE);
	CBookTab(_T("add")).DoModal();
	MoveToHideWindow(FALSE);

	m_project_search.SetFocus();
}

void CMainSearch::MoveToHideWindow(BOOL bHide)
{
	CRect rcWnd;
	GetWindowRect(rcWnd);
	
	int top = rcWnd.top;
	int bottom = rcWnd.bottom;

	if (bHide) {
		// 如果可见将移动到不可见区域
		if (top >= 0 && bottom >= 0) {
			rcWnd.bottom = -top;
			rcWnd.top = -bottom;
		}

	} else {
		// 如果不可见将移动到可见区域
		if (top < 0 && bottom < 0) {
			rcWnd.bottom = -top;
			rcWnd.top = -bottom;
		}
	}

	MoveWindow(rcWnd);
}

void CMainSearch::OnMove(int x, int y)
{
	CAppWnd::OnMove(x, y);

	if (x >= 0 && y >= 0) {
		GetWindowRect(m_rect_if_visiable);
	}
}

void CMainSearch::OnRecentListChange()
{
	m_recent_list.ResetContent();

	auto& list = theApp.GetRecentFileList();
	for (size_t i = 0; i < list.size(); i++) {
		m_recent_list.AddString(list.at(i));
	}
}