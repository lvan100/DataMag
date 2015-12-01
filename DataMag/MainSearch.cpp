#include "stdafx.h"
#include "DataMag.h"
#include "MainSearch.h"
#include "ResourceSet.h"

#include <set>
#include <map>
#include <vector>
#include <algorithm>
using namespace std;

IMPLEMENT_DYNAMIC(CMainSearch, CDialogEx)

CMainSearch::CMainSearch(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_MAINSEARCH1, pParent)
	, m_recommand_list(&theShellManager)
	, m_recent_list(&theShellManager)
{
	m_search_pad = new CSearchPad();
	m_detail_page = new CDetailPage();

	m_search_edit.SetSearchIcon(theApp.GetSearchIcon());

	m_recent_list.SetListBoxEvent(&m_recent_list_event);
	m_recommand_list.SetListBoxEvent(&m_recommand_list_event);

	RecentListChangeListener listener;
	listener = bind(&CMainSearch::OnRecentListChange, this);
	theApp.AddRecentListChangeListener(this, listener);

	ShowDetailEvent showDetail;
	showDetail = bind(&CMainSearch::OnShowDetailPage, this
		, std::placeholders::_1
		, std::placeholders::_2);
	m_search_pad->SetShowDetailEvent(showDetail);
}

CMainSearch::~CMainSearch()
{
	delete m_search_pad;
	delete m_detail_page;
}

void CMainSearch::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_MAIN_SEARCH, m_search_edit);
	DDX_Control(pDX, IDC_RECENT_LIST, m_recent_list);
	DDX_Control(pDX, IDC_RECENT_LABEL, m_recent_label);
	DDX_Control(pDX, IDC_SEARCH_COMBO, m_search_filter);
	DDX_Control(pDX, IDC_RECOMMEND_LIST, m_recommand_list);
	DDX_Control(pDX, IDC_RECOMMEND_LABEL, m_recommand_label);
}

BEGIN_MESSAGE_MAP(CMainSearch, CDialogEx)
	ON_WM_SETFOCUS()
	ON_EN_CHANGE(IDC_MAIN_SEARCH, &CMainSearch::OnEnChangeMainSearch)
	ON_CBN_SELCHANGE(IDC_SEARCH_COMBO, &CMainSearch::OnCbnSelchangeSearchCombo)
END_MESSAGE_MAP()

void CMainSearch::RecentListEvent::InitListBox()
{
	auto pThis = ((CMainSearch*)((BYTE*)this - offsetof(CMainSearch, m_recent_list_event)));

	int itemHeight = pThis->m_recent_list.GetMeasuredItemHeight();

	CRect rcWindow;
	pThis->m_recent_list.GetWindowRect(rcWindow);

	pThis->ScreenToClient(rcWindow);

	rcWindow.bottom = rcWindow.top + itemHeight * CDataMagApp::MaxRecentFileCount + 2;
	pThis->m_recent_list.MoveWindow(rcWindow);
}

void CMainSearch::RecentListEvent::OnDoubleClick()
{
	auto pThis = ((CMainSearch*)((BYTE*)this - offsetof(CMainSearch, m_recent_list_event)));

	int nItem = pThis->m_recent_list.GetCurSel();
	CString strPath = pThis->m_recent_list.GetItemPath(nItem);

	if (PathFileExists(strPath)) {
		theApp.SetRecentFile(strPath);
		ShellExecute(NULL, _T("open"), strPath, NULL, strPath, SW_SHOWMAXIMIZED);
	} else {
		if (pThis->MessageBox(_T("找不到选择项，是否从最近访问列表中删除？"), _T("提示"), MB_OKCANCEL) == IDOK) {
			theApp.RemoveRecentFile(strPath);
		}
	}
}

