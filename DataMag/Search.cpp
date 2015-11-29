#include "stdafx.h"
#include "DataMag.h"
#include "Search.h"
#include "DDXControl.h"

#include <set>
#include <map>
#include <vector>
#include <algorithm>
using namespace std;

IMPLEMENT_DYNAMIC(CSearch, CDialogEx)

CSearch::CSearch(CWnd* pParent)
	: CDialogEx(CSearch::IDD, pParent)
	, m_recommand_list(&theShellManager)
	, m_recent_list(&theShellManager)
	, m_pLastFocusWnd(nullptr)
	, m_book_tab(nullptr)
	, m_code_tab(nullptr)
	, m_tag_tab(nullptr)
{
	HICON hSearchIcon = theApp.GetSearchIcon();
	m_tag_search.SetSearchIcon(hSearchIcon);
	m_book_search.SetSearchIcon(hSearchIcon);
	m_code_search.SetSearchIcon(hSearchIcon);

	m_recent_list.SetListBoxEvent(&m_recent_list_event);
	m_recommand_list.SetListBoxEvent(&m_recommand_list_event);

	RecentListChangeListener listener;
	listener = bind(&CSearch::OnRecentListChange, this);
	theApp.AddRecentListChangeListener(this, listener);
}

CSearch::~CSearch()
{
	theApp.DeleteRecentListChangeListener(this);
}

void CSearch::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	MFC_DDX_Control(pDX, IDC_ADD_TAG, m_add_tag);
	MFC_DDX_Control(pDX, IDC_ADD_BOOK, m_add_book);
	DDX_Control(pDX, IDC_RECENT_LIST, m_recent_list);
	MFC_DDX_Control(pDX, IDC_ADD_CODE, m_add_project);
	MFC_DDX_Control(pDX, IDC_TAG_SEARCH, m_tag_search);
	MFC_DDX_Control(pDX, IDC_CODE_SEARCH, m_code_search);
	MFC_DDX_Control(pDX, IDC_BOOK_SEARCH, m_book_search);
	MFC_DDX_Control(pDX, IDC_RECENT_GROUP, m_recent_group);
	DDX_Control(pDX, IDC_RECOMMEND_LIST, m_recommand_list);
	MFC_DDX_Control(pDX, IDC_RECOMMEND_GROUP, m_recommand_group);
}

BEGIN_MESSAGE_MAP(CSearch, CDialogEx)
	ON_BN_CLICKED(IDC_ADD_CODE, &CSearch::OnBnClickedAddProject)
	ON_BN_CLICKED(IDC_ADD_BOOK, &CSearch::OnBnClickedAddBook)
	ON_BN_CLICKED(IDC_ADD_TAG, &CSearch::OnBnClickedAddTag)
	ON_WM_ACTIVATE()
	ON_WM_MOVE()
END_MESSAGE_MAP()

void CSearch::RecentListEvent::OnDoubleClick()
{
	auto pThis = ((CSearch*)((BYTE*)this - offsetof(CSearch, m_recent_list_event)));

	int nItem = pThis->m_recent_list.GetCurSel();
	CString strPath = pThis->m_recent_list.GetItemPath(nItem);

	if (PathFileExists(strPath)){
		theApp.SetRecentFile(strPath);
		pThis->m_recent_list.DoDoubleClick(nItem);
	} else {
		if (pThis->MessageBox(_T("找不到选择项，是否从最近访问列表中删除？"), _T("提示"), MB_OKCANCEL) == IDOK) {
			theApp.RemoveRecentFile(strPath);
		}
	}
}

void CSearch::RecommandListEvent::OnDoubleClick()
{
	auto pThis = ((CSearch*)((BYTE*)this - offsetof(CSearch, m_recommand_list_event)));

	int nItem = pThis->m_recommand_list.GetCurSel();
	pThis->m_recommand_list.DoDoubleClick(nItem);

	CString strPath = pThis->m_recommand_list.GetItemPath(nItem);
	theApp.SetRecentFile(strPath);
}

BOOL CSearch::OnInitDialog()
{
	CDialogEx::OnInitDialog();

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

	m_recent_list.SetTagImage(hTagIcon);
	m_recent_list.SetBookImage(hBookIcon);
	m_recent_list.SetCodeImage(hProjectIcon);

	m_recommand_list.SetTagImage(hTagIcon);
	m_recommand_list.SetBookImage(hBookIcon);
	m_recommand_list.SetCodeImage(hProjectIcon);
	
	auto event = bind(&CSearch::DoSearch, this);

	m_tag_search.SetClickEvent(event);
	m_tag_search.SetHintText(_T("搜索标签"));

	m_book_search.SetClickEvent(event);
	m_book_search.SetHintText(_T("搜索图书"));

	m_code_search.SetClickEvent(event);
	m_code_search.SetHintText(_T("搜索项目"));

	[&](){
		LOGFONT logFont = { 0 };
		afxGlobalData.fontRegular.GetLogFont(&logFont);

		logFont.lfHeight = -16;

		HFONT hFont = CreateFontIndirect(&logFont);
		m_recent_group.SetFont(CFont::FromHandle(hFont));
		m_recommand_group.SetFont(CFont::FromHandle(hFont));
	}();

	auto& list = theApp.GetRecentFileList();
	for (size_t i = 0; i < list.size(); i++) {
		m_recent_list.AddString(list.at(i));
	}

	[&](){
		// 进行推荐
		DoRecommand();
	}();

	return FALSE;
}

