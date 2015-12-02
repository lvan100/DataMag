#include "stdafx.h"
#include "FileOp.h"
#include "DataMag.h"
#include "DetailPage.h"
#include "ResourceSet.h"

IMPLEMENT_DYNAMIC(CDetailPage, CDialogEx)

CDetailPage::CDetailPage(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_DETAILPAGE, pParent)
	, m_link_list(&theShellManager)
{
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
END_MESSAGE_MAP()

HICON CDetailPage::GetCatalogImage(CString catalog)
{
	if (catalog == _T("Դ��")) {
		return m_hCodeImage;
	}
	else if (catalog == _T("ͼ��")) {
		return m_hBookImage;
	}
	else if (catalog == _T("��ǩ")) {
		return m_hTagImage;
	}
	else {
		return nullptr;
	}
}

BOOL CDetailPage::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	m_detail_title.SetFont(theResourceSet.GetFontBySize(13));
	m_detial_info.SetFont(theResourceSet.GetFontBySize(11));

	HICON hAddIcon = (HICON)LoadImage(AfxGetInstanceHandle()
		, MAKEINTRESOURCE(IDI_ADD_BOOK)
		, IMAGE_ICON, 0, 0, 0);

	HICON hRemoveIcon = (HICON)LoadImage(AfxGetInstanceHandle()
		, MAKEINTRESOURCE(IDI_REMOVE_LINK)
		, IMAGE_ICON, 0, 0, 0);

	m_add_link.SetImage(hAddIcon);
	m_remove_link.SetImage(hRemoveIcon);

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
* ö���ļ������ݣ���������������
*/
STATIC int SimpleEnumFolder(LPCTSTR lpszPath	// �ļ���·��
	, CShellManager* pShellManager				// Shell������
	, function<void(LPITEMIDLIST)> filter)		// ����������
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

				if (filter) { /* VS2015 �������ԣ������� operator bool() ���� */
					LPITEMIDLIST itemID = pShellManager->ConcatenateItem(info.pidlRel, pidlTemp);
					filter(itemID); /* ͨ���ص��ɿͻ������о��� */
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

void CDetailPage::Prepare()
{
	CString strTitle = m_path.Mid(m_path.ReverseFind('\\') + 1);
	m_detail_title.SetWindowText(strTitle);

	m_detail_title.SetIcon(GetCatalogImage(m_catalog));

	CString strFile = m_path + _T("\\����.txt");
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

	SimpleEnumFolder(m_path + _T("\\����\\��ǩ"), &theShellManager, [&](LPITEMIDLIST itemID) {
		TCHAR szPath[MAX_PATH] = { 0 };
		if (SHGetPathFromIDList(itemID, szPath)) {
			m_link_list.AddString(szPath);
		}
	});

	SimpleEnumFolder(m_path + _T("\\����\\Դ��"), &theShellManager, [&](LPITEMIDLIST itemID) {
		TCHAR szPath[MAX_PATH] = { 0 };
		if (SHGetPathFromIDList(itemID, szPath)) {
			m_link_list.AddString(szPath);
		}
	});

	SimpleEnumFolder(m_path + _T("\\����\\ͼ��"), &theShellManager, [&](LPITEMIDLIST itemID) {
		TCHAR szPath[MAX_PATH] = { 0 };
		if (SHGetPathFromIDList(itemID, szPath)) {
			m_link_list.AddString(szPath);
		}
	});
}