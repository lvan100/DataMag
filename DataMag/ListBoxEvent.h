#pragma once

/**
 * �б���¼��ӿ�
 */
class ListBoxEvent
{
public:
	/**
	 * ��ʼ���б��ؼ�
	 */
	virtual void InitListBox() = 0;

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
 * �б���¼��ӿ�������
 */
class CListBoxEventAdapter : public ListBoxEvent{

public:
	/**
	 * ��ʼ���б��ؼ�
	 */
	virtual void InitListBox() {}
	
	/**
	 * �б���˫���¼�
	 */
	virtual void OnDoubleClick() {}

	/**
	 * �б�����仯
	 */
	virtual void OnSelectChanged() {}
};