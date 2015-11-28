#include "stdafx.h"
#include "ItemInfoEdit.h"

IMPLEMENT_DYNAMIC(CItemInfoEdit, CRichEditCtrl)

CItemInfoEdit::CItemInfoEdit()
	: m_pHiliteBorder(nullptr)
{
}

CItemInfoEdit::~CItemInfoEdit()
{
}

BEGIN_MESSAGE_MAP(CItemInfoEdit, CRichEditCtrl)
	ON_WM_CREATE()
	ON_WM_DESTROY()
	ON_WM_SETFOCUS()
	ON_WM_KILLFOCUS()
	ON_NOTIFY_REFLECT(EN_LINK, &CItemInfoEdit::OnEnLink)
	ON_CONTROL_REFLECT(EN_CHANGE, &CItemInfoEdit::OnEnChange)
END_MESSAGE_MAP()

int CItemInfoEdit::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CRichEditCtrl::OnCreate(lpCreateStruct) == -1)
		return -1;

	if (!InitBorder()) {
		return -1;
	}

	InitCtrl();

	return 0;
}

void CItemInfoEdit::PreSubclassWindow()
{
	CRichEditCtrl::PreSubclassWindow();

	if (!InitBorder()) {
		ASSERT(FALSE);
	}

	InitCtrl();
}

BOOL CItemInfoEdit::InitBorder() 
{
	ASSERT(m_pHiliteBorder == nullptr);

	CRect rcBorder;
	GetWindowRect(rcBorder);
	GetParent()->ScreenToClient(rcBorder);

	m_pHiliteBorder = new CHiliteBorder();

	DWORD dwStyle = WS_VISIBLE | WS_CHILD | SS_OWNERDRAW;
	if (!m_pHiliteBorder->Create(nullptr, dwStyle, rcBorder, GetParent())) {
		return FALSE;
	}

	rcBorder.DeflateRect(1,1,1,1);
	MoveWindow(rcBorder);

	return TRUE;
}

void CItemInfoEdit::InitCtrl()
{
	// 限制字数
	LimitText(1000);

	// 自动检测 URL 地址
	SetAutoURLDetect();

	CRect rcClient;
	GetClientRect(rcClient);

	// 设置文本输入框位置
	rcClient.DeflateRect(4,4,2,2);
	SetRect(rcClient);

	// 设置字符转换模式
	SetWordWrapMode(WBF_WORDWRAP | WBF_WORDBREAK);

	// 设置事件响应
	SetEventMask(GetEventMask() | ENM_LINK | ENM_CHANGE);
}

void CItemInfoEdit::OnEnLink(NMHDR *pNMHDR, LRESULT *pResult)
{
	ENLINK *pEnLink = reinterpret_cast<ENLINK *>(pNMHDR);

	if (pEnLink->msg == WM_LBUTTONDOWN)	
	{
		CString strURL;

		CHARRANGE chrg = pEnLink->chrg;
		GetTextRange(chrg.cpMin, chrg.cpMax, strURL);

		ShellExecute(nullptr, _T( "open" ), strURL, nullptr, nullptr, SW_SHOWNORMAL);
	}

	*pResult = 0;
}

void CItemInfoEdit::OnSetFocus(CWnd* pOldWnd)
{
	CRichEditCtrl::OnSetFocus(pOldWnd);

	if (m_pHiliteBorder != nullptr) {
		m_pHiliteBorder->Hilite(TRUE);
	}
}

void CItemInfoEdit::OnKillFocus(CWnd* pNewWnd)
{
	CRichEditCtrl::OnKillFocus(pNewWnd);

	if (m_pHiliteBorder != nullptr) {
		m_pHiliteBorder->Hilite(FALSE);
	}
}

void CItemInfoEdit::OnDestroy()
{
	CRichEditCtrl::OnDestroy();

	if (m_pHiliteBorder != nullptr) {
		m_pHiliteBorder->DestroyWindow();
		delete m_pHiliteBorder;
	}
}

void CItemInfoEdit::OnEnChange()
{
	if (m_change_listener.target<void()>() != nullptr) {
		m_change_listener();
	}
}