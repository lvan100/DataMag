#include "stdafx.h"
#include "TagPathLink.h"

/************************************************************************/
/*                        链接路径子按钮                                */
/************************************************************************/

IMPLEMENT_DYNAMIC(CPathLinkButton, CHilitButton)

CPathLinkButton::CPathLinkButton()
{
	m_nFlatStyle = CMFCButton::BUTTONSTYLE_NOBORDERS;
}

CPathLinkButton::~CPathLinkButton()
{
}

BEGIN_MESSAGE_MAP(CPathLinkButton, CHilitButton)
	ON_CONTROL_REFLECT(BN_DOUBLECLICKED, &CPathLinkButton::OnBnDoubleclicked)
END_MESSAGE_MAP()

void CPathLinkButton::OnBnDoubleclicked()
{
	if (clickEvent.target<void(CMFCButton*)>() != nullptr) {
		clickEvent(this);
	}
}

/************************************************************************/
/*                          路径链接控件                                */
/************************************************************************/

IMPLEMENT_DYNAMIC(CTagPathLink, CStatic)

CTagPathLink::CTagPathLink()
{
}

CTagPathLink::~CTagPathLink()
{
}

BEGIN_MESSAGE_MAP(CTagPathLink, CStatic)
	ON_WM_CREATE()
	ON_WM_DESTROY()
END_MESSAGE_MAP()

int CTagPathLink::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CStatic::OnCreate(lpCreateStruct) == -1) {
		return -1;
	}

	if (!OnInitControl()) {
		return -1;
	}

	return 0;
}

void CTagPathLink::PreSubclassWindow()
{
	CStatic::PreSubclassWindow();

	if (!OnInitControl()) {
		ASSERT(FALSE);
	}
}

BOOL CTagPathLink::OnInitControl()
{
	return TRUE;
}

void CTagPathLink::ClearPathLinkButtons()
{
	auto iter = m_path_btns.begin();
	for (; iter != m_path_btns.end(); iter++) {
		(*iter)->DestroyWindow();
		delete (*iter);
	}

	m_path_btns.clear();
}

void CTagPathLink::OnDestroy()
{
	ClearPathLinkButtons();
	CStatic::OnDestroy();
}

static vector<CString> SplitString(CString source, TCHAR ch)
{
	vector<CString> splits;
	int lastFind = 0;
	do {
		int find = source.Find(ch, lastFind);
		if (find == -1){
			int count = source.GetLength() - lastFind;
			CString str = source.Right(count);
			splits.push_back(str);
			break;
		} else {
			CString str = source.Mid(lastFind, find - lastFind);
			splits.push_back(str);
			lastFind = find + 1;
		}
	} while (true);
	return splits;
}

void CTagPathLink::SetLinkPath(CString strPath)
{
	if (m_path_btns.size() > 0) {
		ClearPathLinkButtons();
	}

	static const int btnPedding = 8;

	CRect rcClient;
	GetClientRect(rcClient);

	CDC* pDC = GetDC();
	CFont* pOldFont = (CFont*) pDC->SelectStockObject(DEFAULT_GUI_FONT);

	static CFont btnFont;

	if (btnFont.GetSafeHandle() == nullptr) {
		btnFont.CreateStockObject(DEFAULT_GUI_FONT);
	}

	int lastRight = 0;
	auto splits = SplitString(strPath, '>');
	for (auto iter = splits.begin(); iter != splits.end(); iter++) {

		CSize size = pDC->GetOutputTextExtent((*iter)) + CSize(0,8);
		int top = (rcClient.Height() - size.cy) / 2 + rcClient.top;

		CRect rcSplitBtn(lastRight, top, lastRight + 12, top + size.cy);
		CRect rcBtn(rcSplitBtn.right, top, btnPedding + rcSplitBtn.right + size.cx, top + size.cy);

		CStatic* split_btn = new CStatic();
		split_btn->Create(_T(">"), SS_CENTER | SS_CENTERIMAGE | WS_CHILD | WS_VISIBLE, rcSplitBtn, this, -1);
		m_path_btns.push_back(split_btn);
		split_btn->SetFont(&btnFont);

		CPathLinkButton* btn = new CPathLinkButton();
		btn->Create((*iter), WS_CHILD | WS_VISIBLE | BS_OWNERDRAW, rcBtn, this, -1);
		btn->clickEvent = bind(&CTagPathLink::OnPathBtnClicked, this, placeholders::_1);
		m_path_btns.push_back(btn);
		btn->SetFont(&btnFont);

		lastRight = rcBtn.right;
	}

	pDC->SelectObject(pOldFont);
	ReleaseDC(pDC);
}

void CTagPathLink::OnPathBtnClicked(CMFCButton* pBtn)
{
	CString strLinkPath;

	auto iter = m_path_btns.begin();
	for (; iter != m_path_btns.end(); iter++) {
		CString strPath;
		(*iter)->GetWindowText(strPath);
		strLinkPath += strPath;
	}

	if (mPathChangedEvent.target<void(CString)>() != nullptr) {
		mPathChangedEvent(strLinkPath);
	}
}