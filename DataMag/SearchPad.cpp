#include "stdafx.h"
#include "DataMag.h"
#include "SearchPad.h"

IMPLEMENT_DYNAMIC(CSearchPad, CDialogEx)

CSearchPad::CSearchPad(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_SEARCHPAD, pParent)
{
}

CSearchPad::~CSearchPad()
{
}

void CSearchPad::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CSearchPad, CDialogEx)
END_MESSAGE_MAP()