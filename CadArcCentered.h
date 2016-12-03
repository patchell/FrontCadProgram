// CadArcCentered.h: interface for the CCadArcCentered class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_CADARCCENTERED_H__C255DC18_CFC6_498A_B1C8_B1C63146AA03__INCLUDED_)
#define AFX_CADARCCENTERED_H__C255DC18_CFC6_498A_B1C8_B1C63146AA03__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "CadObject.h"

//----------------------------------
// It should be noted that point mP1
// is used as the center poiot, and
// m_P2 is used as the radius
//----------------------------------

class CFileParser;

class CCadArcCentered : public CCadObject
{
	friend CFileParser;
public:
	static int m_RenderEnable;
	CCadArcCentered(CCadArcCentered &arc);
	CCadArcCentered();
	virtual ~CCadArcCentered();
	virtual void Draw(CDC *pDC,int mode=0,CPoint Offset=CPoint(0,0),CScale Scale=CScale(0.1,0.1));
	virtual int CheckSelected(CPoint p, CSize Offset = CSize(0, 0));
	virtual CPoint GetReference();
	virtual void Move(CPoint p);
	virtual void Save(FILE *pO);
	virtual void SetVertex(int Vi,CPoint p);
	virtual int GrabVertex(CPoint p);
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
	CCadArcCentered operator=(CCadArcCentered &v);
	virtual void RenderEnable(int e);
	virtual CPoint GetCenter();
	// Moves the center of the object to the spcified point
	virtual void ChangeCenter(CSize p);
private:
	ArcAttributes m_atrb;
	CPen * m_pPenLine;
public:
	virtual CSize GetSize();
	virtual void ChangeSize(CSize Sz);
};

#endif // !defined(AFX_CADARCCENTERED_H__C255DC18_CFC6_498A_B1C8_B1C63146AA03__INCLUDED_)
