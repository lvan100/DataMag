#include "StdAfx.h"
#include "DDXControl.h"

void MFC_DDX_Control(CDataExchange* pDX, int nIDC, CWnd& rControl)
{
	HWND hWndCtrl = nullptr;
	pDX->m_pDlgWnd->GetDlgItem(nIDC, &hWndCtrl);   

	TCHAR lpszClassName [MAX_CLASS_NAME + 1];
	::GetClassName(hWndCtrl, lpszClassName, MAX_CLASS_NAME);

	if (_tcscmp(lpszClassName, _T("MFCButton")) == 0)
	{
		CMFCButton *pMFCButton = (CMFCButton *)pDX->m_pDlgWnd->GetDlgItem(nIDC);
		CToolTipCtrl& pToolTipCtrl = pMFCButton->GetToolTipCtrl();
		if (pToolTipCtrl.m_hWnd != nullptr) {
			CToolTipCtrl* pCtrl = &pToolTipCtrl;
			CTooltipManager::DeleteToolTip(pCtrl);
		}
	}

	DDX_Control(pDX, nIDC, rControl);
}