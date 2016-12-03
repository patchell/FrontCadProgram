// CadLine.cpp: implementation of the CCadLine class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "FrontCad.h"
#include "CadLine.h"
#include "CadPolygon.h"
#include "FrontCadDefines.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

int CCadLine::m_RenderEnable = 1;

CCadLine::CCadLine():CCadObject(OBJECT_TYPE_LINE)
{
	m_OutLineWidth = 1;
	m_LineColor = RGB(0,0,0);	//black line
	m_pPenLine = 0;
	m_Poly = 0;
}

CCadLine::CCadLine(CCadLine &line):CCadObject(OBJECT_TYPE_LINE)
{
	m_OutLineWidth = 1;
	m_LineColor = line.m_LineColor;	//black line
	m_pPenLine = 0;
	m_Poly = 0;
	m_P1 = line.m_P1;
	m_P2 = line.m_P2;
}

CCadLine::~CCadLine()
{
	if(m_pPenLine) delete m_pPenLine;
	if(m_Poly) delete m_Poly;
}

void CCadLine::Draw(CDC *pDC, int mode,CPoint O,CScale Scale)
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
	CPoint P1,P2,Diff;
	int Lw;
	CRect rect;

	if (CCadLine::m_RenderEnable)
	{
		P1 = (Scale * m_P1) + O;
		P2 = (Scale * m_P2) + O;
		Lw = int(Scale.m_ScaleX * m_OutLineWidth);
		if (Lw < 1) Lw = 1;
		switch (mode)
		{
		case OBJECT_MODE_FINAL:
			if (m_LastMode != OBJECT_MODE_FINAL || m_Dirty)
			{
				if (m_pPenLine) delete m_pPenLine;
				m_pPenLine = new CPen(PS_SOLID, Lw, m_LineColor);
				m_LastMode = mode;
				m_Dirty = 0;
			}
			break;
		case OBJECT_MODE_SELECTED:
			if (m_LastMode != OBJECT_MODE_SELECTED || m_Dirty)
			{
				if (m_pPenLine) delete m_pPenLine;
				m_pPenLine = new CPen(PS_SOLID, 1, m_LineColor);
				m_LastMode = mode;
				m_Dirty = 0;
			}
			break;
		case OBJECT_MODE_SKETCH:
			if (m_LastMode != OBJECT_MODE_SELECTED)
			{
				if (m_pPenLine) delete m_pPenLine;
				m_pPenLine = new CPen(PS_DOT, 1, m_LineColor);
				m_LastMode = mode;
			}
			break;
		case OBJECT_MODE_ERASE:
			break;
		}
		switch (mode)
		{
		case OBJECT_MODE_FINAL:
			pOld = pDC->SelectObject(m_pPenLine);
			pDC->MoveTo(P1);
			pDC->LineTo(P2);
			pDC->SelectObject(pOld);
			break;
		case OBJECT_MODE_SELECTED:
			pOld = pDC->SelectObject(m_pPenLine);
			Diff = CPoint(4, 4);

			rect.SetRect(P1 + (-Diff), P1 + Diff);
			pDC->Rectangle(&rect);
			rect.SetRect(P2 + (-Diff), P2 + Diff);
			pDC->Rectangle(&rect);
			pDC->SelectObject(m_pPenLine);
			pDC->MoveTo(P1);
			pDC->LineTo(P2);
			pDC->SelectObject(pOld);
			break;
		case OBJECT_MODE_SKETCH:
			Diff = CPoint(4, 4);
			pOld = pDC->SelectObject(m_pPenLine);
			pDC->MoveTo(P1);
			pDC->LineTo(P2);
			pDC->SelectObject(pOld);
			break;
		case OBJECT_MODE_ERASE:
			break;
		}
	}
}

