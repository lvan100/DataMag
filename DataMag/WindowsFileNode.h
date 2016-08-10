#pragma once

#include "FileNode.h"

class WindowsFileNode : public FileNode
{
public:
	WindowsFileNode(CShellManager* pShellManager, wstring path);
	virtual ~WindowsFileNode(void);

	/*
	 * ��ȡ�ڵ�����
	 */
	virtual wstring GetName() {
		return PathFindFileName(_path.c_str());
	}

	/**
	 * ö���ӽڵ��б�
	 */
	virtual bool Enum(FileEnumConfig config, function<bool(shared_ptr<FileNode>)> filter);

protected:
	/**
	 * ������ Shell �������ӿ�
	 */
	CShellManager* m_pShellManager;
};