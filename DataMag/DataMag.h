#pragma once

#include "resource.h"

#include <map>
#include <vector>
#include <functional>
using namespace std;

/**
 * ����Ŀ¼�仯������
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
	virtual int ExitInstance();

public:
	/**
	 * ��ȡ������ͼ��
	 */
	HICON GetSearchIcon();

protected:
	/**
	 * ������ͼ��
	 */
	HICON m_hSearchIcon;

public:
	/**
	 * ��ȡԴ��Ŀ¼
	 */
	CString GetCodeDir() {
		return GetSettingDirectory(_T("CodeDir"), _T("..\\Դ��"));
	}

	/**
	 * ����Դ��Ŀ¼
	 */
	void SetCodeDir(CString dir);

	/**
	 * ���Դ��Ŀ¼������
	 */
	void AddCodeDirChangeListener(CWnd* pWnd, DirChangeListener listener) {
		codeDirChangeListener.insert(make_pair(pWnd, listener));
	}

	/**
	 * ɾ��Դ��Ŀ¼������
	 */
	void DeleteCodeDirChangeListener(CWnd* pWnd){
		codeDirChangeListener.erase(pWnd);
	}

protected:
	/**
	 * Դ��Ŀ¼�������б�
	 */
	map<CWnd*, DirChangeListener> codeDirChangeListener;

public:
	/**
	 * ��ȡͼ��Ŀ¼
	 */
	CString GetBookDir() {
		return GetSettingDirectory(_T("BookDir"), _T("..\\ͼ��"));
	}

	/**
	 * ����ͼ��Ŀ¼
	 */
	void SetBookDir(CString dir);

	/**
	 * ����ͼ��Ŀ¼������
	 */
	void AddBookDirChangeListener(CWnd* pWnd, DirChangeListener listener){
		bookDirChangeListener.insert(make_pair(pWnd, listener));
	}

	/**
	 * ɾ��ͼ��Ŀ¼������
	 */
	void DeleteBookDirChangeListener(CWnd* pWnd){
		bookDirChangeListener.erase(pWnd);
	}

protected:
	/**
	 * ͼ��Ŀ¼�������б�
	 */
	map<CWnd*, DirChangeListener> bookDirChangeListener;

public:
	/**
	 * ��ȡ��ǩĿ¼
	 */
	CString GetTagDir() {
		return GetSettingDirectory(_T("TagDir"), _T("..\\��ǩ"));
	}

	/**
	 * ���ñ�ǩĿ¼
	 */
	void SetTagDir(CString dir);

	/**
	 * ���ñ�ǩĿ¼������
	 */
	void AddTagDirChangeListener(CWnd* pWnd, DirChangeListener listener){
		tagDirChangeListener.insert(make_pair(pWnd, listener));
	}

	/**
	 * ɾ����ǩĿ¼������
	 */
	void DeleteTagDirChangeListener(CWnd* pWnd){
		tagDirChangeListener.erase(pWnd);
	}

protected:
	/**
	 * ��ǩĿ¼�������б�
	 */
	map<CWnd*, DirChangeListener> tagDirChangeListener;

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
	 * �Ƴ���������б���
	 */
	void RemoveRecentFile(CString file);

	/**
	 * ������������б������
	 */
	void AddRecentListChangeListener(CWnd* pWnd, RecentListChangeListener listener){
		recentListChangeListener.insert(make_pair(pWnd, listener));
	}

	/**
	 * ɾ����������б������
	 */
	void DeleteRecentListChangeListener(CWnd* pWnd) {
		recentListChangeListener.erase(pWnd);
	}

protected:
	/**
	 * ��������б�
	 */
	vector<CString> recentFileList;

	/**
	 * ��������б�������б�
	 */
	map<CWnd*, RecentListChangeListener> recentListChangeListener;

protected:
	/**
	 * ��ȡ�ض����͵�����Ŀ¼
	 */
	CString GetSettingDirectory(CString strType, CString strDefaultPath);
};

/**
 * ȫ�ֵ�Ӧ�ó������
 */
extern CDataMagApp theApp;

/*
 * ȫ�ֵ�Shell������
 */
extern CShellManager theShellManager;