void CMainSearch::RecommandListEvent::InitListBox()
{
	auto pThis = ((CMainSearch*)((BYTE*)this - offsetof(CMainSearch, m_recommand_list_event)));

	int itemHeight = pThis->m_recommand_list.GetMeasuredItemHeight();

	CRect rcWindow;
	pThis->m_recommand_list.GetWindowRect(rcWindow);

	pThis->ScreenToClient(rcWindow);

	rcWindow.bottom = rcWindow.top + itemHeight * CDataMagApp::MaxRecentFileCount + 2;
	pThis->m_recommand_list.MoveWindow(rcWindow);
}

void CMainSearch::RecommandListEvent::OnDoubleClick()
{
	auto pThis = ((CMainSearch*)((BYTE*)this - offsetof(CMainSearch, m_recommand_list_event)));

	int nItem = pThis->m_recommand_list.GetCurSel();
	CString strPath = pThis->m_recommand_list.GetItemPath(nItem);

	ShellExecute(NULL, _T("open"), strPath, NULL, strPath, SW_SHOWMAXIMIZED);
	theApp.SetRecentFile(strPath);
}

void CMainSearch::OnRecentListChange()
{
	m_recent_list.SetRedraw(FALSE);
	{
		m_recent_list.ResetContent();

		auto& list = theApp.GetRecentFileList();
		for (size_t i = 0; i < list.size(); i++) {
			m_recent_list.AddString(list.at(i));
		}
	}
	m_recent_list.SetRedraw(TRUE);
}

BOOL CMainSearch::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	SetIcon(theApp.GetAppIcon(), TRUE);
	SetIcon(theApp.GetAppIcon(), FALSE);

	m_search_pad->Create(IDD_SEARCHPAD, this);
	m_detail_page->Create(IDD_DETAILPAGE, this);

	HICON hTagIcon = (HICON)LoadImage(AfxGetInstanceHandle()
		, MAKEINTRESOURCE(IDI_TAG)
		, IMAGE_ICON, 0, 0, 0);

	HICON hCodeIcon = (HICON)LoadImage(AfxGetInstanceHandle()
		, MAKEINTRESOURCE(IDI_CODE)
		, IMAGE_ICON, 0, 0, 0);

	HICON hBookIcon = (HICON)LoadImage(AfxGetInstanceHandle()
		, MAKEINTRESOURCE(IDI_BOOK)
		, IMAGE_ICON, 0, 0, 0);

	m_recent_list.SetTagImage(hTagIcon);
	m_recent_list.SetBookImage(hBookIcon);
	m_recent_list.SetCodeImage(hCodeIcon);

	m_recommand_list.SetTagImage(hTagIcon);
	m_recommand_list.SetBookImage(hBookIcon);
	m_recommand_list.SetCodeImage(hCodeIcon);

	HICON hRenameIcon = (HICON)LoadImage(AfxGetInstanceHandle()
		, MAKEINTRESOURCE(IDI_RENAME)
		, IMAGE_ICON, 0, 0, 0);

	HICON hInfoIcon = (HICON)LoadImage(AfxGetInstanceHandle()
		, MAKEINTRESOURCE(IDI_INFO)
		, IMAGE_ICON, 0, 0, 0);

	HICON hDeleteIcon = (HICON)LoadImage(AfxGetInstanceHandle()
		, MAKEINTRESOURCE(IDI_DELETE)
		, IMAGE_ICON, 0, 0, 0);

	m_search_pad->SetTagImage(hTagIcon);
	m_search_pad->SetBookImage(hBookIcon);
	m_search_pad->SetInfoImage(hInfoIcon);
	m_search_pad->SetCodeImage(hCodeIcon);
	m_search_pad->SetDeleteImage(hDeleteIcon);
	m_search_pad->SetRenameImage(hRenameIcon);

	m_detail_page->SetTagImage(hTagIcon);
	m_detail_page->SetBookImage(hBookIcon);
	m_detail_page->SetInfoImage(hInfoIcon);
	m_detail_page->SetCodeImage(hCodeIcon);
	m_detail_page->SetDeleteImage(hDeleteIcon);
	m_detail_page->SetRenameImage(hRenameIcon);

	m_recent_label.SetFont(theResourceSet.GetFontBySize(13));
	m_search_filter.SetFont(theResourceSet.GetFontBySize(12));
	m_recommand_label.SetFont(theResourceSet.GetFontBySize(13));

	int searchFilter = theApp.GetProfileInt(_T("Settings"), _T("SearchFilter"), 0);
	m_search_filter.SetCurSel(searchFilter);

	// 填充最近访问列表数据
	auto& list = theApp.GetRecentFileList();
	for (size_t i = 0; i < list.size(); i++) {
		m_recent_list.AddString(list.at(i));
		m_recommand_list.AddString(list.at(i));
	}

	// 完成随机推荐过程
	// DoRandomRecommand();

	// 适配屏幕DPI
	AdjustAnyDpi();
	
	// 窗口居中显示
	CenterWindow();

	// 设置默认焦点控件
	m_search_edit.SetFocus(); /* 自动搜索 */
	m_search_edit.EnableSearchButton(FALSE);

	return FALSE; /* 自定义焦点 */
}

