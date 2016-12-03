// CadElipse.h: interface for the CCadElipse class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_CADELIPSE_H__7652BDAC_7D47_420B_92E2_5F93D2617B54__INCLUDED_)
#define AFX_CADELIPSE_H__7652BDAC_7D47_420B_92E2_5F93D2617B54__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "CadObject.h"

typedef struct {
	int m_Width;
	COLORREF m_LineColor;
	COLORREF m_FillColor;
}ElipseAttributes;

class CFileParser;

class CCadElipse : public CCadObject
{
	friend CFileParser;
	int m_Width;
	COLORREF m_LineColor;
	COLORREF m_FillColor;
	CPen *m_pPenLine;
	CBrush *m_pBrFill;
public:
	static int m_RenderEnable;
	CCadElipse(CCadElipse &e);
	virtual CPoint GetReference();
	virtual void Move(CPoint p);
	virtual void Save(FILE *pO);
	virtual int CheckSelected(CPoint p, CSize Offset = CSize(0, 0));
	virtual void Draw(CDC *pDC,int mode=0,CPoint Offset=CPoint(0,0),CScale Scale=CScale(0.1,0.1));
	virtual void SetVertex(int Vi,CPoint p);
	virtual int GrabVertex(CPoint p);
	virtual void AdjustRefernce(CPoint Ref);
	CCadElipse();
	virtual ~CCadElipse();
	void SetOutLineWidth(int w){m_Width = w;}
	int GetOutLineWidth(void){return m_Width;}
	void SetLineColor(COLORREF c){m_LineColor = c;}
	COLORREF GetLineColor(void){return m_LineColor;}
	void SetFillColor(COLORREF c){m_FillColor = c;}
	COLORREF GetFillColor(void){return m_FillColor;}
	CCadElipse operator=(CCadElipse &v);
	virtual void RenderEnable(int e);
	virtual CPoint GetCenter();
	// Moves the center of the object to the spcified point
	virtual void ChangeCenter(CSize p);
	virtual CSize GetSize();
	virtual void ChangeSize(CSize Sz);
};

#endif // !defined(AFX_CADELIPSE_H__7652BDAC_7D47_420B_92E2_5F93D2617B54__INCLUDED_)
