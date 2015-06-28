#pragma once

#include "resource.h"

#include <vector>
#include <functional>
using namespace std;

/**
 * 定义目录监听器类型
 */
typedef function<void(CString)> DirChangeLinster;

/**
 * 应用设置类
 */
class CSetting
{
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
	void AddCodeMagDirChangeListener(DirChangeLinster listener) {
		codeMagDirChangeListener.push_back(listener);
	}

protected:
	/**
	 * 项目管理目录监听器列表
	 */
	vector<DirChangeLinster> codeMagDirChangeListener;

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
	void AddBookMagDirChangeListener(DirChangeLinster listener){
		bookMagDirChangeListener.push_back(listener);
	}

protected:
	/**
	 * 项目图书目录监听器列表
	 */
	vector<DirChangeLinster> bookMagDirChangeListener;
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