void CSearch::CreateAndShowTagTab()
{
	MoveToHideWindow(TRUE);

	if (m_tag_tab == nullptr) {
		m_tag_tab = new CTagTab(this);
		m_tag_tab->Create(CTagTab::IDD);
		m_tag_tab->ShowWindow(SW_SHOW);
	} else {
		ASSERT(FALSE);
	}
}

void CSearch::CreateAndShowCodeTab()
{
	MoveToHideWindow(TRUE);

	if (m_code_tab == nullptr) {
		m_code_tab = new CCodeTab(this);
		m_code_tab->Create(CCodeTab::IDD);
		m_code_tab->ShowWindow(SW_SHOW);
	} else {
		ASSERT(FALSE);
	}
}

void CSearch::CreateAndShowBookTab()
{
	MoveToHideWindow(TRUE);

	if (m_book_tab == nullptr) {
		m_book_tab = new CBookTab(this);
		m_book_tab->Create(CBookTab::IDD);
		m_book_tab->ShowWindow(SW_SHOW);
	} else {
		ASSERT(FALSE);
	}
}

void CSearch::DeleteTabWnd(CWnd* pWnd)
{
	if (pWnd == m_code_tab){
		m_code_tab = nullptr;
	} else if (pWnd == m_book_tab){
		m_book_tab = nullptr;
	} else if (pWnd == m_tag_tab){
		m_tag_tab = nullptr;
	}

	if (m_pLastFocusWnd == pWnd){
		m_pLastFocusWnd = nullptr;
	}
	pWnd->DestroyWindow();
	delete pWnd;

	MoveToHideWindow(FALSE);
}

void CSearch::DoSearch()
{
	CString strSearchText;

	CWnd* pFocus = GetFocus();
	if (pFocus == &m_tag_search)
	{
		m_tag_search.GetWindowText(strSearchText);

		CreateAndShowTagTab();

		m_tag_search.SetWindowText(_T(""));
		m_tag_tab->DoCommandSearch(strSearchText);

	} 
	else if (pFocus == &m_code_search)
	{
		m_code_search.GetWindowText(strSearchText);

		CreateAndShowCodeTab();

		m_code_search.SetWindowText(_T(""));
		m_code_tab->DoCommandSearch(strSearchText);

	}
	else if (pFocus == &m_book_search)
	{
		m_book_search.GetWindowText(strSearchText);

		CreateAndShowBookTab();

		m_book_search.SetWindowText(_T(""));
		m_book_tab->DoCommandSearch(strSearchText);
	}
}

BOOL CSearch::PreTranslateMessage(MSG* pMsg)
{
	if (pMsg->message == WM_KEYDOWN)
	{
		switch((UINT)pMsg->wParam)
		{
		case VK_RETURN: {
				DoSearch();
			}
			return TRUE;
		default:
			break;
		}
	}

	return CDialogEx::PreTranslateMessage(pMsg);
}

void CSearch::OnBnClickedAddTag()
{
	CreateAndShowTagTab();
	m_tag_tab->DoCommandAdd();
}

void CSearch::OnBnClickedAddProject()
{
	CreateAndShowCodeTab();
	m_code_tab->DoCommandAdd();
}

void CSearch::OnBnClickedAddBook()
{
	CreateAndShowBookTab();
	m_book_tab->DoCommandAdd();
}

