#pragma once

#include <string>
#include <vector>
#include <memory>
#include <functional>
using namespace std;

/**
 * 文件节点的类型
 */
enum FileNodeType
{
	Dir,
	File,
	Link,
};

/*
 * 枚举的文件类型
 */
enum FileEnumConfig
{
	Include_Children = 0x10,
	Include_Folder = 0x20,
	Include_File = 0x40,
};

/**
 * 文件节点
 */
class FileNode
{
public:
	FileNode(wstring path) : _path(path) {}
	virtual ~FileNode() {}
	
	/*
	 * 获取节点名称
	 */
	virtual wstring GetName() = 0;

	/**
	 * 获取节点路径
	 */
	wstring GetPath() { return _path; }

	/**
	 * 是否是文件夹
	 */
	bool IsDir() { return _type == Dir; }

	/**
	 * 是否是文件
	 */
	bool IsFile() { return _type == File; }

	/**
	 * 是否是链接
	 */
	bool IsLink() { return _type == Link; }

	/**
	 * 获取当前节点的子节点列表
	 */
	vector<shared_ptr<FileNode>>& GetNodeList() { return _list; }

	/**
	 * 枚举子节点列表
	 */
	virtual bool Enum(FileEnumConfig config, function<bool(shared_ptr<FileNode>)> filter) = 0;

protected:
	/**
	 * 节点路径
	 */
	wstring _path;

	/**
	 * 当前节点类型
	 */
	FileNodeType _type;

	/**
	 * 当前节点的子节点列表
	 */
	vector<shared_ptr<FileNode>> _list;
};