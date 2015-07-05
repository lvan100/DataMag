#include "stdafx.h"
#include "TabTemplate.h"

IMPLEMENT_DYNAMIC(CTabTemplate, CTabCtrl)

CTabTemplate::CTabTemplate()
	: m_adpter(NULL)
{
}

CTabTemplate::~CTabTemplate()
{
}

BEGIN_MESSAGE_MAP(CTabTemplate, CTabCtrl)
	ON_WM_SIZE()
	ON_WM_CREATE()
	ON_WM_DESTROY()
	ON_WM_SETFOCUS()
	ON_NOTIFY_REFLECT(TCN_SELCHANGE, &CTabTemplate::OnTcnSelchange)
END_MESSAGE_MAP()

int CTabTemplate::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CTabCtrl::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	if (!Init())
	{
		return -1;
	}

	return 0;
}

void CTabTemplate::PreSubclassWindow()
{
	CTabCtrl::PreSubclassWindow();

	if (!Init())
	{
		ASSERT(FALSE);
	}
}

BOOL CTabTemplate::Init()
{
	SetItemSize(CSize(TAB_WIDTH,TAB_HEIGHT));

	if (m_adpter != nullptr)
	{
		UINT nCount = m_adpter->GetCount();
		for (UINT i = 0; i < nCount; i++)
		{
			InsertItem(i, m_adpter->GetItemText(i));

			auto pItem = m_adpter->GetItem(i, this);
			m_content.push_back(pItem);
		}
	}

	ResizeContent();

	return TRUE;
}

BOOL CTabTemplate::SelectTab(UINT nItem)
{
	if (nItem >= 0 && nItem < m_content.size())
	{
		for (auto iter = m_content.begin()
			; iter != m_content.end()
			; iter++)
		{
			auto pItem = (*iter);
			if (pItem != nullptr)
			{
				pItem->ShowWindow(SW_HIDE);
			}
		}

		auto pItem = m_content.at(nItem);
		if (pItem != nullptr)
		{
			pItem->ShowWindow(SW_SHOW);
		}

		SetCurSel(nItem);

		return TRUE;
	}

	return FALSE;
}

BOOL CTabTemplate::DeleteTab(UINT nItem)
{
	if (nItem >= 0 && nItem < m_content.size())
	{
		auto iter = m_content.begin() + nItem;

		auto pItem = (*iter);
		(*iter) = nullptr;

		if (pItem != nullptr)
		{
			pItem->DestroyWindow();
		}

		if (DeleteItem(nItem))
		{
			m_content.erase(iter);
			return TRUE;
		}
	}

	return FALSE;
}

void CTabTemplate::DeleteAllTab()
{
	for (auto iter = m_content.begin()
		; iter != m_content.end()
		; iter++)
	{
		auto pItem = (*iter);
		(*iter) = nullptr;

		if (pItem != nullptr)
		{
			pItem->DestroyWindow();
			delete pItem;
		}
	}

	m_content.clear();
}

void CTabTemplate::OnDestroy()
{
	CTabCtrl::OnDestroy();

	DeleteAllTab();
}

void CTabTemplate::ResizeContent()
{
	for (auto iter = m_content.begin()
		; iter != m_content.end()
		; iter++)
	{
		auto pItem = (*iter);
		if (pItem != nullptr)
		{
			int nItem = iter - m_content.begin();

			CRect rcItem;
			GetItemRect(nItem, rcItem);

			CRect rcClient;
			GetClientRect(rcClient);

			rcClient.top += rcItem.Height();

			rcClient.top += PEDDING_TOP;
			rcClient.left += PEDDING_LEFT;
			rcClient.right -= PEDDING_RIGHT;
			rcClient.bottom -= PEDDING_BOTTOM;

			pItem->MoveWindow(rcClient);
		}
	}
}

void CTabTemplate::OnSize(UINT nType, int cx, int cy)
{
	CTabCtrl::OnSize(nType, cx, cy);
	
	ResizeContent();
}

void CTabTemplate::OnTcnSelchange(NMHDR *pNMHDR, LRESULT *pResult)
{
	SelectTab(GetCurSel());

	*pResult = 0;
}

void CTabTemplate::OnSetFocus(CWnd* pOldWnd)
{
	CTabCtrl::OnSetFocus(pOldWnd);

	auto pItem = m_content.at(GetCurSel());
	if (pItem != nullptr) {
		pItem->SetFocus();
	}
}