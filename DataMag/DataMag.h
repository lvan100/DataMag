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
	 * ͼ�����Ŀ¼�������б�
	 */
	vector<DirChangeLinster> bookMagDirChangeListener;

public:
	/**
	 * ��ȡ��ǩ����Ŀ¼
	 */
	CString GetLabelMagDir();

	/**
	 * ���ñ�ǩ����Ŀ¼
	 */
	void SetLabelMagDir(CString dir);

	/**
	 * ���ñ�ǩ����Ŀ¼������
	 */
	void AddLabelMagDirChangeListener(DirChangeLinster listener){
		labelMagDirChangeListener.push_back(listener);
	}

protected:
	/**
	 * ��ǩ����Ŀ¼�������б�
	 */
	vector<DirChangeLinster> labelMagDirChangeListener;
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