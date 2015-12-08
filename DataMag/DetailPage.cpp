#include "stdafx.h"
#include "FileOp.h"
#include "DataMag.h"
#include "DetailPage.h"
#include "ResourceSet.h"
#include "ImageResource.h"

IMPLEMENT_DYNAMIC(CDetailPage, CDialogEx)

CDetailPage::CDetailPage(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_DETAILPAGE, pParent)
	, m_link_list(&theShellManager)
{
	m_link_list.SetListBoxEvent(&m_link_list_event);
}

CDetailPage::~CDetailPage()
{
}

void CDetailPage::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_ADD_LINK, m_add_link);
	DDX_Control(pDX, IDC_LINK_LIST, m_link_list);
	DDX_Control(pDX, IDC_LINK_FILTER, m_link_filter);
	DDX_Control(pDX, IDC_REMOVE_LINK, m_remove_link);
	DDX_Control(pDX, IDC_DETAIL_INFO, m_detial_info);
	DDX_Control(pDX, IDC_DEATIL_TITLE, m_detail_title);
}

BEGIN_MESSAGE_MAP(CDetailPage, CDialogEx)
	ON_CBN_SELCHANGE(IDC_LINK_FILTER, &CDetailPage::OnCbnSelchangeLinkFilter)
	ON_BN_CLICKED(IDC_ADD_LINK, &CDetailPage::OnBnClickedAddLink)
	ON_BN_CLICKED(IDC_REMOVE_LINK, &CDetailPage::OnBnClickedRemoveLink)
END_MESSAGE_MAP()

HICON CDetailPage::GetCatalogImage(CString catalog)
{
	if (catalog == _T("源码")) {
		return m_hCodeImage;
	}
	else if (catalog == _T("图书")) {
		return m_hBookImage;
	}
	else if (catalog == _T("标签")) {
		return m_hTagImage;
	}
	else {
		return nullptr;
	}
}

BOOL CDetailPage::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	m_detial_info.SetFont(theResourceSet.GetFontBySize(11));
	m_detail_title.SetFont(theResourceSet.GetFontBySize(13));
	
	m_add_link.SetImage(theImageResource.m_hAddRelatedIcon);
	m_remove_link.SetImage(theImageResource.m_hRemoveRelatedIcon);

	m_link_filter.SetCurSel(0);

	return TRUE;
}

BOOL CDetailPage::PreTranslateMessage(MSG* pMsg)
{
	if (pMsg->message == WM_KEYDOWN) {
		if (pMsg->wParam == VK_ESCAPE) {
			ShowWindow(SW_HIDE);
			return TRUE;
		}
	}

	return CDialogEx::PreTranslateMessage(pMsg);
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
		HRESULT hRes = psfCurFolder->EnumObjects(nullptr, (SHCONTF)(SHCONTF_NONFOLDERS), &pEnum);
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

void CDetailPage::OnCbnSelchangeLinkFilter()
{
	Refresh(m_catalog, m_path);
}

void CDetailPage::Refresh(CString strCatalog, CString strPath)
{
	m_path = strPath; 
	m_catalog = strCatalog;

	m_link_value.clear();
	m_link_list.ResetContent();

	m_link_list.SetTagImage(m_hTagImage);
	m_link_list.SetCodeImage(m_hCodeImage);
	m_link_list.SetBookImage(m_hBookImage);

	int nFind = m_path.ReverseFind('\\');
	auto& strTitle = m_path.Mid(nFind + 1);
	m_detail_title.SetWindowText(strTitle);

	m_detail_title.SetIcon(GetCatalogImage(m_catalog));
	m_detail_title.Invalidate();

	CString strFile = m_path + _T("\\描述.txt");
	CStdioFile file(strFile, CFile::modeReadWrite | CFile::typeText);

	UINT nSize = UINT(file.GetLength()) + 1;
	char* szText = m_text.GetBuffer();

	if ((UINT)m_text.GetLength() < nSize) {
		szText = m_text.GetBufferSetLength(nSize);
	}

	memset(szText, 0, nSize);
	file.Read(szText, nSize);

	file.Close();

	SetWindowTextA(m_detial_info.GetSafeHwnd(), szText);

	auto AddLink = [&](LPITEMIDLIST itemID) {
		TCHAR szPath[MAX_PATH] = { 0 };
		if (SHGetPathFromIDList(itemID, szPath)) {
			CString str(szPath);
			m_link_list.AddString(str);
			m_link_value.push_back(str);
		}
	};

	CString strFilter;
	m_link_filter.GetWindowText(strFilter);

	if (strFilter == _T("标签") || strFilter == _T("全部")) {
		SimpleEnumFolder(m_path + _T("\\关联\\标签"), &theShellManager, AddLink);
	}

	if (strFilter == _T("源码") || strFilter == _T("全部")) {
		SimpleEnumFolder(m_path + _T("\\关联\\源码"), &theShellManager, AddLink);
	}

	if (strFilter == _T("图书") || strFilter == _T("全部")) {
		SimpleEnumFolder(m_path + _T("\\关联\\图书"), &theShellManager, AddLink);
	}
}

void CDetailPage::LinkListEvent::OnDoubleClick()
{
	auto pThis = ((CDetailPage*)((BYTE*)this - offsetof(CDetailPage, m_link_list_event)));

	int nItem = pThis->m_link_list.GetCurSel();
	CString strPath = pThis->m_link_list.GetItemPath(nItem);

	CString strTargetPath;
	if (GetLinkFilePath(strTargetPath, strPath) && PathFileExists(strTargetPath)) {
		if (strTargetPath.Find(theApp.GetTagDir().GetString()) >= 0) {
			pThis->Refresh(_T("标签"), strTargetPath);
		} else if (strTargetPath.Find(theApp.GetCodeDir().GetString()) >= 0) {
			pThis->Refresh(_T("源码"), strTargetPath);
		} else if (strTargetPath.Find(theApp.GetBookDir().GetString()) >= 0) {
			pThis->Refresh(_T("图书"), strTargetPath);
		} else {
			// DO NOTHING
		}
	} else {
		if (pThis->MessageBox(_T("关联已失效，是否删除该关联？"), _T("关联失效"), MB_OKCANCEL) == IDOK) {
			DeleteFile(strPath);
			pThis->Refresh(pThis->m_catalog, pThis->m_path);
		}
	}
}

void CDetailPage::OnBnClickedAddLink()
{

}

void CDetailPage::OnBnClickedRemoveLink()
{
	int nItem = m_link_list.GetCurSel();
	CString strPath = m_link_list.GetItemPath(nItem);

	DeleteFile(strPath);
	Refresh(m_catalog, m_path);
}