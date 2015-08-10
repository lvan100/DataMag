#pragma once

#include "resource.h"

#include <vector>
#include <functional>
using namespace std;

/**
 * ����Ŀ¼����������
 */
typedef function<void(CString)> DirChangeListener;

/**
 * ��������б������
 */
typedef function<void()> RecentListChangeListener;

/**
 * Ӧ��������
 */
class CSetting
{
public:
	CSetting();

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
	void AddCodeMagDirChangeListener(DirChangeListener listener) {
		codeMagDirChangeListener.push_back(listener);
	}

protected:
	/**
	 * ��Ŀ����Ŀ¼�������б�
	 */
	vector<DirChangeListener> codeMagDirChangeListener;

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
	void AddBookMagDirChangeListener(DirChangeListener listener){
		bookMagDirChangeListener.push_back(listener);
	}

protected:
	/**
	 * ͼ�����Ŀ¼�������б�
	 */
	vector<DirChangeListener> bookMagDirChangeListener;

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
	void AddLabelMagDirChangeListener(DirChangeListener listener){
		labelMagDirChangeListener.push_back(listener);
	}

protected:
	/**
	 * ��ǩ����Ŀ¼�������б�
	 */
	vector<DirChangeListener> labelMagDirChangeListener;

public:
	/**
	 * ��������б���������
	 */
	enum { MaxRecentFileCount = 8 };

	/**
	 * ��ȡ��������ļ��б�
	 */
	const vector<CString>& GetRecentFileList();

	/**
	 * ������������ļ�
	 */
	void SetRecentFile(CString file);

	/**
	 * ������������б������
	 */
	void AddRecentListChangeListener(RecentListChangeListener listener){
		recentListChangeListener.push_back(listener);
	}

protected:
	/**
	 * ��������б�
	 */
	vector<CString> recentFileList;

	/**
	 * ��������б�������б�
	 */
	vector<RecentListChangeListener> recentListChangeListener;
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