#pragma once

#include "resource.h"

#include <vector>
#include <functional>
using namespace std;

/**
 * ����Ŀ¼����������
 */
typedef function<void(CString)> DirChangeLinster;

/**
 * Ӧ��������
 */
class CSetting
{
public:
	/**
	 * ��ȡ��Ŀ����Ŀ¼
	 */
	CString GetCodeMagDir();

	/**
	 * ������Ŀ����Ŀ¼
	 */
	void SetCodeMagDir(CString dir);

	/**
	 * ������Ŀ����Ŀ¼������
	 */
	void AddCodeMagDirChangeListener(DirChangeLinster listener) {
		codeMagDirChangeListener.push_back(listener);
	}

protected:
	/**
	 * ��Ŀ����Ŀ¼�������б�
	 */
	vector<DirChangeLinster> codeMagDirChangeListener;

public:
	/**
	 * ��ȡͼ�����Ŀ¼
	 */
	CString GetBookMagDir();

	/**
	 * ����ͼ�����Ŀ¼
	 */
	void SetBookMagDir(CString dir);

	/**
	 * ����ͼ�����Ŀ¼������
	 */
	void AddBookMagDirChangeListener(DirChangeLinster listener){
		bookMagDirChangeListener.push_back(listener);
	}

protected:
	/**
	 * ��Ŀͼ��Ŀ¼�������б�
	 */
	vector<DirChangeLinster> bookMagDirChangeListener;
};

/**
 * ȫ�ֵ�Ӧ�����ö���
 */
extern CSetting theSetting;

/**
 * Ӧ�ó�����
 */
class CDataMagApp : public CWinApp
{
public:
	CDataMagApp();

public:
	virtual BOOL InitInstance();
};

/**
 * ȫ�ֵ�Ӧ�ó������
 */
extern CDataMagApp theApp;

/*
 * ȫ�ֵ�Shell������
 */
extern CShellManager theShellManager;