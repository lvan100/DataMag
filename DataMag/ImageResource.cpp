#include "stdafx.h"
#include "resource.h"
#include "SelfModule.h"
#include "TypeSelect.h"
#include "ImageResource.h"

/**
 * 全局的图像资源管理器
 */
ImageResource theImageResource;

/**
 * 定义当前模块
 */
STATIC DefModule _Module("ImageResource"
	, ""
	, []$(Constructor)() {
	}, []$(Initializer)() {
		theImageResource.MustInitFirst();
	}, []$(Uninitializer)() {
	}, []$(Destructor)() {
	});


ImageResource::ImageResource()
{
}

ImageResource::~ImageResource()
{
}

void ImageResource::MustInitFirst()
{
	HINSTANCE appInstance = AfxGetInstanceHandle();

	m_hAppIcon = (HICON)LoadImage(appInstance
		, MAKEINTRESOURCE(IDR_MAINFRAME)
		, IMAGE_ICON, 0, 0, 0);

	m_hSearchIcon = (HICON)LoadImage(appInstance
		, MAKEINTRESOURCE(IDI_SEARCH)
		, IMAGE_ICON, 0, 0, 0);

	m_hTagIcon = (HICON)LoadImage(appInstance
		, MAKEINTRESOURCE(IDI_TAG)
		, IMAGE_ICON, 0, 0, 0);

	m_hCodeIcon = (HICON)LoadImage(appInstance
		, MAKEINTRESOURCE(IDI_CODE)
		, IMAGE_ICON, 0, 0, 0);

	m_hBookIcon = (HICON)LoadImage(appInstance
		, MAKEINTRESOURCE(IDI_BOOK)
		, IMAGE_ICON, 0, 0, 0);

	m_hRenameIcon = (HICON)LoadImage(appInstance
		, MAKEINTRESOURCE(IDI_RENAME)
		, IMAGE_ICON, 0, 0, 0);

	m_hInfoIcon = (HICON)LoadImage(appInstance
		, MAKEINTRESOURCE(IDI_INFO)
		, IMAGE_ICON, 0, 0, 0);

	m_hDeleteIcon = (HICON)LoadImage(appInstance
		, MAKEINTRESOURCE(IDI_DELETE)
		, IMAGE_ICON, 0, 0, 0);

	m_hAddRelatedIcon = (HICON)LoadImage(appInstance
		, MAKEINTRESOURCE(IDI_ADD_BOOK)
		, IMAGE_ICON, 0, 0, 0);

	m_hRemoveRelatedIcon = (HICON)LoadImage(appInstance
		, MAKEINTRESOURCE(IDI_REMOVE_LINK)
		, IMAGE_ICON, 0, 0, 0);
}