#include "StdAfx.h"
#include "FileOp.h"
#include "BookModel.h"

BookModel::BookModel(CString strBookDir)
	: m_strBookDir(strBookDir)
{
}

BookModel::~BookModel(void)
{
}

CString BookModel::GetBook(CString strBookName, BOOL checkExist)
{ 
	CString strFile = m_strBookDir + _T("\\") + strBookName;
	return (checkExist || PathFileExists(strFile)) ? strFile : _T("");
}

BOOL BookModel::AddBook(CString strBookName)
{
	CString strBookFolder = m_strBookDir;
	strBookFolder +=  _T("\\") + strBookName;

	if (CreateDirectory(strBookFolder, nullptr)) {
		CString strFile = strBookFolder + _T("\\����.txt");
		CloseHandle(CreateFile(strFile, 0, 0, nullptr, CREATE_ALWAYS, 0, nullptr));
		return TRUE;
	}
	return FALSE;
}

BOOL BookModel::DeleteBook(CString strBookName)
{
	CString strFile = GetBook(strBookName);
	if (PathIsDirectory(strFile)) {
		DeleteDirectory(strFile);
	} else {
		DeleteFile(strFile);
	}
	return TRUE;
}

BOOL BookModel::RenameBook(CString oldBookName, CString newBookName)
{
// 	CString strOldBook = GetBook(oldBookName);
// 	if (PathFileExists(szOldPath)) {
// 		if (PathFileExists(szNewPath)) {
// 			CString strContent = _T("ͼ��Ŀ¼\"\"�Ѵ��ڣ�");
// 			strContent.Insert(5, szNewPath);
// 			MessageBox(strContent, _T("����"), MB_ICONERROR);
// 			return _T(""); /* �ļ��Ѵ��ڣ�������ʧ��! */
// 
// 		} else {
// 			CFile::Rename(szOldPath, szNewPath);
// 		}
// 	}
// 
// 	_tcsncpy_s(szOldPath, szNewPath, MAX_PATH);
// 
// 	PathAppend(szNewPath, dlg.m_name);
// 	PathAppend(szOldPath, szOldName);
// 
// 	if (PathFileExists(szOldPath)) {
// 		CFile::Rename(szOldPath, szNewPath);
// 	}
// 
// 	return dlg.m_name;
	return false;
}