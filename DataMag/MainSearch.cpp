#include "stdafx.h"
#include "DataMag.h"
#include "BookTab.h"
#include "LabelTab.h"
#include "ProjectTab.h"
#include "MainSearch.h"

IMPLEMENT_DYNAMIC(CMainSearch, CDialogEx)

CMainSearch::CMainSearch(CWnd* pParent /*=NULL*/)
	: CDialogEx(CMainSearch::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);

	HICON hSearchIcon = (HICON)LoadImage(AfxGetInstanceHandle()
		, MAKEINTRESOURCE(IDI_SEARCH)
		, IMAGE_ICON, 0, 0, 0);
	m_book_search.SetSearchIcon(hSearchIcon);
	m_label_search.SetSearchIcon(hSearchIcon);
	m_project_search.SetSearchIcon(hSearchIcon);
}

CMainSearch::~CMainSearch()
{
}

void CMainSearch::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_MY_BLOG, m_blog);
	DDX_Control(pDX, IDC_ADD_BOOK, m_add_book);
	DDX_Control(pDX, IDC_ADD_LABEL, m_add_label);
	DDX_Control(pDX, IDC_BOOK_SEARCH, m_book_search);
	DDX_Control(pDX, IDC_ADD_PROJECT, m_add_project);
	DDX_Control(pDX, IDC_LABEL_SEARCH, m_label_search);
	DDX_Control(pDX, IDC_PROJECT_SEARCH, m_project_search);
}

BEGIN_MESSAGE_MAP(CMainSearch, CDialogEx)
	ON_BN_CLICKED(IDC_ADD_PROJECT, &CMainSearch::OnBnClickedAddProject)
	ON_BN_CLICKED(IDC_ADD_LABEL, &CMainSearch::OnBnClickedAddLabel)
	ON_BN_CLICKED(IDC_ADD_BOOK, &CMainSearch::OnBnClickedAddBook)
	ON_WM_ACTIVATE()
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
	m_add_label.SetImage(hLabelIcon);

	HICON hProjectIcon = (HICON)LoadImage(AfxGetInstanceHandle()
		, MAKEINTRESOURCE(IDI_CODE)
		, IMAGE_ICON, 0, 0, 0);
	m_add_project.SetImage(hProjectIcon);

	HICON hBookIcon = (HICON)LoadImage(AfxGetInstanceHandle()
		, MAKEINTRESOURCE(IDI_BOOK)
		, IMAGE_ICON, 0, 0, 0);
	m_add_book.SetImage(hBookIcon);

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

					CLabelTab(_T("search:") + strSearchText).DoModal();

					m_label_search.SetWindowText(_T(""));
					m_label_search.SetFocus();

				} else if (pFocus == &m_project_search) {
					m_project_search.GetWindowText(strSearchText);

					CProjectTab(_T("search:") + strSearchText).DoModal();

					m_project_search.SetWindowText(_T(""));
					m_project_search.SetFocus();

				} else if (pFocus == &m_book_search) {
					m_book_search.GetWindowText(strSearchText);

					CBookTab(_T("search:") + strSearchText).DoModal();

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
	CLabelTab(_T("add")).DoModal();
	m_project_search.SetFocus();
}

void CMainSearch::OnBnClickedAddProject()
{
	CProjectTab(_T("add")).DoModal();
	m_project_search.SetFocus();
}

void CMainSearch::OnBnClickedAddBook()
{
	CBookTab(_T("add")).DoModal();
	m_project_search.SetFocus();
}

void CMainSearch::OnActivate(UINT nState, CWnd* pWndOther, BOOL bMinimized)
{
	CDialogEx::OnActivate(nState, pWndOther, bMinimized);

	if (nState != WA_INACTIVE && GetFocus() != &m_project_search) {
		m_project_search.SetFocus();
	}
}