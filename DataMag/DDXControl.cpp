#include "StdAfx.h"
#include "DDXControl.h"

void MFC_DDX_Control(CDataExchange* pDX, int nIDC, CWnd& rControl)
{
	CWnd* pOrgWnd = pDX->m_pDlgWnd->GetDlgItem(nIDC);   

	TCHAR lpszClassName [MAX_CLASS_NAME + 1] = { 0 };
	::GetClassName(pOrgWnd->m_hWnd, lpszClassName, MAX_CLASS_NAME);

	if (_tcscmp(lpszClassName, _T("MFCButton")) == 0)
	{
		CMFCButton *pMFCButton = (CMFCButton *) pOrgWnd;
		auto& pToolTipCtrl = pMFCButton->GetToolTipCtrl();

		if (pToolTipCtrl.m_hWnd != nullptr) {
			CToolTipCtrl* pCtrl = &pToolTipCtrl;
			CTooltipManager::DeleteToolTip(pCtrl);
		}
	}

	DDX_Control(pDX, nIDC, rControl);
}