/**
 * 获取绘制文字所需的区域大小
 */
STATIC CSize GetTextExtentSize(CString strText, int fontSize)
{
	CSize size;
	HDC hDC = GetDC(GetDesktopWindow());
	CFont* pFont = theResourceSet.GetFontBySize(fontSize);
	HFONT hOldFont = (HFONT)SelectObject(hDC, pFont->GetSafeHandle());
	GetTextExtentPoint32(hDC, strText, (int)strText.GetLength(), &size);
	ReleaseDC(GetDesktopWindow(), hDC);
	return size;
}

void CMainSearch::AdjustAnyDpi()
{
	CRect rcClient;
	GetClientRect(rcClient);

	CRect rcSearchFilter;
	m_search_filter.GetWindowRect(rcSearchFilter);
	ScreenToClient(rcSearchFilter);

	CSize sizeText = GetTextExtentSize(_T("标签"), 12);
	CSize sizeLabel = GetTextExtentSize(_T("标签"), 13);

	CRect rcSearchEdit;
	rcSearchEdit.top = rcSearchFilter.top;
	rcSearchEdit.bottom = rcSearchFilter.bottom;
	rcSearchEdit.left = rcSearchFilter.right + 2;
	rcSearchEdit.right = rcClient.right - rcSearchFilter.left;
	m_search_edit.MoveWindow(rcSearchEdit);

	CRect rcRecentLabel;
	rcRecentLabel.left = rcSearchFilter.left;
	rcRecentLabel.right = rcSearchEdit.right;
	rcRecentLabel.top = rcSearchFilter.bottom + 12;
	rcRecentLabel.bottom = rcRecentLabel.top + sizeLabel.cy;
	m_recent_label.MoveWindow(rcRecentLabel);

	CRect rcRecentList;
	rcRecentList.left = rcRecentLabel.left + 1;
	rcRecentList.right = rcRecentLabel.right - 1;
	rcRecentList.top = rcRecentLabel.bottom + 4 + 1;
	rcRecentList.bottom = rcRecentList.top + CDataMagApp::MaxRecentFileCount * (sizeText.cy + 8);
	m_recent_list.MoveWindow(rcRecentList);

	CRect rcRecommandLabel;
	rcRecommandLabel.left = rcRecentList.left;
	rcRecommandLabel.right = rcRecentList.right;
	rcRecommandLabel.top = rcRecentList.bottom + 12;
	rcRecommandLabel.bottom = rcRecommandLabel.top + sizeLabel.cy;
	m_recommand_label.MoveWindow(rcRecommandLabel);

	CRect rcRecommandList;
	rcRecommandList.left = rcRecommandLabel.left + 1;
	rcRecommandList.right = rcRecommandLabel.right - 1;
	rcRecommandList.top = rcRecommandLabel.bottom + 4 + 1;
	rcRecommandList.bottom = rcRecommandList.top + CDataMagApp::MaxRecentFileCount * (sizeText.cy + 8);
	m_recommand_list.MoveWindow(rcRecommandList);

	CRect rcWindow;
	GetWindowRect(rcWindow);
	rcWindow.bottom += (rcRecommandList.bottom + rcSearchFilter.top - rcClient.Height());
	MoveWindow(rcWindow);
}

