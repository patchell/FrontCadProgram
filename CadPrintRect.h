#pragma once
#include "CadObject.h"

typedef struct {
	int m_Width;
	COLORREF m_LineColor;
}PrintRectAttributes;

class CFileParser;

class CCadPrintRect :public CCadObject
{
	friend CFileParser;
	PrintRectAttributes m_Attrib;
	CPen *m_pPenLine;
	CSize m_Size;
public:
	static int m_RenderEnable;
	CCadPrintRect();
	CCadPrintRect(CCadPrintRect &PR);
	virtual ~CCadPrintRect();
	virtual void Draw(CDC *pDC, int mode = 0, CPoint Offset = CPoint(0, 0), CScale Scale = CScale(0.1, 0.1));
	virtual int CheckSelected(CPoint p, CSize Offset = CSize(0, 0));
	virtual void AdjustRefernce(CPoint Ref);
	virtual void Move(CPoint p);
	virtual void Save(FILE *pO);
	virtual void SetVertex(int Vi, CPoint p);
	virtual int GrabVertex(CPoint p);
	void SetOutLineWidth(int w) { m_Attrib.m_Width = w; }
	int GetOutLineWidth(void) { return m_Attrib.m_Width; }
	void SetLineColor(COLORREF c) { m_Attrib.m_LineColor = c; }
	COLORREF GetLineColor(void) { return m_Attrib.m_LineColor; }
	CPoint GetReference();
	CCadPrintRect operator=(CCadPrintRect &v);
	virtual void RenderEnable(int e);
	virtual CPoint GetCenter();
	// Moves the center of the object to the spcified point
	virtual void ChangeCenter(CSize p);
	void SetSize(CSize s) { m_Size = s; }
	virtual CSize GetSize(void) { return m_Size; }
	virtual CRect GetRect(void);
	virtual void ChangeSize(CSize Sz);
};

