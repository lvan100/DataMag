#pragma once

/**
 * ͼ����Դ(����)
 */
class ImageResource
{
public:
	ImageResource();
	~ImageResource();

	/**
	 * Ӧ�ó����ͼ��
	 */
	HICON m_hAppIcon;

	/**
	 * �������ͼ��
	 */
	HICON m_hSearchIcon;

	/**
	 * ��ǩ��ͼ��
	 */
	HICON m_hTagIcon;

	/**
	 * Դ���ͼ��
	 */
	HICON m_hCodeIcon;

	/**
	 * ͼ���ͼ��
	 */
	HICON m_hBookIcon;

	/**
	 * ��������ͼ��
	 */
	HICON m_hRenameIcon;

	/**
	 * ��ϸ��Ϣ��ͼ��
	 */
	HICON m_hInfoIcon;

	/**
	 * ɾ����ͼ��
	 */
	HICON m_hDeleteIcon;

	/**
	 * ��ӹ�����ͼ��
	 */
	HICON m_hAddRelatedIcon;

	/**
	 * �Ƴ�������ͼ��
	 */
	HICON m_hRemoveRelatedIcon;

public:
	/**
	 * ���뱣֤��Դ��ʼ��
	 */
	void MustInitFirst();
};

/**
 * ȫ�ֵ�ͼ����Դ������
 */
extern ImageResource theImageResource;