// CadHoleRound.h: interface for the CCadHoleRound class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_CADHOLEROUND_H__14A455ED_1405_4CBA_BD0F_79D1203DB952__INCLUDED_)
#define AFX_CADHOLEROUND_H__14A455ED_1405_4CBA_BD0F_79D1203DB952__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "CadObject.h"

class CFileParser;

typedef struct {
	int m_Radius;
	int m_Width;
	COLORREF m_LineColor;
}HoleRoundAttributes;

class CCadHoleRound : public CCadObject
{
	friend CFileParser;
	CPen *m_pPenLine;
	int m_Width;
	COLORREF m_LineColor;
	int m_Radius;
public:
	static int m_RenderEnable;
	virtual CRect GetRect(void);
	CCadHoleRound(CCadHoleRound &h);
	virtual CPoint GetReference();
	virtual void Move(CPoint p);
	virtual void Save(FILE *pO);
	virtual int CheckSelected(CPoint p, CSize Offset = CSize(0, 0));
	virtual void SetVertex(int Vi,CPoint p);
	virtual int GrabVertex(CPoint p);
	virtual void AdjustRefernce(CPoint Ref);
	CCadHoleRound();
	virtual ~CCadHoleRound();
	virtual void Draw(CDC *pDC, int mode=0,CPoint Offset=CPoint(0,0),CScale Scale=CScale(0.1,0.1));
	void SetLineColor(COLORREF c){m_LineColor = c;}
	COLORREF GetLineColor(void){return m_LineColor;}
	void SetWidth(int w){m_Width = w;}
	int GetWidth(void){return m_Width;}
	void SetRadius(int r){m_Radius = r;}
	int GetRadius(void){return m_Radius;}
	CCadHoleRound operator=(CCadHoleRound &v);
	virtual void RenderEnable(int e);
	virtual CPoint GetCenter();
	// Moves the center of the object to the spcified point
	virtual void ChangeCenter(CSize p);
	virtual CSize GetSize();
	virtual void ChangeSize(CSize Sz);
};

#endif // !defined(AFX_CADHOLEROUND_H__14A455ED_1405_4CBA_BD0F_79D1203DB952__INCLUDED_)
