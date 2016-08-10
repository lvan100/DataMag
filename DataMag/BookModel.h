#pragma once

// 图书管理目录结构
//
// -- 根目录
//	 -- 图书
//	    -- 图书文件
//	    -- 描述.txt
//	 -- 图书
//	    -- 图书文件
//	    -- 描述.txt

/**
 * 图书管理模型
 */
class BookModel
{
public:
	BookModel(CString strBookDir);
	~BookModel(void);

	/**
	 * 添加图书
	 */
	BOOL AddBook(CString strBookName);

	/**
	 * 删除图书
	 */
	BOOL DeleteBook(CString strBookName);

	/**
	 * 重命名图书
	 */
	BOOL RenameBook(CString oldBookName, CString newBookName);

	/**
	 * 获取图书
	 */
	CString GetBook(CString strBookName, BOOL checkExist = TRUE);

protected:
	/**
	 * 图书管理目录
	 */
	CString m_strBookDir;
};