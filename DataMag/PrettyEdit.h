#pragma once

/**
 * Ư�����ı��༭�򣬶��д�ֱ����
 */
class CPrettyEdit : public CEdit
{
	DECLARE_DYNAMIC(CPrettyEdit)

public:
	CPrettyEdit();
	virtual ~CPrettyEdit();

protected:
	virtual void Init();

protected:
	virtual void PreSubclassWindow();

	DECLARE_MESSAGE_MAP()
protected:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
};