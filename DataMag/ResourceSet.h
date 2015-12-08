#pragma once

#include <map>
using namespace std;

/**
 * 资源集合
 */
class CResourceSet
{
public:
	CResourceSet();
	~CResourceSet();

	/**
	 * 获取系统缩放比例
	 */
	float GetScreenScale();

	/**
	 * 根据字体大小获取预设字体
	 */
	CFont* GetFontBySize(float size);

protected:
	/**
	 * 预设字体集合
	 */
	map<int, CFont*> m_font_set;
};

/**
 * 全局唯一的资源集合
 */
extern CResourceSet theResourceSet;