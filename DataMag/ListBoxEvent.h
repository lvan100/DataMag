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