#pragma once

#include "FileNode.h"

class WindowsFileNode : public FileNode
{
public:
	WindowsFileNode(CShellManager* pShellManager, wstring path);
	virtual ~WindowsFileNode(void);

	/*
	 * 获取节点名称
	 */
	virtual wstring GetName() {
		return PathFindFileName(_path.c_str());
	}

	/**
	 * 枚举子节点列表
	 */
	virtual bool Enum(FileEnumConfig config, function<bool(shared_ptr<FileNode>)> filter);

protected:
	/**
	 * 关联的 Shell 管理器接口
	 */
	CShellManager* m_pShellManager;
};