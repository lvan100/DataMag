#pragma once

#include <string>
#include <vector>
#include <memory>
#include <functional>
using namespace std;

/**
 * �ļ��ڵ������
 */
enum FileNodeType
{
	Dir,
	File,
	Link,
};

/*
 * ö�ٵ��ļ�����
 */
enum FileEnumConfig
{
	Include_Children = 0x10,
	Include_Folder = 0x20,
	Include_File = 0x40,
};

/**
 * �ļ��ڵ�
 */
class FileNode
{
public:
	FileNode(wstring path) : _path(path) {}
	virtual ~FileNode() {}
	
	/*
	 * ��ȡ�ڵ�����
	 */
	virtual wstring GetName() = 0;

	/**
	 * ��ȡ�ڵ�·��
	 */
	wstring GetPath() { return _path; }

	/**
	 * �Ƿ����ļ���
	 */
	bool IsDir() { return _type == Dir; }

	/**
	 * �Ƿ����ļ�
	 */
	bool IsFile() { return _type == File; }

	/**
	 * �Ƿ�������
	 */
	bool IsLink() { return _type == Link; }

	/**
	 * ��ȡ��ǰ�ڵ���ӽڵ��б�
	 */
	vector<shared_ptr<FileNode>>& GetNodeList() { return _list; }

	/**
	 * ö���ӽڵ��б�
	 */
	virtual bool Enum(FileEnumConfig config, function<bool(shared_ptr<FileNode>)> filter) = 0;

protected:
	/**
	 * �ڵ�·��
	 */
	wstring _path;

	/**
	 * ��ǰ�ڵ�����
	 */
	FileNodeType _type;

	/**
	 * ��ǰ�ڵ���ӽڵ��б�
	 */
	vector<shared_ptr<FileNode>> _list;
};