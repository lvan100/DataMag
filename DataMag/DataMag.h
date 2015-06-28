#pragma once

#include "resource.h"

class CDataMagApp : public CWinApp
{
public:
	CDataMagApp();

public:
	virtual BOOL InitInstance();
};

extern CDataMagApp theApp;
extern CShellManager* afxShellManager;