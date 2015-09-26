#pragma once

#include <vector>
#include <algorithm>
#include <functional>
using namespace std;

#include "HilitButton.h"

/**
 * ��������İ�ť����¼�
 */
typedef function<void(CMFCButton*)> ReflactClickEvent;

/**
 * ����·���ؼ����Ӱ�ť
 */
class CPathLinkButton : public CHilitButton
{
	DECLARE_DYNAMIC(CPathLinkButton)

public:
	CPathLinkButton();
	virtual ~CPathLinkButton();

	/**
	 * ��ť����¼�
	 */
	ReflactClickEvent clickEvent;

	DECLARE_MESSAGE_MAP()
protected:
	afx_msg void OnBnDoubleclicked();
};

/**
 * ·�������仯���¼�
 */
typedef function<void(CString)> PathChangedEvent;

/**
 * ��ǩ·���ؼ�
 */
class CTagPathLink : public CStatic
{
	DECLARE_DYNAMIC(CTagPathLink)

public:
	CTagPathLink();
	virtual ~CTagPathLink();

	/**
	 * ��������·��
	 */
	void SetLinkPath(CString strPath);

	/**
	 * �Ӱ�ť�¼���Ӧ
	 */
	void OnPathBtnClicked(CMFCButton* pBtn);

	/**
	 * ��˫���Ӱ�ť��ʱ�򴥷����¼�
	 */
	PathChangedEvent mPathChangedEvent;

protected:
	/**
	 * ���������Ӱ�ť
	 */
	void ClearPathLinkButtons();

protected:
	/**
	 * ·����ť
	 */
	vector<CWnd*> m_path_btns;

protected:
	virtual BOOL OnInitControl();
	virtual void PreSubclassWindow();

	DECLARE_MESSAGE_MAP()
protected:
	afx_msg void OnDestroy();
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
};