void CSearch::MoveToHideWindow(BOOL bHide)
{
	CRect rcWnd(m_rect_if_visiable);
	
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

void CSearch::OnMove(int x, int y)
{
	CDialogEx::OnMove(x, y);

	if (x >= 0 && y >= 0) {
		GetWindowRect(m_rect_if_visiable);
	}
}

void CSearch::OnRecentListChange()
{
	m_recent_list.ResetContent();

	auto& list = theApp.GetRecentFileList();
	for (size_t i = 0; i < list.size(); i++) {
		m_recent_list.AddString(list.at(i));
	}
}

/**
 * 简单的枚举文件夹内容，返回内容数量
 */
static int SimpleEnumFolder(LPCTSTR lpszPath		// 文件夹路径
	, CShellManager* pShellManager			// Shell管理器
	, function<void(LPITEMIDLIST)> filter)	// 过滤器函数
{
	ENSURE(lpszPath != nullptr);
	ASSERT_VALID(pShellManager);

	AFX_SHELLITEMINFO info;
	HRESULT hr = pShellManager->ItemFromPath(lpszPath, info.pidlRel);
	if (FAILED(hr))	{
		return 0;
	}

	int nFolderCount = 0;

	LPSHELLFOLDER pDesktopFolder;
	hr = SHGetDesktopFolder(&pDesktopFolder);

	if (SUCCEEDED(hr)) {

		IShellFolder* psfCurFolder = nullptr;
		hr = pDesktopFolder->BindToObject(info.pidlRel, nullptr, IID_IShellFolder, (LPVOID*)&psfCurFolder);

		LPENUMIDLIST pEnum = nullptr;
		HRESULT hRes = psfCurFolder->EnumObjects(nullptr, (SHCONTF)(SHCONTF_FOLDERS), &pEnum);
		if (SUCCEEDED(hRes) && pEnum != nullptr) {

			DWORD dwFetched = 1;
			LPITEMIDLIST pidlTemp;
			while (pEnum->Next(1, &pidlTemp, &dwFetched) == S_OK && dwFetched) {

				if (filter != false) {
					LPITEMIDLIST itemID = pShellManager->ConcatenateItem(info.pidlRel, pidlTemp);
					filter(itemID);
					pShellManager->FreeItem(itemID);
				}

				pShellManager->FreeItem(pidlTemp);

				nFolderCount++;
				dwFetched = 0;
			}

			pEnum->Release();
		}

		psfCurFolder->Release();
		pDesktopFolder->Release();
	}

	pShellManager->FreeItem(info.pidlRel);
	return nFolderCount;
}

void CSearch::DoRecommand()
{
	m_recommand_values.clear();
	m_recommand_list.ResetContent();

	int nCodeCount = SimpleEnumFolder(theApp.GetCodeDir(), &theShellManager, function<void(LPITEMIDLIST)>());
	int nBookCount = SimpleEnumFolder(theApp.GetBookDir(), &theShellManager, function<void(LPITEMIDLIST)>());

	int nCount = nCodeCount + nBookCount;
	if (nCount == 0) {
		return;
	}

	srand((unsigned int)time(nullptr));

	set<int> codeBookSet;

	do {
		int nRander = rand() % nCount;
		codeBookSet.insert(nRander);
	} while (codeBookSet.size() < (size_t)nCount && codeBookSet.size() < CDataMagApp::MaxRecentFileCount);

	vector<int> codeSet, bookSet;

	for_each (codeBookSet.begin(), codeBookSet.end(), [&](int n){
		if (n < nCodeCount) {
			codeSet.push_back(n);
		} else {
			bookSet.push_back(n - nCodeCount);
		}
	});

	unsigned int nItemIndex = 0, nSetIndex = 0;

	SimpleEnumFolder(theApp.GetCodeDir(), &theShellManager, [&](LPITEMIDLIST itemID){
		if (nSetIndex < codeSet.size() && codeSet.at(nSetIndex) == nItemIndex) {
			TCHAR szPath [MAX_PATH] = { 0 };
			if (SHGetPathFromIDList(itemID, szPath)) {
				m_recommand_values.push_back(szPath);
			}
			nSetIndex++;
		}
		nItemIndex++;
	});

	nItemIndex = 0, nSetIndex = 0;

	SimpleEnumFolder(theApp.GetBookDir(), &theShellManager, [&](LPITEMIDLIST itemID){
		if (nSetIndex < bookSet.size() && bookSet.at(nSetIndex) == nItemIndex) {
			TCHAR szPath [MAX_PATH] = { 0 };
			if (SHGetPathFromIDList(itemID, szPath)) {
				m_recommand_values.push_back(szPath);
			}
			nSetIndex++;
		}
		nItemIndex++;
	});

	int displaySet[CDataMagApp::MaxRecentFileCount] = { 0 };

	for (size_t iTag = 0; iTag < m_recommand_values.size();) {
		int nRander = rand() % m_recommand_values.size();
		if (displaySet[nRander] == 0) {
			CString strPath = m_recommand_values.at(nRander);
			m_recommand_list.AddString(strPath);
			displaySet[nRander] = 1;
			iTag++;
		}
	}
}

void CSearch::OnActivate(UINT nState, CWnd* pWndOther, BOOL bMinimized)
{
	CDialogEx::OnActivate(nState, pWndOther, bMinimized);

	if (nState == WA_INACTIVE ) {
		// 已经最小化的窗口不再处理
		if (!bMinimized) {
			if (m_tag_tab != nullptr) {
				m_pLastFocusWnd = m_tag_tab;
			} else if (m_code_tab != nullptr) {
				m_pLastFocusWnd = m_code_tab;
			} else if (m_book_tab != nullptr) {
				m_pLastFocusWnd = m_book_tab;
			} else {
				m_pLastFocusWnd = GetFocus();
			}
		}
	} else {
		if (m_pLastFocusWnd != nullptr) {
			m_pLastFocusWnd->SetFocus();
		} else {
			if (m_tag_tab != nullptr) {
				m_tag_tab->SetFocus();
			} else if (m_code_tab != nullptr) {
				m_code_tab->SetFocus();
			} else if (m_book_tab != nullptr) {
				m_book_tab->SetFocus();
			} else {
				CWnd* pWnd = GetDefaultFocusWnd();
				if (pWnd != nullptr) {
					pWnd->SetFocus();
				}
			}
		}
	}
}