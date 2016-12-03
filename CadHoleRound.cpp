// CadHoleRound.cpp: implementation of the CCadHoleRound class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "FrontCad.h"
#include "CadHoleRound.h"
#include "FrontCadDefines.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

int CCadHoleRound::m_RenderEnable = 1;

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CCadHoleRound::CCadHoleRound():CCadObject(OBJECT_TYPE_HOLEROUND)
{
	m_pPenLine = 0;
}

CCadHoleRound::CCadHoleRound(CCadHoleRound &h):CCadObject(OBJECT_TYPE_HOLEROUND)
{
	m_pPenLine = 0;
	m_P1 = h.m_P1;
	m_P2 = h.m_P2;
	m_LineColor = h.m_LineColor;
	m_Radius = h.m_Radius;
	m_Width = h.m_Width;
}

CCadHoleRound::~CCadHoleRound()
{
	if(m_pPenLine) delete m_pPenLine;
}

void CCadHoleRound::Draw(CDC *pDC, int mode,CPoint O,CScale Scale)
{
	///---------------------------------------------
	///	Draw
	///		This function draws the object onto the
	///	specified device context.
	///
	/// parameters:
	///		pDC......pointer to the device context
	///		mode.....mode to use when drawing
	///		Offset...Offset to add to points
	///		Scale....Sets Units to Pixels ratio
	///---------------------------------------------
	CPen *pOld;
	CRect rect;
	int dx = m_Radius;
	CPoint start;
	CPoint P1,P2;
	int Lw;

	if (CCadHoleRound::m_RenderEnable)
	{
		P1 = Scale * m_P1 + O;
		dx = int(Scale.m_ScaleX * dx);
		Lw = int(Scale.m_ScaleX * m_Width);
		if (Lw < 1) Lw = 1;

		rect.SetRect(P1 + CPoint(-dx, -dx), P1 + CPoint(dx, dx));
		start = P1 + CPoint(dx, 0);
		dx = dx / 2;

		if (m_LastMode != mode || m_Dirty)
		{
			if (m_pPenLine) delete m_pPenLine;
			switch (mode)
			{
			case OBJECT_MODE_FINAL:
				m_pPenLine = new CPen(PS_SOLID, Lw, m_LineColor);
				break;
			case OBJECT_MODE_SELECTED:
				m_pPenLine = new CPen(PS_SOLID, Lw, RGB(0, 255, 0));
				break;
			case OBJECT_MODE_SKETCH:
				m_pPenLine = new CPen(PS_SOLID, 1, m_LineColor);
				break;
			}
			m_Dirty = 0;
		}
		switch (mode)
		{
		case OBJECT_MODE_FINAL:
		case OBJECT_MODE_SELECTED:
		case OBJECT_MODE_SKETCH:
			pOld = pDC->SelectObject(m_pPenLine);
			pDC->Arc(&rect, start, start);
			pDC->MoveTo(P1 + CPoint(dx, dx));
			pDC->LineTo(P1 + CPoint(-dx, -dx));
			pDC->MoveTo(P1 + CPoint(-dx, dx));
			pDC->LineTo(P1 + CPoint(dx, -dx));
			pDC->SelectObject(pOld);
			break;
		case OBJECT_MODE_ERASE:
			break;
		}
		m_LastMode = mode;
	}
}

int CCadHoleRound::CheckSelected(CPoint p,CSize O)
{
	double a,b,xo,yo,v;
	int rV;
	static int count = 0;

	CPoint P1 = m_P1+O;
	a = double(m_Radius);
	b = double(m_Radius);
	xo = double(p.x) - double(P1.x);
	yo = double(p.y) - double(P1.y);
	v = (xo * xo)/(a * a) + (yo * yo)/(b * b);
	if( v < 1.0) rV = TRUE;
	else rV = FALSE;
	return rV;
}

void CCadHoleRound::Save(FILE *pO)
{
	fprintf(pO,"HOLERND(POINT(%d,%d),RADIUS(%d),COLOR(%d,%d,%d),WIDTH(%d))\n",
		m_P1.x,m_P1.y,m_Radius,(m_LineColor &0x0ff0000) >> 16,
		(m_LineColor &0x0ff00) >> 8,(m_LineColor &0x0ff),
		this->m_Width);
}

CCadHoleRound CCadHoleRound::operator=(CCadHoleRound &v)
{
	m_P1 = v.m_P1;
	m_P2 = v.m_P2;
	m_LineColor = v.m_LineColor;
	m_Radius = v.m_Radius;
	m_Width = v.m_Width;
	return *this;
}

void CCadHoleRound::Move(CPoint p)
{
	m_P1 = p;
}

int CCadHoleRound::GrabVertex(CPoint point)
{
	return -1;
}

void CCadHoleRound::SetVertex(int Vi, CPoint p)
{

}

CPoint CCadHoleRound::GetReference()
{
	return m_P1;
}

void CCadHoleRound::AdjustRefernce(CPoint p)
{
	///-----------------------------------------
	///	AdjustRefernce
	///		Thhis function is used to normalize
	///	the location of points in the object
	/// relative to a point choseen on the
	/// drawing.
	///	parameters:
	///		p.....selected reference point
	///-----------------------------------------
	m_P1 -= p;
}

CRect CCadHoleRound::GetRect()
{
	CRect rect;
	CPoint p1,p2;
	int dx,dy;

	dx = m_Radius;
	dy = m_Radius;
	rect.SetRect(m_P1+CPoint(dx,dy),m_P1+CPoint(-dx,-dy));
	rect.NormalizeRect();
	return rect;
}

void CCadHoleRound::RenderEnable(int e)
{
	CCadHoleRound::m_RenderEnable = e;
}

CPoint CCadHoleRound::GetCenter()
{
	return m_P1;
}

// Moves the center of the object to the spcified point
void CCadHoleRound::ChangeCenter(CSize p)
{
	///-----------------------------------------
	///	ChangeCenter
	///		Thhis function is used to normalize
	///	the location of points in the object
	/// relative to a point choseen on the
	/// drawing.
	///	parameters:
	///		p.....selected reference point
	///-----------------------------------------
	m_P1 -= p;
}


CSize CCadHoleRound::GetSize()
{
	CRect rect = GetRect();
	return rect.Size();
}


void CCadHoleRound::ChangeSize(CSize Sz)
{
	m_Radius = Sz.cx / 2;
}
