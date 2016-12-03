// CadHoleRnd1Flat.h: interface for the CCadHoleRnd1Flat class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_CADHOLERND1FLAT_H__A9C94EAC_1F36_421C_A9F3_8B94ED066B9B__INCLUDED_)
#define AFX_CADHOLERND1FLAT_H__A9C94EAC_1F36_421C_A9F3_8B94ED066B9B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "CadObject.h"

class CFileParser;

typedef struct{
	int m_Width;
	int m_FlatDist;	///distance from flat to center point
	COLORREF m_LineColor;
	int m_Radius;
} HoleRnd1FlatAttributes;

class CCadHoleRnd1Flat : public CCadObject
{
	friend CFileParser;
	CPen *m_pPenLine;
	int m_Width;
	int m_FlatDist;	///distance from flat to center point
	int m_Radius;
	COLORREF m_LineColor;
public:
	static int m_RenderEnable;
	virtual CRect GetRect(void);
	CCadHoleRnd1Flat(CCadHoleRnd1Flat &h);
	virtual CPoint GetReference();
	virtual void Move(CPoint p);
	virtual void Save(FILE *pO);
	virtual int CheckSelected(CPoint p, CSize Offset = CSize(0, 0));
	virtual void SetVertex(int Vi,CPoint p);
	virtual int GrabVertex(CPoint p);
	virtual void AdjustRefernce(CPoint Ref);
	CCadHoleRnd1Flat();
	virtual ~CCadHoleRnd1Flat();
	virtual void Draw(CDC *pDC,int mode=0,CPoint Offset=CPoint(0,0),CScale Scale=CScale(0.1,0.1));
	void SetOutLineWidth(int w){m_Width = w;}
	int GetOutLineWidth(void){return m_Width;}
	void SetLineColor(COLORREF c){m_LineColor = c;}
	COLORREF GetLineColor(void){return m_LineColor;}
	double SolveIntersection(int m,CPoint P,int FlatDist,int Radius);
	void SetFlatDist(int d){m_FlatDist = d;}
	int GetFlatDist(void){return m_FlatDist;}
	void SetRadius(int r){m_Radius = r;}
	int GetRadius(void){return m_Radius;}
	void SetWidth(int w){m_Width = w;}
	int GetWidth(void){return m_Width;}
	CCadHoleRnd1Flat operator=(CCadHoleRnd1Flat &v);
	virtual void RenderEnable(int e);
	virtual CPoint GetCenter();
	// Moves the center of the object to the spcified point
	virtual void ChangeCenter(CSize p);
	virtual CSize GetSize();
	virtual void ChangeSize(CSize Sz);
};

#endif // !defined(AFX_CADHOLERND1FLAT_H__A9C94EAC_1F36_421C_A9F3_8B94ED066B9B__INCLUDED_)
