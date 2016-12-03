#pragma once

#define PRINTRECT_11x85		0
#define PRINTRECT_14x85		1

// CComboBoxPrintRect

class CComboBoxPrintRect : public CComboBox
{
	DECLARE_DYNAMIC(CComboBoxPrintRect)
	int m_ObjMsg;
	int m_WMsg;
	CWnd *m_pW;

public:
	CComboBoxPrintRect();
	virtual ~CComboBoxPrintRect();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	void Init();
	void SetFontWeight(int w);
	void SetupMessaging(CWnd *pW, int Wmsg, int Smsg);
	void SetPrintRect(int w);
	int GetPrintRect();
	CSize GetPrintRectSize();

protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnCbnSelchange();
};


