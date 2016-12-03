// CadHoleRnd2Flat.h: interface for the CCadHoleRnd2Flat class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_CADHOLERND2FLAT_H__EAD08037_7F47_4162_A518_74A7EF370B20__INCLUDED_)
#define AFX_CADHOLERND2FLAT_H__EAD08037_7F47_4162_A518_74A7EF370B20__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "CadObject.h"

typedef struct{
	int m_Width;
	int m_FlatDist;	///distance from flat to center point
	COLORREF m_LineColor;
	int m_Radius;
} HoleRnd2FlatAttributes;

class CFileParser;

class CCadHoleRnd2Flat : public CCadObject
{
	friend CFileParser;
	CPen *m_pPenLine;
	int m_Width;
	int m_FlatDist;
	int m_Radius;
	COLORREF m_LineColor;
public:
	static int m_RenderEnable;
	virtual CRect GetRect(void);
	CCadHoleRnd2Flat(CCadHoleRnd2Flat & h);
	virtual CPoint GetReference();
	virtual void Move(CPoint p);
	virtual void Save(FILE *pO);
	virtual int CheckSelected(CPoint p, CSize Offset = CSize(0, 0));
	virtual void SetVertex(int Vi,CPoint p);
	virtual int GrabVertex(CPoint p);
	double SolveIntersection(int m,CPoint P,int FlatDist,int Radius);
	virtual void Draw(CDC *pDC, int mode=0,CPoint Offset=CPoint(0,0),CScale Scale=CScale(0.1,0.1));
	virtual void AdjustRefernce(CPoint Ref);
	CCadHoleRnd2Flat();
	virtual ~CCadHoleRnd2Flat();
	void SetWidth(int w){m_Width = w;}
	int GetWidth(void){return m_Width;}
	void SetFlatDist(int d){m_FlatDist = d;}
	int GetFlatDist(void){return m_FlatDist;}
	void SetRadius(int r){m_Radius = r;}
	int GetRadius(void){return m_Radius;}
	void SetLineColor(COLORREF c){m_LineColor = c;}
	COLORREF GetLineColor(void){return m_LineColor;}
	CCadHoleRnd2Flat operator=(CCadHoleRnd2Flat &v);
	virtual void RenderEnable(int e);
	virtual CPoint GetCenter();
	// Moves the center of the object to the spcified point
	virtual void ChangeCenter(CSize p);
	virtual CSize GetSize();
	virtual void ChangeSize(CSize Sz);
};

#endif // !defined(AFX_CADHOLERND2FLAT_H__EAD08037_7F47_4162_A518_74A7EF370B20__INCLUDED_)
