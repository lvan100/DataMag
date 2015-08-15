#include "stdafx.h"
#include "ItemInfoEdit.h"

IMPLEMENT_DYNAMIC(CItemInfoEdit, CRichEditCtrl)

CItemInfoEdit::CItemInfoEdit()
{
}

CItemInfoEdit::~CItemInfoEdit()
{
}

BEGIN_MESSAGE_MAP(CItemInfoEdit, CRichEditCtrl)
	ON_WM_CREATE()
	ON_NOTIFY_REFLECT(EN_LINK, &CItemInfoEdit::OnEnLink)
END_MESSAGE_MAP()

int CItemInfoEdit::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CRichEditCtrl::OnCreate(lpCreateStruct) == -1)
		return -1;

	Init();

	return 0;
}

void CItemInfoEdit::PreSubclassWindow()
{
	CRichEditCtrl::PreSubclassWindow();

	Init();
}

void CItemInfoEdit::Init()
{
	// ��������
	LimitText(1000);

	// �Զ���� URL ��ַ
	SetAutoURLDetect();

	CRect rcClient;
	GetClientRect(rcClient);

	// �����ı������λ��
	rcClient.DeflateRect(4,4,2,2);
	SetRect(rcClient);

	// �����ַ�ת��ģʽ
	SetWordWrapMode(WBF_WORDWRAP | WBF_WORDBREAK);

	// �����¼���Ӧ
	SetEventMask(GetEventMask() | ENM_LINK);
}

void CItemInfoEdit::OnEnLink(NMHDR *pNMHDR, LRESULT *pResult)
{
	ENLINK *pEnLink = reinterpret_cast<ENLINK *>(pNMHDR);

	if (pEnLink->msg == WM_LBUTTONDOWN)	
	{
		CString strURL;

		CHARRANGE chrg = pEnLink->chrg;
		GetTextRange(chrg.cpMin, chrg.cpMax, strURL);

		ShellExecute(NULL, _T( "open" ), strURL, NULL, NULL, SW_SHOWNORMAL);
	}

	*pResult = 0;
}