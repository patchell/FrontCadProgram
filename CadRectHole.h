// CadRectHole.h: interface for the CCadRectHole class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_CADRECTHOLE_H__7D533B79_BB3B_4DFF_8AD2_D368D3DD1871__INCLUDED_)
#define AFX_CADRECTHOLE_H__7D533B79_BB3B_4DFF_8AD2_D368D3DD1871__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "CadObject.h"

typedef struct {
	int m_Width;
	COLORREF m_LineColor;
	int m_W;
	int m_H;
}RectHoleAttributes;

class CFileParser;

class CCadRectHole : public CCadObject  
{
	friend CFileParser;
	CPen *m_pPenLine;
	int m_Width;
	COLORREF m_LineColor;
	int m_W;
	int m_H;
public:
	static int m_RenderEnable;
	virtual CRect GetRect(void);
	CCadRectHole(CCadRectHole &r);
	virtual CPoint GetReference();
	virtual void Move(CPoint p);
	virtual void Save(FILE *pO);
	virtual int CheckSelected(CPoint p, CSize Offset = CSize(0, 0));
	virtual void Draw(CDC *pDC,int mode=0,CPoint Offset=CPoint(0,0),CScale Scale=CScale(0.1,0.1));
	virtual void SetVertex(int Vi,CPoint p);
	virtual int GrabVertex(CPoint p);
	virtual void AdjustRefernce(CPoint p);
	CCadRectHole();
	virtual ~CCadRectHole();
	void SetOutLineWidth(int w){m_Width = w;}
	int GetOutLineWidth(void){return m_Width;}
	void SetLineColor(COLORREF c){m_LineColor = c;}
	COLORREF GetLineColor(void){return m_LineColor;}
	void SetHieghtWidth(int h, int w){m_H = h;m_W=w;}
	int GetWidth(void){return m_W;}
	int GetHieght(void){return m_H;}
	CCadRectHole operator=(CCadRectHole &v);
	virtual void RenderEnable(int e);
	virtual CPoint GetCenter();
	// Moves the center of the object to the spcified point
	virtual void ChangeCenter(CSize p);
	virtual CSize GetSize();
	virtual void ChangeSize(CSize Sz);
};
#endif // !defined(AFX_CADRECTHOLE_H__7D533B79_BB3B_4DFF_8AD2_D368D3DD1871__INCLUDED_)
