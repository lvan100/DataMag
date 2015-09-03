#pragma once

/**
 * �б��ؼ��¼��ӿ�
 */
class ListBoxEvent
{
public:
	/**
	 * ��ʼ���б��ؼ�
	 */
	virtual void InitListBox() = 0;

	/**
	 * �����б���˫���¼�
	 */
	virtual void OnDoubleClick() = 0;

	/**
	 * �б�ѡ������仯
	 */
	virtual void OnSelectChanged() = 0;
};

/**
 * �б��ؼ��¼��ӿ���������Ϊ�¼��ṩĬ�Ͻӿ�.
 */
class CListBoxEventAdapter : public ListBoxEvent{

public:
	/**
	 * ��ʼ���б��ؼ�
	 */
	virtual void InitListBox() {}
	
	/**
	 * �����б���˫���¼�
	 */
	virtual void OnDoubleClick() {}

	/**
	 * �б�ѡ������仯
	 */
	virtual void OnSelectChanged() {}
};