void CMainSearch::OnSetFocus(CWnd* pOldWnd)
{
	CDialogEx::OnSetFocus(pOldWnd);

	if (GetFocus() != &m_search_edit) {
		m_search_edit.SetFocus();
	}

	// 设置不默认选中全部文本
	m_search_edit.SetSel(-1);
}

CRect CMainSearch::GetSearchPadRect()
{
	CRect rcSearchFilter;
	m_search_filter.GetWindowRect(rcSearchFilter);
	
	CRect rcRecommandList;
	m_recommand_list.GetWindowRect(rcRecommandList);

	return CRect(rcSearchFilter.left
		, rcSearchFilter.bottom + 2
		, rcRecommandList.right + 1
		, rcRecommandList.bottom);
}

void CMainSearch::ShowSearchPad(bool bShow)
{
	m_search_pad->ShowWindow(bShow ? SW_SHOW : SW_HIDE);
	m_search_pad->MoveWindow(GetSearchPadRect(), FALSE);
	m_search_pad->Invalidate(bShow ? TRUE : FALSE);

	// 将焦点始终设置在搜索框上
	if (GetFocus() != &m_search_edit) {
		m_search_edit.SetFocus();
	}
}

void CMainSearch::ShowDetailPage(bool bShow)
{
	m_detail_page->ShowWindow(bShow ? SW_SHOW : SW_HIDE);
	m_detail_page->MoveWindow(GetSearchPadRect(), FALSE);
	m_detail_page->Invalidate(bShow ? TRUE : FALSE);

	// 将焦点始终设置在搜索框上
	if (GetFocus() != &m_search_edit) {
		m_search_edit.SetFocus();
	}
}

/**
 * 枚举文件夹内容，并返回内容数量
 */
