// CadElipse.cpp: implementation of the CCadElipse class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "FrontCad.h"
#include "CadElipse.h"
#include "FrontCadDefines.h"

#include <stdio.h>

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

int CCadElipse::m_RenderEnable = 1;

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CCadElipse::CCadElipse():CCadObject(OBJECT_TYPE_ELLIPSE)
{
	m_pPenLine = 0;
	m_pBrFill = 0;
}

CCadElipse::CCadElipse(CCadElipse &e):CCadObject(OBJECT_TYPE_ELLIPSE)
{
	m_P1 = e.m_P1;
	m_P2 = e.m_P2;
	this->m_FillColor = e.m_FillColor;
	this->m_LineColor = e.m_LineColor;
	this->m_Width = e.m_Width;
	m_pPenLine = 0;
	m_pBrFill = 0;
}

CCadElipse::~CCadElipse()
{
	if(m_pPenLine) delete m_pPenLine;
	if(m_pBrFill) delete m_pBrFill;
}

void CCadElipse::Draw(CDC *pDC, int mode,CPoint Offset,CScale Scale)
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
	CBrush *pOldBr;
	CRect rect;
	CSize rectLWcomp;
	CPoint P1,P2;
	int Lw;

	if (CCadElipse::m_RenderEnable)
	{
		P1 = Scale * m_P1 + Offset;
		P2 = Scale * m_P2 + Offset;
		if ((Lw = int(Scale.m_ScaleX * m_Width)) < 1) Lw = 1;
		if (Lw <= 1 || OBJECT_MODE_SKETCH == mode)
		{
			Lw = 1;
			rectLWcomp = CSize(0, 0);
		}
		else
			rectLWcomp = CSize(Lw / 2, Lw / 2);
		if ((m_LastMode != mode) || m_Dirty)
		{
			if (m_pPenLine) delete m_pPenLine;
			switch (mode)
			{
			case OBJECT_MODE_FINAL:
				m_pPenLine = new CPen(PS_SOLID, Lw, m_LineColor);
				break;
			case OBJECT_MODE_SELECTED:
				m_pPenLine = new CPen(PS_SOLID, Lw, RGB(200, 50, 50));
				break;
			case OBJECT_MODE_SKETCH:
				m_pPenLine = new CPen(PS_DOT, 1, m_LineColor);
				break;
			}
		}
		if (m_pBrFill == 0)
			m_pBrFill = new CBrush(m_FillColor);
		else if (m_Dirty)
		{
			if (m_pBrFill) delete m_pBrFill;
			m_pBrFill = new CBrush(m_FillColor);
			m_Dirty = 0;
		}
		SetRect(rect, P1, P2, rectLWcomp);
		switch (mode)
		{
		case OBJECT_MODE_FINAL:
			pOld = pDC->SelectObject(m_pPenLine);
			pOldBr = pDC->SelectObject(m_pBrFill);
			pDC->Ellipse(&rect);
			pDC->SelectObject(pOldBr);
			pDC->SelectObject(pOld);
			break;
		case OBJECT_MODE_SELECTED:
		{
			CPen SelPen;
			CBrush SelBrush;
			SelPen.CreatePen(PS_SOLID, 1, RGB(255, 0, 0));
			SelBrush.CreateSolidBrush(RGB(255, 0, 0));
			pOld = pDC->SelectObject(m_pPenLine);
			pOldBr = pDC->SelectObject(m_pBrFill);
			pDC->Ellipse(&rect);
			pDC->SelectObject(&SelPen);
			pDC->SelectObject(&SelBrush);
			CRect rect;
			CSize p = CSize(4, 4);
			rect.SetRect(P1 - p, P1 + p);
			pDC->Rectangle(&rect);
			rect.SetRect(P2 - p, P2 + p);
			pDC->Rectangle(&rect);
			pDC->SelectObject(pOldBr);
			pDC->SelectObject(pOld);
		}
		break;
		case OBJECT_MODE_SKETCH:
			pOld = pDC->SelectObject(m_pPenLine);
			pDC->Ellipse(&rect);
			pDC->SelectObject(pOld);
			break;
		case OBJECT_MODE_ERASE:
			break;
		}
		m_LastMode = mode;
	}
}

int CCadElipse::CheckSelected(CPoint p,CSize O)
{
	double a,b,xo,yo,v;
	int rV;
	static int count = 0;
	CPoint P1 = m_P1+O;
	CPoint P2 = m_P2+O;

	a = double(P2.x - P1.x)/2.0;
	b = double(P2.y - P1.y)/2.0;
	xo = p.x - (a + P1.x);
	yo = p.y - (b + P1.y);
	v = (xo * xo)/(a * a) + (yo * yo)/(b * b);
	if( v < 1.0) rV = TRUE;
	else rV = FALSE;
	return rV;
}

void CCadElipse::Save(FILE *pO)
{
	fprintf(pO,"CIRCLE(POINT(%d,%d),POINT(%d,%d),COLOR(%d,%d,%d),COLOR(%d,%d,%d),WIDTH(%d))\n",
		m_P1.x,m_P1.y,m_P2.x,m_P2.y,(m_LineColor &0x0ff0000) >> 16,
		(m_LineColor &0x0ff00) >> 8,(m_LineColor &0x0ff),
		(m_FillColor &0x0ff0000) >> 16,(m_FillColor &0x0ff00) >> 8,
		(m_FillColor &0x0ff),this->m_Width);
}

CCadElipse CCadElipse::operator=(CCadElipse &v)
{
	m_P1 = v.m_P1;
	m_P2 = v.m_P2;
	m_FillColor = v.m_FillColor;
	m_LineColor = v.m_LineColor;
	m_Width = v.m_Width;
	return *this;
}

void CCadElipse::Move(CPoint p)
{
	CSize Diff = p - m_P1;
	m_P1 += Diff;
	m_P2 += Diff;
}

int CCadElipse::GrabVertex(CPoint point)
{
	int rV = -1;
	CSize Diff(4,4);
	CRect rect;
	rect.SetRect(m_P1+Diff,m_P1-Diff);
	rect.NormalizeRect();
	if(rect.PtInRect(point))
		rV = 0;
	else
	{
		rect.SetRect(m_P2+Diff,m_P2-Diff);
		rect.NormalizeRect();
		if(rect.PtInRect(point))
			rV = 1;
	}
	return rV;
}

void CCadElipse::SetVertex(int Vi, CPoint p)
{
	if(Vi)
		m_P2 += p - m_P2;
	else
		m_P1 += p - m_P1;
}

CPoint CCadElipse::GetReference()
{
	return m_P1;
}

void CCadElipse::AdjustRefernce(CPoint p)
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

void CCadElipse::RenderEnable(int e)
{
	CCadElipse::m_RenderEnable = e;
}

CPoint CCadElipse::GetCenter()
{
	CRect rect = this->GetRect();
	return rect.CenterPoint();
}

// Moves the center of the object to the spcified point
void CCadElipse::ChangeCenter(CSize p)
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
	m_P2 -= p;
}


CSize CCadElipse::GetSize()
{
	CRect rect = GetRect();
	return rect.Size();
}


void CCadElipse::ChangeSize(CSize Sz)
{
	m_P2.x += Sz.cx;
	m_P2.y += Sz.cy;
}
