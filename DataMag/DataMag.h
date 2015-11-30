#pragma once

#include "resource.h"

#include <map>
#include <vector>
#include <functional>
using namespace std;

/**
 * 定义目录变化监听器
 */
typedef function<void(CString)> DirChangeListener;

/**
 * 定义访问列表监听器
 */
typedef function<void()> RecentListChangeListener;

/**
 * 应用程序类
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
	 * 获取搜索框图标
	 */
	HICON GetSearchIcon();

protected:
	/**
	 * 搜索框图标
	 */
	HICON m_hSearchIcon;

public:
	/**
	 * 获取源码目录
	 */
	CString GetCodeDir() {
		return GetSettingDirectory(_T("CodeDir"), _T("..\\源码"));
	}

	/**
	 * 设置源码目录
	 */
	void SetCodeDir(CString dir);

	/**
	 * 添加源码目录监听器
	 */
	void AddCodeDirChangeListener(CWnd* pWnd, DirChangeListener listener) {
		codeDirChangeListener.insert(make_pair(pWnd, listener));
	}

	/**
	 * 删除源码目录监听器
	 */
	void DeleteCodeDirChangeListener(CWnd* pWnd){
		codeDirChangeListener.erase(pWnd);
	}

protected:
	/**
	 * 源码目录监听器列表
	 */
	map<CWnd*, DirChangeListener> codeDirChangeListener;

public:
	/**
	 * 获取图书目录
	 */
	CString GetBookDir() {
		return GetSettingDirectory(_T("BookDir"), _T("..\\图书"));
	}

	/**
	 * 设置图书目录
	 */
	void SetBookDir(CString dir);

	/**
	 * 设置图书目录监听器
	 */
	void AddBookDirChangeListener(CWnd* pWnd, DirChangeListener listener){
		bookDirChangeListener.insert(make_pair(pWnd, listener));
	}

	/**
	 * 删除图书目录监听器
	 */
	void DeleteBookDirChangeListener(CWnd* pWnd){
		bookDirChangeListener.erase(pWnd);
	}

protected:
	/**
	 * 图书目录监听器列表
	 */
	map<CWnd*, DirChangeListener> bookDirChangeListener;

public:
	/**
	 * 获取标签目录
	 */
	CString GetTagDir() {
		return GetSettingDirectory(_T("TagDir"), _T("..\\标签"));
	}

	/**
	 * 设置标签目录
	 */
	void SetTagDir(CString dir);

	/**
	 * 设置标签目录监听器
	 */
	void AddTagDirChangeListener(CWnd* pWnd, DirChangeListener listener){
		tagDirChangeListener.insert(make_pair(pWnd, listener));
	}

	/**
	 * 删除标签目录监听器
	 */
	void DeleteTagDirChangeListener(CWnd* pWnd){
		tagDirChangeListener.erase(pWnd);
	}

protected:
	/**
	 * 标签目录监听器列表
	 */
	map<CWnd*, DirChangeListener> tagDirChangeListener;

public:
	/**
	 * 最近访问列表的最大数量
	 */
	enum { MaxRecentFileCount = 8 };

	/**
	 * 获取最近访问文件列表
	 */
	const vector<CString>& GetRecentFileList();

	/**
	 * 设置最近访问文件
	 */
	void SetRecentFile(CString file);

	/**
	 * 移除最近访问列表项
	 */
	void RemoveRecentFile(CString file);

	/**
	 * 设置最近访问列表监听器
	 */
	void AddRecentListChangeListener(CWnd* pWnd, RecentListChangeListener listener){
		recentListChangeListener.insert(make_pair(pWnd, listener));
	}

	/**
	 * 删除最近访问列表监听器
	 */
	void DeleteRecentListChangeListener(CWnd* pWnd) {
		recentListChangeListener.erase(pWnd);
	}

protected:
	/**
	 * 最近访问列表
	 */
	vector<CString> recentFileList;

	/**
	 * 最近访问列表监听器列表
	 */
	map<CWnd*, RecentListChangeListener> recentListChangeListener;

protected:
	/**
	 * 获取特定类型的设置目录
	 */
	CString GetSettingDirectory(CString strType, CString strDefaultPath);
};

/**
 * 全局的应用程序对象
 */
extern CDataMagApp theApp;

/*
 * 全局的Shell管理器
 */
extern CShellManager theShellManager;