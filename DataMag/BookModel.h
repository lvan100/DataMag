#pragma once

// ͼ�����Ŀ¼�ṹ
//
// -- ��Ŀ¼
//	 -- ͼ��
//	    -- ͼ���ļ�
//	    -- ����.txt
//	 -- ͼ��
//	    -- ͼ���ļ�
//	    -- ����.txt

/**
 * ͼ�����ģ��
 */
class BookModel
{
public:
	BookModel(CString strBookDir);
	~BookModel(void);

	/**
	 * ���ͼ��
	 */
	BOOL AddBook(CString strBookName);

	/**
	 * ɾ��ͼ��
	 */
	BOOL DeleteBook(CString strBookName);

	/**
	 * ������ͼ��
	 */
	BOOL RenameBook(CString oldBookName, CString newBookName);

	/**
	 * ��ȡͼ��
	 */
	CString GetBook(CString strBookName, BOOL checkExist = TRUE);

protected:
	/**
	 * ͼ�����Ŀ¼
	 */
	CString m_strBookDir;
};