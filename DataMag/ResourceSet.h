#pragma once

#include <map>
using namespace std;

/**
 * ��Դ����
 */
class CResourceSet
{
public:
	CResourceSet();
	~CResourceSet();

	/**
	 * ��ȡϵͳ���ű���
	 */
	float GetScreenScale();

	/**
	 * ���������С��ȡԤ������
	 */
	CFont* GetFontBySize(float size);

protected:
	/**
	 * Ԥ�����弯��
	 */
	map<int, CFont*> m_font_set;
};

/**
 * ȫ��Ψһ����Դ����
 */
extern CResourceSet theResourceSet;