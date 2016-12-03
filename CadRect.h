// CadRect.h: interface for the CCadRect class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_CADRECT_H__65FA097A_DD30_4DCD_A691_E90895F5F2BC__INCLUDED_)
#define AFX_CADRECT_H__65FA097A_DD30_4DCD_A691_E90895F5F2BC__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "CadObject.h"

typedef struct {
	int m_Width;
	COLORREF m_LineColor;
	COLORREF m_FillColor;
}RectAttributes;

class CFileParser;

class CCadRect : public CCadObject
{
	friend CFileParser;
	int m_OutLineWidth;
	COLORREF m_LineColor;
	COLORREF m_FillColor;
	CPen *m_pPenLine;
	CBrush *m_pBrFill;
public:
	static int m_RenderEnable;
	CCadRect();
	CCadRect(CCadRect &r);
	virtual ~CCadRect();
	virtual void Draw(CDC *pDC, int mode = 0, CPoint Offset = CPoint(0, 0), CScale Scale = CScale(0.1, 0.1));
	virtual int CheckSelected(CPoint p, CSize Offset = CSize(0, 0));
	virtual void AdjustRefernce(CPoint Ref);
	virtual void Move(CPoint p);
	virtual void Save(FILE *pO);
	virtual void SetVertex(int Vi, CPoint p);
	virtual int GrabVertex(CPoint p);
	void SetOutLineWidth(int w) { m_OutLineWidth = w; }
	int GetOutLineWidth(void) { return m_OutLineWidth; }
	void SetLineColor(COLORREF c) { m_LineColor = c; }
	COLORREF GetLineColor(void) { return m_LineColor; }
	void SetFillColor(COLORREF c) { m_FillColor = c; }
	COLORREF GetFillColor(void) { return m_FillColor; }
	CPoint GetReference();
	CCadRect operator=(CCadRect &v);
	virtual void RenderEnable(int e);
	virtual CPoint GetCenter();
	// Moves the center of the object to the spcified point
	virtual void ChangeCenter(CSize p);
	virtual CSize GetSize();
	virtual void ChangeSize(CSize Sz);
};

#endif // !defined(AFX_CADRECT_H__65FA097A_DD30_4DCD_A691_E90895F5F2BC__INCLUDED_)
