// CadLine.h: interface for the CCadLine class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_CADLINE_H__B86774F1_566B_4664_8176_344A2AC8B46E__INCLUDED_)
#define AFX_CADLINE_H__B86774F1_566B_4664_8176_344A2AC8B46E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "CadObject.h"
class CCadPolygon;

class CFileParser;

typedef struct{
	int m_Width;
	COLORREF m_Color;
} LineAttrib;

class CCadLine : public CCadObject
{
	friend CFileParser;
	int m_OutLineWidth;
	COLORREF m_LineColor;
	CPen *m_pPenLine;
public:
	static int m_RenderEnable;
	virtual CPoint GetReference(void);
	CCadPolygon * m_Poly;
	CCadLine();
	CCadLine(CCadLine &line);
	virtual ~CCadLine();
	virtual void Move(CPoint p);
	virtual void Save(FILE *pO);
	virtual void SetVertex(int Vi,CPoint p);
	virtual int GrabVertex(CPoint p);
	virtual int CheckSelected(CPoint p, CSize Offset = CSize(0, 0));
	virtual void Draw(CDC *pDC,int mode=0,CPoint Offset=CPoint(0,0),CScale Scale=CScale(0.1,0.1));
	virtual void AdjustRefernce(CPoint Ref);
	void SetOutLineWidth(int w){m_OutLineWidth = w;}
	int GetOutLineWidth(void){return m_OutLineWidth;}
	void SetLineColor(COLORREF c){m_LineColor = c;}
	COLORREF GetLineColor(void){return m_LineColor;}
	CCadLine operator=(CCadLine &v);
	virtual void RenderEnable(int e);
	virtual CPoint GetCenter();
	// Moves the center of the object to the spcified point
	virtual void ChangeCenter(CSize p);
	virtual CSize GetSize();
	virtual void ChangeSize(CSize Sz);
};

#endif // !defined(AFX_CADLINE_H__B86774F1_566B_4664_8176_344A2AC8B46E__INCLUDED_)
