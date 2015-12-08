#pragma once

/**
 * 图像资源(集合)
 */
class ImageResource
{
public:
	ImageResource();
	~ImageResource();

	/**
	 * 应用程序的图标
	 */
	HICON m_hAppIcon;

	/**
	 * 搜索框的图标
	 */
	HICON m_hSearchIcon;

	/**
	 * 标签的图标
	 */
	HICON m_hTagIcon;

	/**
	 * 源码的图标
	 */
	HICON m_hCodeIcon;

	/**
	 * 图书的图标
	 */
	HICON m_hBookIcon;

	/**
	 * 重命名的图标
	 */
	HICON m_hRenameIcon;

	/**
	 * 详细信息的图标
	 */
	HICON m_hInfoIcon;

	/**
	 * 删除的图标
	 */
	HICON m_hDeleteIcon;

	/**
	 * 添加关联的图标
	 */
	HICON m_hAddRelatedIcon;

	/**
	 * 移除关联的图标
	 */
	HICON m_hRemoveRelatedIcon;

public:
	/**
	 * 必须保证资源初始化
	 */
	void MustInitFirst();
};

/**
 * 全局的图像资源管理器
 */
extern ImageResource theImageResource;