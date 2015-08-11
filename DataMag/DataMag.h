#pragma once

#include "resource.h"

#include <vector>
#include <functional>
using namespace std;

/**
 * 定义目录监听器类型
 */
typedef function<void(CString)> DirChangeListener;

/**
 * 最近访问列表监听器
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

public:
	/**
	 * 获取源码目录
	 */
	CString GetCodeDir();

	/**
	 * 设置源码目录
	 */
	void SetCodeDir(CString dir);

	/**
	 * 添加源码目录监听器
	 */
	void AddCodeDirChangeListener(DirChangeListener listener) {
		codeDirChangeListener.push_back(listener);
	}

protected:
	/**
	 * 源码目录监听器列表
	 */
	vector<DirChangeListener> codeDirChangeListener;

public:
	/**
	 * 获取图书目录
	 */
	CString GetBookDir();

	/**
	 * 设置图书目录
	 */
	void SetBookDir(CString dir);

	/**
	 * 设置图书目录监听器
	 */
	void AddBookDirChangeListener(DirChangeListener listener){
		bookDirChangeListener.push_back(listener);
	}

protected:
	/**
	 * 图书目录监听器列表
	 */
	vector<DirChangeListener> bookDirChangeListener;

public:
	/**
	 * 获取标签目录
	 */
	CString GetTagDir();

	/**
	 * 设置标签目录
	 */
	void SetTagDir(CString dir);

	/**
	 * 设置标签目录监听器
	 */
	void AddTagDirChangeListener(DirChangeListener listener){
		tagDirChangeListener.push_back(listener);
	}

protected:
	/**
	 * 标签目录监听器列表
	 */
	vector<DirChangeListener> tagDirChangeListener;

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
	 * 设置最近访问列表监听器
	 */
	void AddRecentListChangeListener(RecentListChangeListener listener){
		recentListChangeListener.push_back(listener);
	}

protected:
	/**
	 * 最近访问列表
	 */
	vector<CString> recentFileList;

	/**
	 * 最近访问列表监听器列表
	 */
	vector<RecentListChangeListener> recentListChangeListener;
};

/**
 * 全局的应用程序对象
 */
extern CDataMagApp theApp;

/*
 * 全局的Shell管理器
 */
extern CShellManager theShellManager;