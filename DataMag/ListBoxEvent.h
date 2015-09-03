#pragma once

/**
 * 列表框控件事件接口
 */
class ListBoxEvent
{
public:
	/**
	 * 初始化列表框控件
	 */
	virtual void InitListBox() = 0;

	/**
	 * 处理列表项双击事件
	 */
	virtual void OnDoubleClick() = 0;

	/**
	 * 列表选择项发生变化
	 */
	virtual void OnSelectChanged() = 0;
};

/**
 * 列表框控件事件接口适配器，为事件提供默认接口.
 */
class CListBoxEventAdapter : public ListBoxEvent{

public:
	/**
	 * 初始化列表框控件
	 */
	virtual void InitListBox() {}
	
	/**
	 * 处理列表项双击事件
	 */
	virtual void OnDoubleClick() {}

	/**
	 * 列表选择项发生变化
	 */
	virtual void OnSelectChanged() {}
};