int CCadLine::CheckSelected(CPoint p,CSize O)
{
	if(m_Poly == 0)
		m_Poly = new CCadPolygon(4);
	else
		m_Poly->Reset();

	//-----------------------------
	// enclose the line inside
	// of a polygon
	//-----------------------------
	CPoint P1 = m_P1+O;
	CPoint P2 = m_P2+O;
	int d = this->m_OutLineWidth/2;
	if(d < 10) d = 10;
	if((P1.x < P2.x) && (P1.y < P2.y))
	{
		m_Poly->AddPoint(CPoint(P1.x-d,P1.y+d));
		m_Poly->AddPoint(CPoint(P1.x+d,P1.y-d));
		m_Poly->AddPoint(CPoint(P2.x+d,P2.y-d));
		m_Poly->AddPoint(CPoint(P2.x-d,P2.y+d));
	}
	else if((P1.x > P2.x) && (P1.y < P2.y))
	{
		m_Poly->AddPoint(CPoint(P1.x-d,P1.y-d));
		m_Poly->AddPoint(CPoint(P1.x+d,P1.y+d));
		m_Poly->AddPoint(CPoint(P2.x+d,P2.y+d));
		m_Poly->AddPoint(CPoint(P2.x-d,P2.y-d));
	}
	else if((P1.x < P2.x) && (P1.y > P2.y))
	{
		m_Poly->AddPoint(CPoint(P1.x-d,P1.y-d));
		m_Poly->AddPoint(CPoint(P1.x+d,P1.y+d));
		m_Poly->AddPoint(CPoint(P2.x+d,P2.y+d));
		m_Poly->AddPoint(CPoint(P2.x-d,P2.y-d));
	}
	else if((P1.x > P2.x) && (P1.y > P2.y))
	{
		m_Poly->AddPoint(CPoint(P1.x-d,P1.y+d));
		m_Poly->AddPoint(CPoint(P1.x+d,P1.y-d));
		m_Poly->AddPoint(CPoint(P2.x+d,P2.y-d));
		m_Poly->AddPoint(CPoint(P2.x-d,P2.y+d));
	}
	else if((P1.x == P2.x)&& (P1.y < P2.y))
	{
		m_Poly->AddPoint(CPoint(P1.x-d,P1.y-d));
		m_Poly->AddPoint(CPoint(P1.x+d,P1.y-d));
		m_Poly->AddPoint(CPoint(P2.x+d,P2.y+d));
		m_Poly->AddPoint(CPoint(P2.x-d,P2.y+d));
	}
	else if((P1.x == P2.x)&& (P1.y > P2.y))
	{
		m_Poly->AddPoint(CPoint(P1.x-d,P1.y+d));
		m_Poly->AddPoint(CPoint(P1.x+d,P1.y+d));
		m_Poly->AddPoint(CPoint(P2.x+d,P2.y-d));
		m_Poly->AddPoint(CPoint(P2.x-d,P2.y-d));
	}
	else if((P1.x > P2.x)&& (P1.y == P2.y))
	{
		m_Poly->AddPoint(CPoint(P1.x+d,P1.y+d));
		m_Poly->AddPoint(CPoint(P1.x+d,P1.y-d));
		m_Poly->AddPoint(CPoint(P2.x-d,P2.y-d));
		m_Poly->AddPoint(CPoint(P2.x-d,P2.y+d));
	}
	else if((P1.x < P2.x)&& (P1.y == P2.y))
	{
		m_Poly->AddPoint(CPoint(P1.x-d,P1.y+d));
		m_Poly->AddPoint(CPoint(P1.x-d,P1.y-d));
		m_Poly->AddPoint(CPoint(P2.x+d,P2.y-d));
		m_Poly->AddPoint(CPoint(P2.x+d,P2.y+d));
	}
	return m_Poly->PointEnclosed(p);
}

void CCadLine::Save(FILE *pO)
{
	fprintf(pO,"LINE(POINT(%d,%d),POINT(%d,%d),COLOR(%d,%d,%d),WIDTH(%d))\n",
		m_P1.x,m_P1.y,m_P2.x,m_P2.y,(m_LineColor &0x0ff0000) >> 16,
		(m_LineColor &0x0ff00) >> 8,(m_LineColor &0x0ff),
		this->m_OutLineWidth);
}

CCadLine CCadLine::operator=(CCadLine &v)
{
	m_P1 = v.m_P1;
	m_P2 = v.m_P2;
	m_LineColor = v.m_LineColor;
	m_OutLineWidth = v.m_OutLineWidth;
	if((m_Poly == NULL) && (v.m_Poly != NULL))
		m_Poly = (CCadPolygon *)v.m_Poly->CopyObject();
	return *this;
}

void CCadLine::Move(CPoint p)
{
	CPoint Diff;
	Diff = p - m_P1;
	m_P1 += Diff;
	m_P2 += Diff;
}

int CCadLine::GrabVertex(CPoint point)
{
	int rV = -1;
	CRect rect;
	CPoint p1,p2,p;
	p = CPoint(4,4);
	p1 = m_P1 + p;
	p2 = m_P1 - p;
	rect.SetRect(p1,p2);
	rect.NormalizeRect();
	if(rect.PtInRect(point))
		rV = 0;
	else
	{
		p1 = m_P2 + p;
		p2 = m_P2 - p;
		rect.SetRect(p1,p2);
		rect.NormalizeRect();
		if(rect.PtInRect(point))
			rV = 1;
	}
	return rV;
}

void CCadLine::SetVertex(int Vi, CPoint p)
{
	if(Vi)
	{
		m_P2 = p;
	}
	else
		m_P1 = p;
}

CPoint CCadLine::GetReference()
{
	return m_P1;
}

void CCadLine::AdjustRefernce(CPoint p)
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

void CCadLine::RenderEnable(int e)
{
	CCadLine::m_RenderEnable = e;
}

CPoint CCadLine::GetCenter()
{
	CPoint Center((m_P1.x + m_P2.x) / 2, (m_P1.y + m_P2.y) / 2);
	return Center;
}

// Moves the center of the object to the spcified point
void CCadLine::ChangeCenter(CSize p)
{
}


CSize CCadLine::GetSize()
{
	return CSize(abs(m_P1.x - m_P2.x),abs(m_P1.y-m_P2.y));
}


void CCadLine::ChangeSize(CSize Sz)
{
	m_P2 += Sz;
}
