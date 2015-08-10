#include "stdafx.h"
#include "DataMag.h"
#include "BookTab.h"
#include "LabelTab.h"
#include "ProjectTab.h"
#include "MainSearch.h"
#include "DDXControl.h"

/**
 * 全局的主搜索对话框对象
 */
CMainSearch* theMainSearch = NULL;

IMPLEMENT_DYNAMIC(CMainSearch, CDialogEx)

CMainSearch::CMainSearch(CWnd* pParent /*=NULL*/)
	: CDialogEx(CMainSearch::IDD, pParent)
{
	if (theMainSearch == NULL) {
		theMainSearch = this;
	} else {
		ASSERT(FALSE);
	}

	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);

	HICON hSearchIcon = (HICON)LoadImage(AfxGetInstanceHandle()
		, MAKEINTRESOURCE(IDI_SEARCH)
		, IMAGE_ICON, 0, 0, 0);
	m_book_search.SetSearchIcon(hSearchIcon);
	m_label_search.SetSearchIcon(hSearchIcon);
	m_project_search.SetSearchIcon(hSearchIcon);

	RecentListChangeListener listener;
	listener = bind(&CMainSearch::OnRecentListChange, this);
	theSetting.AddRecentListChangeListener(listener);
}

CMainSearch::~CMainSearch()
{
}

void CMainSearch::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	MFC_DDX_Control(pDX, IDC_MY_BLOG, m_blog);
	MFC_DDX_Control(pDX, IDC_ADD_BOOK, m_add_book);
	DDX_Control(pDX, IDC_RECENT_LIST, m_recent_list);
	MFC_DDX_Control(pDX, IDC_ADD_LABEL, m_add_label);
	MFC_DDX_Control(pDX, IDC_BOOK_SEARCH, m_book_search);
	MFC_DDX_Control(pDX, IDC_ADD_PROJECT, m_add_project);
	DDX_Control(pDX, IDC_FRENQUENT_LIST, m_frequent_list);
	MFC_DDX_Control(pDX, IDC_LABEL_SEARCH, m_label_search);
	MFC_DDX_Control(pDX, IDC_PROJECT_SEARCH, m_project_search);
}

BEGIN_MESSAGE_MAP(CMainSearch, CDialogEx)
	ON_BN_CLICKED(IDC_ADD_PROJECT, &CMainSearch::OnBnClickedAddProject)
	ON_BN_CLICKED(IDC_ADD_LABEL, &CMainSearch::OnBnClickedAddLabel)
	ON_BN_CLICKED(IDC_ADD_BOOK, &CMainSearch::OnBnClickedAddBook)
	ON_WM_ACTIVATE()
	ON_WM_MOVE()
END_MESSAGE_MAP()

BOOL CMainSearch::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	SetIcon(m_hIcon, TRUE);
	SetIcon(m_hIcon, FALSE);

	// 将焦点设置到项目搜索上
	m_project_search.SetFocus();

	HICON hLabelIcon = (HICON)LoadImage(AfxGetInstanceHandle()
		, MAKEINTRESOURCE(IDI_LABEL)
		, IMAGE_ICON, 0, 0, 0);
	m_add_label.SetImage(hLabelIcon, FALSE);

	HICON hProjectIcon = (HICON)LoadImage(AfxGetInstanceHandle()
		, MAKEINTRESOURCE(IDI_CODE)
		, IMAGE_ICON, 0, 0, 0);
	m_add_project.SetImage(hProjectIcon, FALSE);

	HICON hBookIcon = (HICON)LoadImage(AfxGetInstanceHandle()
		, MAKEINTRESOURCE(IDI_BOOK)
		, IMAGE_ICON, 0, 0, 0);
	m_add_book.SetImage(hBookIcon, FALSE);

	m_recent_list.SetBookImage(hBookIcon);
	m_recent_list.SetLabelImage(hLabelIcon);
	m_recent_list.SetCodeImage(hProjectIcon);

	m_frequent_list.SetBookImage(hBookIcon);
	m_frequent_list.SetLabelImage(hLabelIcon);
	m_frequent_list.SetCodeImage(hProjectIcon);

	[&](){
		LOGFONT logFont = { 0 };
		afxGlobalData.fontBold.GetLogFont(&logFont);

		logFont.lfHeight = -20;

		HFONT hFont = CreateFontIndirect(&logFont);
		m_blog.SetFont(CFont::FromHandle(hFont));
	}();
	
	m_book_search.SetHintText(_T("搜索图书"));
	m_label_search.SetHintText(_T("搜索标签"));
	m_project_search.SetHintText(_T("搜索项目"));

	auto& list = theSetting.GetRecentFileList();
	for (int i = 0; i < list.size(); i++) {
		m_recent_list.AddString(list.at(i));
	}

	m_frequent_list.AddString(_T("C:\\Users\\欢\\Desktop\\资料管理\\源码\\23"));
	m_frequent_list.AddString(_T("C:\\Users\\欢\\Desktop\\资料管理\\图书\\23"));
	m_frequent_list.AddString(_T("C:\\Users\\欢\\Desktop\\资料管理\\标签\\23"));
	m_frequent_list.AddString(_T("C:\\Users\\欢\\Desktop\\资料管理\\源码\\23"));
	m_frequent_list.AddString(_T("C:\\Users\\欢\\Desktop\\资料管理\\源码\\23"));
	m_frequent_list.AddString(_T("C:\\Users\\欢\\Desktop\\资料管理\\图书\\23"));
	m_frequent_list.AddString(_T("C:\\Users\\欢\\Desktop\\资料管理\\标签\\23"));
	m_frequent_list.AddString(_T("C:\\Users\\欢\\Desktop\\资料管理\\源码\\23"));
	m_frequent_list.AddString(_T("C:\\Users\\欢\\Desktop\\资料管理\\源码\\23"));

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
				if (pFocus == &m_label_search) {
					m_label_search.GetWindowText(strSearchText);

					MoveToHideWindow(TRUE);
					CLabelTab(_T("search:") + strSearchText).DoModal();
					MoveToHideWindow(FALSE);

					m_label_search.SetWindowText(_T(""));
					m_label_search.SetFocus();

				} else if (pFocus == &m_project_search) {
					m_project_search.GetWindowText(strSearchText);

					MoveToHideWindow(TRUE);
					CProjectTab(_T("search:") + strSearchText).DoModal();
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

	return CDialogEx::PreTranslateMessage(pMsg);
}

void CMainSearch::OnBnClickedAddLabel()
{
	MoveToHideWindow(TRUE);
	CLabelTab(_T("add")).DoModal();
	MoveToHideWindow(FALSE);

	m_project_search.SetFocus();
}

void CMainSearch::OnBnClickedAddProject()
{
	MoveToHideWindow(TRUE);
	CProjectTab(_T("add")).DoModal();
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

void CMainSearch::OnActivate(UINT nState, CWnd* pWndOther, BOOL bMinimized)
{
	CDialogEx::OnActivate(nState, pWndOther, bMinimized);

	if (nState != WA_INACTIVE && GetFocus() != &m_project_search) {
		m_project_search.SetFocus();
	}
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
	CDialogEx::OnMove(x, y);

	if (x >= 0 && y >= 0) {
		GetWindowRect(m_rect_if_visiable);
	}
}

void CMainSearch::OnRecentListChange()
{
	m_recent_list.ResetContent();

	auto& list = theSetting.GetRecentFileList();
	for (int i = 0; i < list.size(); i++) {
		m_recent_list.AddString(list.at(i));
	}
}