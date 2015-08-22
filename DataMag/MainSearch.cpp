#include "stdafx.h"
#include "DataMag.h"
#include "BookTab.h"
#include "TagTab.h"
#include "CodeTab.h"
#include "MainSearch.h"
#include "DDXControl.h"

#include <set>
#include <map>
#include <vector>
#include <algorithm>
using namespace std;

/**
 * 全局的主搜索对话框对象
 */
CMainSearch* theMainSearch = nullptr;

IMPLEMENT_DYNAMIC(CMainSearch, CAppWnd)

CMainSearch::CMainSearch(CWnd* pParent /*=nullptr*/)
	: CAppWnd(CMainSearch::IDD, pParent)
	, m_recommand_list(&theShellManager)
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
	m_code_search.SetSearchIcon(hSearchIcon);

	m_recent_list.SetListEvent(&m_recent_list_event);
	m_recommand_list.SetListEvent(&m_recommand_list_event);

	RecentListChangeListener listener;
	listener = bind(&CMainSearch::OnRecentListChange, this);
	theApp.AddRecentListChangeListener(this, listener);
}

CMainSearch::~CMainSearch()
{
	theApp.DeleteRecentListChangeListener(this);
}

void CMainSearch::DoDataExchange(CDataExchange* pDX)
{
	CAppWnd::DoDataExchange(pDX);
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

BEGIN_MESSAGE_MAP(CMainSearch, CAppWnd)
	ON_BN_CLICKED(IDC_ADD_CODE, &CMainSearch::OnBnClickedAddProject)
	ON_BN_CLICKED(IDC_ADD_BOOK, &CMainSearch::OnBnClickedAddBook)
	ON_BN_CLICKED(IDC_ADD_TAG, &CMainSearch::OnBnClickedAddTag)
	ON_WM_MOVE()
END_MESSAGE_MAP()

void CMainSearch::RecentListEvent::OnDoubleClick()
{
	auto pThis = ((CMainSearch*)((BYTE*)this - offsetof(CMainSearch, m_recent_list_event)));

	int nItem = pThis->m_recent_list.GetCurSel();
	pThis->m_recent_list.DoDefaultDClick(nItem);

	CString strPath = pThis->m_recent_list.GetItemPath(nItem);
	theApp.SetRecentFile(strPath);
}

void CMainSearch::RecommandListEvent::OnDoubleClick()
{
	auto pThis = ((CMainSearch*)((BYTE*)this - offsetof(CMainSearch, m_recommand_list_event)));

	int nItem = pThis->m_recommand_list.GetCurSel();
	pThis->m_recommand_list.DoDefaultDClick(nItem);

	CString strPath = pThis->m_recommand_list.GetItemPath(nItem);
	theApp.SetRecentFile(strPath);
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

	m_recent_list.SetTagImage(hTagIcon);
	m_recent_list.SetBookImage(hBookIcon);
	m_recent_list.SetCodeImage(hProjectIcon);

	m_recommand_list.SetTagImage(hTagIcon);
	m_recommand_list.SetBookImage(hBookIcon);
	m_recommand_list.SetCodeImage(hProjectIcon);
	
	auto event = bind(&CMainSearch::DoSearch, this);

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

	// 进行推荐
	DoRecommand();

	return FALSE;
}

void CMainSearch::DoSearch()
{
	CString strSearchText;

	CWnd* pFocus = GetFocus();
	if (pFocus == &m_tag_search)
	{
		m_tag_search.GetWindowText(strSearchText);

		MoveToHideWindow(TRUE);
		CTagTab(_T("search:") + strSearchText).DoModal();
		MoveToHideWindow(FALSE);

		m_tag_search.SetWindowText(_T(""));
		m_tag_search.SetFocus();

	} 
	else if (pFocus == &m_code_search)
	{
		m_code_search.GetWindowText(strSearchText);

		MoveToHideWindow(TRUE);
		CCodeTab(_T("search:") + strSearchText).DoModal();
		MoveToHideWindow(FALSE);

		m_code_search.SetWindowText(_T(""));
		m_code_search.SetFocus();

	}
	else if (pFocus == &m_book_search)
	{
		m_book_search.GetWindowText(strSearchText);

		MoveToHideWindow(TRUE);
		CBookTab(_T("search:") + strSearchText).DoModal();
		MoveToHideWindow(FALSE);

		m_book_search.SetWindowText(_T(""));
		m_book_search.SetFocus();
	}
}

BOOL CMainSearch::PreTranslateMessage(MSG* pMsg)
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

	return CAppWnd::PreTranslateMessage(pMsg);
}

void CMainSearch::OnBnClickedAddTag()
{
	MoveToHideWindow(TRUE);
	CTagTab(_T("add")).DoModal();
	MoveToHideWindow(FALSE);

	m_code_search.SetFocus();
}

void CMainSearch::OnBnClickedAddProject()
{
	MoveToHideWindow(TRUE);
	CCodeTab(_T("add")).DoModal();
	MoveToHideWindow(FALSE);

	m_code_search.SetFocus();
}

void CMainSearch::OnBnClickedAddBook()
{
	MoveToHideWindow(TRUE);
	CBookTab(_T("add")).DoModal();
	MoveToHideWindow(FALSE);

	m_code_search.SetFocus();
}

void CMainSearch::MoveToHideWindow(BOOL bHide)
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

/**
 * 简单的枚举文件夹内容，返回内容数量
 */
int SimpleEnumFolder(LPCTSTR lpszPath		// 文件夹路径
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

				if (!filter._Empty()) {
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

void CMainSearch::DoRecommand()
{
	m_recommand_values.clear();
	m_recommand_list.ResetContent();

	int nCodeCount = SimpleEnumFolder(theApp.GetCodeDir(), &theShellManager, function<void(LPITEMIDLIST)>());
	int nBookCount = SimpleEnumFolder(theApp.GetBookDir(), &theShellManager, function<void(LPITEMIDLIST)>());

	int nCount = nCodeCount + nBookCount;

	srand((unsigned int)time(nullptr));

	set<int> codeBookSet;

	do {
		int nRander = rand() % nCount;
		codeBookSet.insert(nRander);
	} while(codeBookSet.size() < CDataMagApp::MaxRecentFileCount);

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