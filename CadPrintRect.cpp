#include "stdafx.h"
#include "FrontCad.h"
#include "CadPrintRect.h"
#include "FrontCadDefines.h"

int CCadPrintRect::m_RenderEnable = 1;

CCadPrintRect::CCadPrintRect():CCadObject(OBJECT_TYPE_PRINTRECT)
{
	m_pPenLine = 0;
}

CCadPrintRect::CCadPrintRect(CCadPrintRect &PR) : CCadObject(OBJECT_TYPE_PRINTRECT)
{
	m_pPenLine = 0;
	m_Attrib.m_LineColor = PR.m_Attrib.m_LineColor;
	m_Attrib.m_Width = PR.m_Attrib.m_Width;
	m_P1 = PR.m_P1;
	m_P2 = PR.m_P2;

}


CCadPrintRect::~CCadPrintRect()
{
	if (m_pPenLine) delete m_pPenLine;
}


void CCadPrintRect::Draw(CDC *pDC, int mode, CPoint Offset, CScale Scale)
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
	CSize rectLWcomp;
	CPoint P1, P2;
	int Lw;

	if (!CCadPrintRect::m_RenderEnable) return;	//don't print
	P1 = Scale * m_P1 + Offset;
	P2 = P1 + Scale * m_Size;;
	Lw = int(m_Attrib.m_Width * Scale.m_ScaleX);
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
			m_pPenLine = new CPen(PS_SOLID, Lw, m_Attrib.m_LineColor);
			break;
		case OBJECT_MODE_SELECTED:
			m_pPenLine = new CPen(PS_SOLID, Lw, RGB(200, 50, 50));
			break;
		case OBJECT_MODE_SKETCH:
			m_pPenLine = new CPen(PS_DOT, 1, m_Attrib.m_LineColor);
			break;
		}
	}
	if (m_Dirty)
	{
		m_Dirty = 0;
	}
	switch (mode)
	{
	case OBJECT_MODE_FINAL:
		pOld = pDC->SelectObject(m_pPenLine);
		pDC->MoveTo(P1);
		pDC->LineTo(P1.x, P2.y);
		pDC->LineTo(P2);
		pDC->LineTo(P2.x, P1.y);
		pDC->LineTo(P1);
		pDC->SelectObject(pOld);
		break;
	case OBJECT_MODE_SELECTED:
	{
		CPen SelPen;
		CBrush SelBrush;
		SelPen.CreatePen(PS_SOLID, 1, RGB(255, 0, 0));
		SelBrush.CreateSolidBrush(RGB(255, 0, 0));
		pOld = pDC->SelectObject(m_pPenLine);
		pDC->MoveTo(P1);
		pDC->LineTo(P1.x, P2.y);
		pDC->LineTo(P2);
		pDC->LineTo(P2.x, P1.y);
		pDC->LineTo(P1);
		pDC->SelectObject(&SelPen);
		pDC->SelectObject(&SelBrush);
		CSize p = CSize(4, 4);
		rect.SetRect(P1 - p, P1 + p);
		pDC->Rectangle(&rect);
		rect.SetRect(P2 - p, P2 + p);
		pDC->Rectangle(&rect);
		pDC->SelectObject(pOld);
	}
	break;
	case OBJECT_MODE_SKETCH:
		pOld = pDC->SelectObject(m_pPenLine);
		pDC->MoveTo(P1);
		pDC->LineTo(P1.x, P2.y);
		pDC->LineTo(P2);
		pDC->LineTo(P2.x, P1.y);
		pDC->LineTo(P1);
		pDC->SelectObject(pOld);
		break;
	case OBJECT_MODE_ERASE:
		break;
	}
	m_LastMode = mode;
}

int CCadPrintRect::CheckSelected(CPoint p,CSize O)
{
	CRect rect;
	int rV;
	CPoint P1 = m_P1+O;
	CPoint P2 = P1 + m_Size;
	rect.SetRect(P1, P2);
	rect.NormalizeRect();
	rV = (int)rect.PtInRect(p);
	return rV;
}

void CCadPrintRect::Save(FILE *pO)
{
	fprintf(pO, "PRINTRECT(POINT(%d,%d),SIZE(%d,%d),WIDTH(%d),COLOR(%d,%d,%d))",
		m_P1.x,m_P1.y,
		m_Size.cx,m_Size.cy,
		m_Attrib.m_Width,
		RED(m_Attrib.m_LineColor),GREEN(m_Attrib.m_LineColor),BLUE(m_Attrib.m_LineColor)
	);
}

CCadPrintRect CCadPrintRect::operator=(CCadPrintRect &v)
{
	m_P1 = v.m_P1;
	m_P2 = v.m_P2;
	m_Attrib.m_LineColor = v.m_Attrib.m_LineColor;
	m_Attrib.m_Width = v.m_Attrib.m_Width;
	return *this;
}


void CCadPrintRect::Move(CPoint p)
{
	CSize Diff = p - m_P1;
	m_P1 += Diff;
}

int CCadPrintRect::GrabVertex(CPoint point)
{
	int rV = -1;
	CSize Diff(4, 4);
	CRect rect;
	rect.SetRect(m_P1 + Diff, m_P1 - Diff);
	rect.NormalizeRect();
	if (rect.PtInRect(point))
		rV = 0;
	else
	{
		rect.SetRect(m_P2 + Diff, m_P2 - Diff);
		rect.NormalizeRect();
		if (rect.PtInRect(point))
			rV = 1;
	}
	return rV;
}

void CCadPrintRect::SetVertex(int Vi, CPoint p)
{
	if (Vi)
		m_P2 += p - m_P2;
	else
		m_P1 += p - m_P1;
}

CPoint CCadPrintRect::GetReference()
{
	return m_P1;
}

void CCadPrintRect::AdjustRefernce(CPoint p)
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

CRect CCadPrintRect::GetRect()
{
	CRect rect;

	rect.SetRect(m_P1, m_P1 + m_Size);
	rect.NormalizeRect();
	return rect;
}
void CCadPrintRect::RenderEnable(int e)
{
	CCadPrintRect::m_RenderEnable = e;
}

CPoint CCadPrintRect::GetCenter()
{
	CRect rect = this->GetRect();
	return rect.CenterPoint();
}

// Moves the center of the object to the spcified point
void CCadPrintRect::ChangeCenter(CSize p)
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

void CCadPrintRect::ChangeSize(CSize Sz)
{
	m_P2.x += Sz.cx;
	m_P2.y += Sz.cy;
}
