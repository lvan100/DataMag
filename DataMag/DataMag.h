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
 * 应用设置类
 */
class CSetting
{
public:
	CSetting();

public:
	/**
	 * 获取项目管理目录
	 */
	CString GetCodeMagDir();

	/**
	 * 设置项目管理目录
	 */
	void SetCodeMagDir(CString dir);

	/**
	 * 设置项目管理目录监听器
	 */
	void AddCodeMagDirChangeListener(DirChangeListener listener) {
		codeMagDirChangeListener.push_back(listener);
	}

protected:
	/**
	 * 项目管理目录监听器列表
	 */
	vector<DirChangeListener> codeMagDirChangeListener;

public:
	/**
	 * 获取图书管理目录
	 */
	CString GetBookMagDir();

	/**
	 * 设置图书管理目录
	 */
	void SetBookMagDir(CString dir);

	/**
	 * 设置图书管理目录监听器
	 */
	void AddBookMagDirChangeListener(DirChangeListener listener){
		bookMagDirChangeListener.push_back(listener);
	}

protected:
	/**
	 * 图书管理目录监听器列表
	 */
	vector<DirChangeListener> bookMagDirChangeListener;

public:
	/**
	 * 获取标签管理目录
	 */
	CString GetLabelMagDir();

	/**
	 * 设置标签管理目录
	 */
	void SetLabelMagDir(CString dir);

	/**
	 * 设置标签管理目录监听器
	 */
	void AddLabelMagDirChangeListener(DirChangeListener listener){
		labelMagDirChangeListener.push_back(listener);
	}

protected:
	/**
	 * 标签管理目录监听器列表
	 */
	vector<DirChangeListener> labelMagDirChangeListener;

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
 * 全局的应用设置对象
 */
extern CSetting theSetting;

/**
 * 应用程序类
 */
class CDataMagApp : public CWinApp
{
public:
	CDataMagApp();

public:
	virtual BOOL InitInstance();
};

/**
 * 全局的应用程序对象
 */
extern CDataMagApp theApp;

/*
 * 全局的Shell管理器
 */
extern CShellManager theShellManager;