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