// CadArc.h: interface for the CCadArc class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_CADARC_H__3B090840_9D7E_4C2D_BCEB_A0159F1FCBEA__INCLUDED_)
#define AFX_CADARC_H__3B090840_9D7E_4C2D_BCEB_A0159F1FCBEA__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "CadObject.h"

typedef struct{
	int m_Width;
	COLORREF m_LineColor;
	CPoint m_Start;
	CPoint m_End;
	int m_StartAngle;
	int m_EndAngle;
}ArcAttributes;

class CFileParser;

class CCadArc : public CCadObject
{
	friend CFileParser;
public:
	static int m_RenderEnable;
	CCadArc();
	CCadArc(CCadArc& arc);
	virtual ~CCadArc();
	virtual CPoint GetReference();
	virtual void Move(CPoint p);
	virtual void Save(FILE *pO);
	virtual void SetVertex(int Vi,CPoint p);
	virtual int GrabVertex(CPoint p);
	virtual void Draw(CDC *pDC,int mode=0,CPoint Offset=CPoint(0,0),CScale Scale=CScale(0.1,0.1));
	virtual int CheckSelected(CPoint p, CSize Offset = CSize(0, 0));
	virtual void AdjustRefernce(CPoint Ref);
	void SetWidth(int w){m_atrb.m_Width = w;}
	int GetWidth(void){return m_atrb.m_Width;}
	void SetLineColor(COLORREF c){m_atrb.m_LineColor = c;}
	COLORREF GetLineColor(void){return m_atrb.m_LineColor;}
	void SetEndAngle(int Angle);
	int GetEndAngle(void){return m_atrb.m_EndAngle;}
	void SetStartAngle(int Angle);
	int GetStartAngle(void){return m_atrb.m_StartAngle;}
	void SetEndPoint(CPoint p);
	CPoint GetEndPoint(void){return m_atrb.m_End;}
	void SetStartPoint(CPoint p);
	CPoint GetStartPoint(void){return m_atrb.m_Start;}
	CCadArc operator=(CCadArc &v);
	virtual void RenderEnable(int e);
	virtual CPoint GetCenter();
	// Moves the center of the object to the spcified point
	virtual void ChangeCenter(CSize p);
private:
	ArcAttributes m_atrb;
	CPen * m_pPenLine;
public:
	virtual CSize GetSize();
	void ChangeSize(CSize Sz);
};

#endif // !defined(AFX_CADARC_H__3B090840_9D7E_4C2D_BCEB_A0159F1FCBEA__INCLUDED_)
