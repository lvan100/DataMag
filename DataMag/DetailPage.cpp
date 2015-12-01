#include "stdafx.h"
#include "DataMag.h"
#include "DetailPage.h"
#include "ResourceSet.h"

IMPLEMENT_DYNAMIC(CDetailPage, CDialogEx)

CDetailPage::CDetailPage(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_DETAILPAGE, pParent)
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
	if (catalog == _T("‘¥¬Î")) {
		return m_hCodeImage;
	}
	else if (catalog == _T("Õº È")) {
		return m_hBookImage;
	}
	else if (catalog == _T("±Í«©")) {
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

void CDetailPage::Prepare()
{
	CString strTitle = m_path.Mid(m_path.ReverseFind('\\') + 1);
	m_detail_title.SetWindowText(strTitle);

	m_detail_title.SetIcon(GetCatalogImage(m_catalog));

	CString strFile = m_path + _T("\\√Ë ˆ.txt");
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



}