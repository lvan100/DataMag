#pragma once

/**
 * Shell�б��¼��ӿ�
 */
class ShellListEvent
{
public:
	/**
	 * ��ʼ���б��ؼ�
	 */
	virtual void InitShellList() = 0;

	/**
	 * �б���˫���¼�
	 */
	virtual void OnDoubleClick() = 0;

	/**
	 * �б�����仯
	 */
	virtual void OnSelectChanged() = 0;
};

/**
 * Shell�б��¼��ӿ�������
 */
class CShellListEventAdapter : public ShellListEvent{

public:
	/**
	 * ��ʼ���б��ؼ�
	 */
	virtual void InitShellList() {}
	
	/**
	 * �б���˫���¼�
	 */
	virtual void OnDoubleClick() {}

	/**
	 * �б�����仯
	 */
	virtual void OnSelectChanged() {}
};

/**
 * Shell�б��ؼ�
 */
class CShellListCtrl : public CMFCShellListCtrl
{
	DECLARE_DYNAMIC(CShellListCtrl)

public:
	/**
	 * �����б��¼�ʵ�ֽӿ�
	 */
	void SetListEvent(ShellListEvent* event){
		m_event = event;
	}

protected:
	/**
	 * �ͻ���ʵ�ֵ��¼�����
	 */
	ShellListEvent* m_event;

public:
	CShellListCtrl();
	virtual ~CShellListCtrl();

	/**
	 * ��ȡ�б���ĵ�ַ
	 *
	 * @param iItem
	 *        �б��������
	 * @return �б���ĵ�ַ
	 */
	CString GetItemPath(int iItem){
		CString strPath;
		CMFCShellListCtrl::GetItemPath(strPath, iItem);
		return strPath;
	}

	/**
	 * ��ȡ��ǰĿ¼��ַ
	 *
	 * @return ��ȡ��ǰĿ¼��ַ
	 */
	CString GetCurrentFolder(){
		CString strPath;
		CMFCShellListCtrl::GetCurrentFolder(strPath);
		return strPath;
	}

	/**
	 * ���ù����ַ���
	 *
	 * @param str
	 *        �����ַ���
	 */
	void SetFilterString(CString str){
		m_filter = str;
		m_filter.MakeLower();

		DisplayFolder(GetCurrentFolder());
	}

protected:
	/**
	 * �����ַ���
	 */
	CString m_filter;

protected:
	/**
	 * ������
	 */
	virtual void OnSetColumns();

	/**
	 * ���໯�ؼ�
	 */
	virtual void PreSubclassWindow();

protected:
	DECLARE_MESSAGE_MAP()

protected:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnDoubleClick(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnLvnItemchanged(NMHDR *pNMHDR, LRESULT *pResult);

protected:
	virtual HRESULT EnumObjects(LPSHELLFOLDER pParentFolder, LPITEMIDLIST pidlParent);
};