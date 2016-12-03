// CadHoleRnd2Flat.cpp: implementation of the CCadHoleRnd2Flat class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "FrontCad.h"
#include "CadHoleRnd2Flat.h"
#include "FrontCadDefines.h"

#include <math.h>

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

int CCadHoleRnd2Flat::m_RenderEnable = 1;

CCadHoleRnd2Flat::CCadHoleRnd2Flat():CCadObject(OBJECT_TYPE_HOLERND2FLAT)
{
	m_pPenLine = 0;
}

CCadHoleRnd2Flat::CCadHoleRnd2Flat(CCadHoleRnd2Flat &h):CCadObject(OBJECT_TYPE_HOLERND2FLAT)
{
	m_pPenLine = 0;

	m_P1 = h.m_P1;
	m_P2 = h.m_P2;
	m_FlatDist = h.m_FlatDist;
	m_LineColor = h.m_LineColor;
	m_Radius = h.m_Radius;
	m_Width = h.m_Width;
}

CCadHoleRnd2Flat::~CCadHoleRnd2Flat()
{
	if(m_pPenLine) delete m_pPenLine;
}

void CCadHoleRnd2Flat::AdjustRefernce(CPoint p)
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

void CCadHoleRnd2Flat::Draw(CDC *pDC, int mode,CPoint Offset,CScale Scale)
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
	CPoint start,end;
	CPoint P1,P2;
	int Lw,Fd;
	int Rad;

	if (CCadHoleRnd2Flat::m_RenderEnable)
	{
		P1 = (Scale * m_P1) + Offset;
		dx = int(Scale.m_ScaleX * dx);
		Rad = dx;
		Lw = int(Scale.m_ScaleX * m_Width);
		Fd = int(Scale.m_ScaleX * m_FlatDist);
		if (Lw < 1) Lw = 1;

		rect.SetRect(P1.x - dx, P1.y - dx, P1.x + dx, P1.y + dx);
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
				m_pPenLine = new CPen(PS_SOLID, Lw, RGB(0, 250, 0));
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
			start.x = P1.x + Fd;
			start.y = int(SolveIntersection(1, P1, Fd, Rad));
			end.x = P1.x - Fd;
			end.y = start.y;
			pDC->Arc(&rect, start, end);
			start.x = P1.x - Fd;
			end.x = P1.x + Fd;
			start.y = int(SolveIntersection(0, P1, Fd, Rad));
			end.y = start.y;
			pDC->Arc(&rect, start, end);
			pDC->MoveTo(P1.x + Fd, int(SolveIntersection(1, P1, Fd, Rad)));
			pDC->LineTo(P1.x + Fd, int(SolveIntersection(0, P1, Fd, Rad)));
			pDC->MoveTo(P1.x - Fd, int(SolveIntersection(1, P1, Fd, Rad)));
			pDC->LineTo(P1.x - Fd, int(SolveIntersection(0, P1, Fd, Rad)));
			pDC->MoveTo(P1.x + dx, P1.y + dx);
			pDC->LineTo(P1.x - dx, P1.y - dx);
			pDC->MoveTo(P1.x - dx, P1.y + dx);
			pDC->LineTo(P1.x + dx, P1.y - dx);
			pDC->SelectObject(pOld);
			break;
		case OBJECT_MODE_ERASE:
			break;
		}
		m_LastMode = mode;
	}
}

double CCadHoleRnd2Flat::SolveIntersection(int m,CPoint P1,int FlatDist,int Radius)
{
	double Y,T,Xd,R;

	Xd = double(FlatDist);
	R = double(Radius);
	Y = double(P1.y);
	if(m)
		T = Y - sqrt(R*R-Xd*Xd);
	else
		T = Y + sqrt(R*R-Xd*Xd);
	return T;
}


int CCadHoleRnd2Flat::CheckSelected(CPoint p,CSize O)
{
	int dx,dy;
	CPoint P1 = m_P1+O;
	dx = this->m_FlatDist;
	dy = this->m_Radius;
	CRect rect(P1 + CPoint(-dx,-dy),P1 + CPoint(dx,dy));
	rect.NormalizeRect();
	return rect.PtInRect(p);
}

void CCadHoleRnd2Flat::Save(FILE *pO)
{
	fprintf(pO,"HOLE2FLAT(POINT(%d,%d),RADIUS(%d),FLATDIST(%d),COLOR(%d,%d,%d),WIDTH(%d))\n",
		m_P1.x,m_P1.y,m_Radius,m_FlatDist,(m_LineColor &0x0ff0000) >> 16,
		(m_LineColor &0x0ff00) >> 8,(m_LineColor &0x0ff),
		this->m_Width);
}

CCadHoleRnd2Flat CCadHoleRnd2Flat::operator =(CCadHoleRnd2Flat &v)
{
	m_P1 = v.m_P1;
	m_P2 = v.m_P2;
	m_FlatDist = v.m_FlatDist;
	m_LineColor = v.m_LineColor;
	m_Radius = v.m_Radius;
	m_Width = v.m_Width;
	return *this;
}

void CCadHoleRnd2Flat::Move(CPoint p)
{
	m_P1 = p;
}

int CCadHoleRnd2Flat::GrabVertex(CPoint point)
{
	return -1;
}

void CCadHoleRnd2Flat::SetVertex(int Vi, CPoint p)
{

}

CPoint CCadHoleRnd2Flat::GetReference()
{
	return m_P1;
}

CRect CCadHoleRnd2Flat::GetRect()
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

void CCadHoleRnd2Flat::RenderEnable(int e)
{
	CCadHoleRnd2Flat::m_RenderEnable = e;
}

CPoint CCadHoleRnd2Flat::GetCenter()
{
	return m_P1;
}

// Moves the center of the object to the spcified point
void CCadHoleRnd2Flat::ChangeCenter(CSize p)
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


CSize CCadHoleRnd2Flat::GetSize()
{
	CRect rect = GetRect();
	return rect.Size();
}


void CCadHoleRnd2Flat::ChangeSize(CSize Sz)
{
	m_Radius = Sz.cx / 2;
}
