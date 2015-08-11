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
 * Ӧ�ó�����
 */
class CDataMagApp : public CWinApp
{
public:
	CDataMagApp();

public:
	virtual BOOL InitInstance();

public:
	/**
	 * ��ȡԴ��Ŀ¼
	 */
	CString GetCodeDir();

	/**
	 * ����Դ��Ŀ¼
	 */
	void SetCodeDir(CString dir);

	/**
	 * ���Դ��Ŀ¼������
	 */
	void AddCodeDirChangeListener(DirChangeListener listener) {
		codeDirChangeListener.push_back(listener);
	}

protected:
	/**
	 * Դ��Ŀ¼�������б�
	 */
	vector<DirChangeListener> codeDirChangeListener;

public:
	/**
	 * ��ȡͼ��Ŀ¼
	 */
	CString GetBookDir();

	/**
	 * ����ͼ��Ŀ¼
	 */
	void SetBookDir(CString dir);

	/**
	 * ����ͼ��Ŀ¼������
	 */
	void AddBookDirChangeListener(DirChangeListener listener){
		bookDirChangeListener.push_back(listener);
	}

protected:
	/**
	 * ͼ��Ŀ¼�������б�
	 */
	vector<DirChangeListener> bookDirChangeListener;

public:
	/**
	 * ��ȡ��ǩĿ¼
	 */
	CString GetTagDir();

	/**
	 * ���ñ�ǩĿ¼
	 */
	void SetTagDir(CString dir);

	/**
	 * ���ñ�ǩĿ¼������
	 */
	void AddTagDirChangeListener(DirChangeListener listener){
		tagDirChangeListener.push_back(listener);
	}

protected:
	/**
	 * ��ǩĿ¼�������б�
	 */
	vector<DirChangeListener> tagDirChangeListener;

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
 * ȫ�ֵ�Ӧ�ó������
 */
extern CDataMagApp theApp;

/*
 * ȫ�ֵ�Shell������
 */
extern CShellManager theShellManager;