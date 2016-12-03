// CadRect.cpp: implementation of the CCadRect class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "FrontCad.h"
#include "CadRect.h"
#include "FrontCadDefines.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

int CCadRect::m_RenderEnable = 1;

CCadRect::CCadRect():CCadObject(OBJECT_TYPE_RECT)
{
	m_pPenLine=0;
	m_pBrFill = 0;
}

CCadRect::CCadRect(CCadRect &r):CCadObject(OBJECT_TYPE_RECT)
{
	m_pPenLine=0;
	m_pBrFill = 0;
	m_FillColor = r.m_FillColor;
	m_LineColor = r.m_LineColor;
	m_OutLineWidth = r.m_OutLineWidth;
	m_P1 = r.m_P1;
	m_P2 = r.m_P2;
}

CCadRect::~CCadRect()
{
	if(m_pPenLine) delete m_pPenLine;
	if(m_pBrFill) delete m_pBrFill;
}

void CCadRect::Draw(CDC *pDC, int mode,CPoint Offset,CScale Scale)
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

	if (CCadRect::m_RenderEnable)
	{
		P1 = Scale * m_P1 + Offset;
		P2 = Scale * m_P2 + Offset;
		Lw = int(m_OutLineWidth * Scale.m_ScaleX);
		if (Lw <= 1 || OBJECT_MODE_SKETCH == mode)
		{
			Lw = 1;
			rectLWcomp = CSize(0, 0);
		}
		else
			rectLWcomp = CSize(Lw / 2, Lw / 2);
		if (m_LastMode != mode || m_Dirty)
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
		if (m_Dirty)
		{
			if (m_pBrFill) delete m_pBrFill;
			m_pBrFill = new CBrush(m_FillColor);
			m_Dirty = 0;
		}
		if (m_pBrFill == 0) m_pBrFill = new CBrush(m_FillColor);
		SetRect(rect, P1, P2, rectLWcomp);
		switch (mode)
		{
		case OBJECT_MODE_FINAL:
			pOld = pDC->SelectObject(m_pPenLine);
			pOldBr = pDC->SelectObject(m_pBrFill);
			pDC->Rectangle(&rect);
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
			pDC->Rectangle(&rect);
			pDC->SelectObject(&SelPen);
			pDC->SelectObject(&SelBrush);
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
			pDC->Rectangle(&rect);
			pDC->SelectObject(pOld);
			break;
		case OBJECT_MODE_ERASE:
			break;
		}
		m_LastMode = mode;
	}
}

int CCadRect::CheckSelected(CPoint p,CSize O)
{
	CRect rect;
	int rV;
	CPoint P1 = m_P1+O;
	CPoint P2 = m_P2+O;
	rect.SetRect(P1,P2);
	rect.NormalizeRect();
	rV = (int)rect.PtInRect(p);
	return rV;
}

void CCadRect::Save(FILE *pO)
{
	fprintf(pO,"RECT(POINT(%d,%d),POINT(%d,%d),COLOR(%d,%d,%d),COLOR(%d,%d,%d),WIDTH(%d))\n",
		m_P1.x,m_P1.y,m_P2.x,m_P2.y,
		(m_LineColor & 0x0ff0000) >> 16,
		(m_LineColor & 0x0ff00) >> 8,
		(m_LineColor & 0x0ff),
		(m_FillColor & 0x0ff0000) >> 16,
		(m_FillColor & 0x0ff00) >> 8,
		(m_FillColor & 0x0ff),m_OutLineWidth);
}

CCadRect CCadRect::operator=(CCadRect &v)
{
	m_P1 = v.m_P1;
	m_P2 = v.m_P2;
	m_FillColor = v.m_FillColor;
	m_LineColor = v.m_LineColor;
	m_OutLineWidth = v.m_OutLineWidth;
	return *this;
}


void CCadRect::Move(CPoint p)
{
	CSize Diff = p - m_P1;
	m_P1 += Diff;
	m_P2 += Diff;
}

int CCadRect::GrabVertex(CPoint point)
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

void CCadRect::SetVertex(int Vi, CPoint p)
{
	if(Vi)
		m_P2 += p - m_P2;
	else
		m_P1 += p - m_P1;
}

CPoint CCadRect::GetReference()
{
	return m_P1;
}

void CCadRect::AdjustRefernce(CPoint p)
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

void CCadRect::RenderEnable(int e)
{
	CCadRect::m_RenderEnable = e;
}

CPoint CCadRect::GetCenter()
{
	CRect rect = this->GetRect();
	return rect.CenterPoint();
}

// Moves the center of the object to the spcified point
void CCadRect::ChangeCenter(CSize p)
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


CSize CCadRect::GetSize()
{
	CRect rect = GetRect();
	return rect.Size();
}


void CCadRect::ChangeSize(CSize Sz)
{
	m_P2.x +=  Sz.cx;
	m_P2.y +=  Sz.cy;
}
