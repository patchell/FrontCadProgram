// CadRectHole.cpp: implementation of the CCadRectHole class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "FrontCad.h"
#include "CadRectHole.h"
#include "FrontCadDefines.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
int CCadRectHole::m_RenderEnable = 1;

CCadRectHole::CCadRectHole():CCadObject(OBJECT_TYPE_HOLERECT)
{
	m_pPenLine = 0;
}


CCadRectHole::CCadRectHole(CCadRectHole &r):CCadObject(OBJECT_TYPE_HOLERECT)
{
	m_pPenLine=0;
	m_LineColor = r.m_LineColor;
	m_Width = r.m_Width;
	m_P1 = r.m_P1;
	m_P2 = r.m_P2;
	m_W = r.m_W;
	m_H = r.m_H;
}

CCadRectHole::~CCadRectHole()
{
	if(m_pPenLine) delete m_pPenLine;

}

void CCadRectHole::Draw(CDC *pDC, int mode,CPoint Offset,CScale Scale)
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
	int dx,dy;
	dx = m_W/2;
	dy = m_H/2;
	CPoint P1,P2;
	int Lw;

	if (CCadRectHole::m_RenderEnable)
	{
		P1 = Scale * m_P1 + Offset;
		P2 = Scale * m_P2 + Offset;
		dx = int((Scale.m_ScaleX * m_W) / 2.0);
		dy = int((Scale.m_ScaleY * m_H) / 2.0);
		Lw = int(m_Width * Scale.m_ScaleX);
		if (Lw < 1) Lw = 1;

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
			pDC->MoveTo(P1 + CPoint(-dx, -dy));
			pDC->LineTo(P1 + CPoint(-dx, dy));
			pDC->LineTo(P1 + CPoint(dx, dy));
			pDC->LineTo(P1 + CPoint(dx, -dy));
			pDC->LineTo(P1 + CPoint(-dx, -dy));
			dx /= 2;
			dy /= 2;
			pDC->MoveTo(P1 + CPoint(dx, dy));
			pDC->LineTo(P1 + CPoint(-dx, -dy));
			pDC->MoveTo(P1 + CPoint(-dx, dy));
			pDC->LineTo(P1 + CPoint(dx, -dy));
			pDC->SelectObject(pOld);
			break;
		case OBJECT_MODE_ERASE:
			break;
		}
		m_LastMode = mode;
	}
}

int CCadRectHole::CheckSelected(CPoint p,CSize O)
{
	int dx,dy;
	dx = m_W/2;
	dy = m_H/2;
	CPoint P1 = m_P1+O;
	CRect rect(P1 + CPoint(-dx,-dy),P1 + CPoint(dx,dy));
	rect.NormalizeRect();
	return rect.PtInRect(p);
}

void CCadRectHole::Save(FILE *pO)
{
	fprintf(pO,"HOLERECT(POINT(%d,%d),RECTW(%d),RECTH(%d),COLOR(%d,%d,%d),WIDTH(%d))\n",
		m_P1.x,m_P1.y,m_W,m_H,(m_LineColor &0x0ff0000) >> 16,
		(m_LineColor &0x0ff00) >> 8,(m_LineColor &0x0ff),
		this->m_Width);
}

CCadRectHole CCadRectHole::operator=(CCadRectHole &v)
{
	m_P1 = v.m_P1;
	m_P2 = v.m_P2;
	m_H = v.m_H;
	m_W = v.m_W;
	m_LineColor = v.m_LineColor;
	m_Width = v.m_Width;
	return *this;
}


void CCadRectHole::Move(CPoint p)
{
	m_P1 = p;
}

int CCadRectHole::GrabVertex(CPoint point)
{
	int rV = -1;
	return rV;
}

void CCadRectHole::SetVertex(int Vi, CPoint p)
{

}

CPoint CCadRectHole::GetReference()
{
	return m_P1;
}

void CCadRectHole::AdjustRefernce(CPoint p)
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
	m_P2 -= p;
}


CRect CCadRectHole::GetRect()
{
	CRect rect;
	CPoint p1,p2;
	int dx,dy;

	dx = this->m_W/2;
	dy = this->m_H/2;
	rect.SetRect(m_P1+CPoint(dx,dy),m_P1+CPoint(-dx,-dy));
	rect.NormalizeRect();
	return rect;
}

void CCadRectHole::RenderEnable(int e)
{
	CCadRectHole::m_RenderEnable = e;
}

CPoint CCadRectHole::GetCenter()
{
	return m_P1;
}

// Moves the center of the object to the spcified point
void CCadRectHole::ChangeCenter(CSize p)
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


CSize CCadRectHole::GetSize()
{
	CRect rect = GetRect();
	return rect.Size();
}


void CCadRectHole::ChangeSize(CSize Sz)
{
	m_P2.x += Sz.cx;
	m_P2.y += Sz.cy;
}
