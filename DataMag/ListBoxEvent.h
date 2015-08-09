#pragma once

/**
 * 列表框事件接口
 */
class ListBoxEvent
{
public:
	/**
	 * 初始化列表框控件
	 */
	virtual void InitListBox() = 0;

	/**
	 * 列表项双击事件
	 */
	virtual void OnDoubleClick() = 0;

	/**
	 * 列表项发生变化
	 */
	virtual void OnSelectChanged() = 0;
};

/**
 * 列表框事件接口适配器
 */
class CListBoxEventAdapter : public ListBoxEvent{

public:
	/**
	 * 初始化列表框控件
	 */
	virtual void InitListBox() {}
	
	/**
	 * 列表项双击事件
	 */
	virtual void OnDoubleClick() {}

	/**
	 * 列表项发生变化
	 */
	virtual void OnSelectChanged() {}
};