STATIC int SimpleEnumFolder(LPCTSTR lpszPath	// 文件夹路径
	, CShellManager* pShellManager				// Shell管理器
	, function<void(LPITEMIDLIST)> filter)		// 过滤器函数
{
	ENSURE(lpszPath != nullptr);
	ASSERT_VALID(pShellManager);

	AFX_SHELLITEMINFO info;
	HRESULT hr = pShellManager->ItemFromPath(lpszPath, info.pidlRel);
	if (FAILED(hr)) {
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

				if (filter) { /* VS2015 的新特性，增加了 operator bool() 函数 */
					LPITEMIDLIST itemID = pShellManager->ConcatenateItem(info.pidlRel, pidlTemp);
					filter(itemID); /* 通过回调由客户端自行决定 */
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

void CMainSearch::DoRandomRecommand()
{
	int nCodeCount = SimpleEnumFolder(theApp.GetCodeDir()
		, &theShellManager, nullptr);
	int nBookCount = SimpleEnumFolder(theApp.GetBookDir()
		, &theShellManager, nullptr);

	int nCount = nCodeCount + nBookCount;
	if (nCount == 0) {
		return;
	}

	srand((unsigned int)time(nullptr));

	set<int> codeBookSet;

	do {
		codeBookSet.insert(rand() % nCount);
		if (codeBookSet.size() >= (size_t)nCount) { break; }
	} while (codeBookSet.size() < CDataMagApp::MaxRecentFileCount);

	vector<int> codeSet, bookSet;

	for_each(codeBookSet.begin(), codeBookSet.end(), [&](int n) {
		if (n < nCodeCount) {
			codeSet.push_back(n);
		} else {
			bookSet.push_back(n - nCodeCount);
		}
	});

	unsigned int nItemIndex = 0, nSetIndex = 0;

	SimpleEnumFolder(theApp.GetCodeDir(), &theShellManager, [&](LPITEMIDLIST itemID) {
		if (nSetIndex < codeSet.size() && codeSet.at(nSetIndex) == nItemIndex) {
			TCHAR szPath[MAX_PATH] = { 0 };
			if (SHGetPathFromIDList(itemID, szPath)) {
				m_recommand_values.push_back(szPath);
			}
			nSetIndex++;
		}
		nItemIndex++;
	});

	nItemIndex = 0, nSetIndex = 0;

	SimpleEnumFolder(theApp.GetBookDir(), &theShellManager, [&](LPITEMIDLIST itemID) {
		if (nSetIndex < bookSet.size() && bookSet.at(nSetIndex) == nItemIndex) {
			TCHAR szPath[MAX_PATH] = { 0 };
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

BOOL CMainSearch::PreTranslateMessage(MSG* pMsg)
{
	if (pMsg->message == WM_KEYDOWN) {

		if (GetFocus() != &m_search_edit) {
			m_search_edit.SetFocus();
		}

		if (pMsg->wParam == VK_ESCAPE) {
			if (m_search_pad->IsWindowVisible()) {
				m_search_edit.SetWindowText(_T(""));
				ShowSearchPad(false);
				return TRUE;
			}
			if (m_detail_page->IsWindowVisible()) {
				m_search_edit.SetWindowText(_T(""));
				ShowDetailPage(false);
				return TRUE;
			}
		}

		if (pMsg->wParam == VK_DOWN) {
			if (m_search_pad->IsWindowVisible()) {
				m_search_pad->SelectNextItem();
				return TRUE;
			}
		}

		if (pMsg->wParam == VK_UP) {
			if (m_search_pad->IsWindowVisible()) {
				m_search_pad->SelectPrevItem();
				return TRUE;
			}
		}
	}

	return CDialogEx::PreTranslateMessage(pMsg);
}

void CMainSearch::OnEnChangeMainSearch()
{
	m_search_pad->ClearResult();

	CString strSearchText;
	m_search_edit.GetWindowText(strSearchText);
	m_search_pad->SetSearchText(strSearchText);

	CString strSearchFilter;
	m_search_filter.GetWindowText(strSearchFilter);
	m_search_pad->SetSearchFilter(strSearchFilter);
	
	if (strSearchText.GetLength() > 0) {

		vector<CString> arrResult;

		auto FilterPath = [&](LPITEMIDLIST itemID) {
			TCHAR szPath[MAX_PATH] = { 0 };
			if (SHGetPathFromIDList(itemID, szPath)) {
				CString strPath = CString(szPath).MakeLower();
				if (strPath.Find(strSearchText.MakeLower()) >= 0) {
					arrResult.push_back(szPath);
				}
			}
		};

		if (strSearchFilter == _T("标签") || strSearchFilter == _T("全部")) {
			SimpleEnumFolder(theApp.GetTagDir(), &theShellManager, FilterPath);
			m_search_pad->AddTag(arrResult);
			arrResult.clear();
		}

		if (strSearchFilter == _T("源码") || strSearchFilter == _T("全部")) {
			SimpleEnumFolder(theApp.GetCodeDir(), &theShellManager, FilterPath);
			m_search_pad->AddCode(arrResult);
			arrResult.clear();
		}

		if (strSearchFilter == _T("图书") || strSearchFilter == _T("全部")) {
			SimpleEnumFolder(theApp.GetBookDir(), &theShellManager, FilterPath);
			m_search_pad->AddBook(arrResult);
			arrResult.clear();
		}
	}

	// 执行显示前的准备工作
	m_search_pad->Prepare();

	// 显示或者隐藏搜索结果对话框
	ShowSearchPad(strSearchText.GetLength() > 0);
}

void CMainSearch::OnCbnSelchangeSearchCombo()
{
	OnEnChangeMainSearch();

	// 将本次选择的过滤器保存起来，下次启动的时候使用本次配置。
	theApp.WriteProfileInt(_T("Settings"), _T("SearchFilter"), m_search_filter.GetCurSel());
}

void CMainSearch::OnShowDetailPage(CString strCatalog, CString strPath)
{
	ShowSearchPad(false);

	m_detail_page->SetCatalog(strCatalog);
	m_detail_page->SetPath(strPath);
	m_detail_page->Prepare();
	ShowDetailPage(true);
}