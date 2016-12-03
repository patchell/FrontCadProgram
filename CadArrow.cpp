#include "stdafx.h"
#include "CadArrow.h"
#include "CadPolygon.h"
#include "FrontCadDefines.h"
#include <math.h>

int CCadArrow::m_RenderEnable = 1;

CCadArrow::CCadArrow() :CCadObject(OBJECT_TYPE_ARROW)
{
	m_Attrib.m_Len = 0;
	m_Attrib.m_LineWidth = 0;
	m_Attrib.m_Color = RGB(0, 0, 0);
	m_pPen = 0;
	m_pBrFill = 0;
}

CCadArrow::CCadArrow(CCadArrow &ca):CCadObject(OBJECT_TYPE_ARROW)
{
	m_Attrib.m_Len = ca.m_Attrib.m_Len;
	m_Attrib.m_LineWidth = ca.m_Attrib.m_LineWidth;
	m_Attrib.m_Color = ca.m_Attrib.m_Color;
	m_pPen = 0;
	m_pBrFill = 0;
	m_P1 = ca.m_P1;
	m_P2 = ca.m_P2;
}


CCadArrow::~CCadArrow()
{
	if (m_pPen) delete m_pPen;
	if (m_pBrFill) delete m_pBrFill;
}

void CCadArrow::Move(CPoint p)
{
	CSize Diff = p - m_P1;
	m_P1 += Diff;
	m_P2 += Diff;
}

void CCadArrow::Save(FILE *pO)
{
	fprintf(pO, "ARROW(POINT(%d,%d),POINT(%d,%d),LENGTH(%d),WIDTH(%d),COLOR(%d,%d,%d))\n",
		m_P1.x,m_P1.y,m_P2.x,m_P2.y,
		m_Attrib.m_Len,m_Attrib.m_LineWidth,
		(m_Attrib.m_Color & 0x0ff0000) >> 16,
		(m_Attrib.m_Color & 0x0ff00) >> 8,
		m_Attrib.m_Color & 0xff
		);
}

void CCadArrow::SetVertex(int Vi, CPoint p)
{
	if (Vi)
		m_P2 += p - m_P2;
	else
		m_P1 += p - m_P1;
}

int CCadArrow::GrabVertex(CPoint p)
{
	return -1;
}

void CCadArrow::Draw(CDC *pDC, int mode, CPoint Offset, CScale Scale)
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
	CPen *pOldPen;
	CBrush *pOldBr;
	CRect rect;
	CPoint P1, P2,P3,P4;

	if (CCadArrow::m_RenderEnable)
	{
		P1 = Scale * m_P1 + Offset;
		P2 = Scale * m_P2 + Offset;
		if (mode == OBJECT_MODE_FINAL)
			CalcPoints(P3, P4, Offset, Scale);
		else
			CalcPoints(P3, P4, Offset, Scale);
		CPoint *pP = new CPoint[3];
		pP[0] = P1;
		pP[1] = P3;
		pP[2] = P4;

		if ((m_LastMode != mode) || m_Dirty)
		{
			m_Dirty = 0;
			switch (mode)
			{
			case OBJECT_MODE_FINAL:
				if (m_pPen) delete m_pPen;
				m_pPen = new CPen();
				m_pPen->CreatePen(PS_SOLID, 1, m_Attrib.m_Color);
				if (m_pBrFill) delete m_pBrFill;
				m_pBrFill = new CBrush(m_Attrib.m_Color);
				break;
			case OBJECT_MODE_SELECTED:
				if (m_pPen) delete m_pPen;
				m_pPen = new CPen();
				m_pPen->CreatePen(PS_SOLID, 1, m_Attrib.m_Color ^ 0x00ff00);
				if (m_pBrFill) delete m_pBrFill;
				m_pBrFill = new CBrush(m_Attrib.m_Color ^ 0x00ff00);
				break;
			case OBJECT_MODE_SKETCH:
				if (m_pPen) delete m_pPen;
				m_pPen = new CPen();
				m_pPen->CreatePen(PS_SOLID, 1, m_Attrib.m_Color);
				if (m_pBrFill) delete m_pBrFill;
				m_pBrFill = new CBrush(m_Attrib.m_Color);
				break;
			}
		}
		switch (mode)
		{
		case OBJECT_MODE_FINAL:
		case OBJECT_MODE_SELECTED:
			pOldPen = pDC->SelectObject(m_pPen);
			pOldBr = pDC->SelectObject(m_pBrFill);
			pDC->Polygon(pP, 3);
			pDC->SelectObject(pOldPen);
			pDC->SelectObject(pOldBr);
			break;
		case OBJECT_MODE_SKETCH:
			pOldPen = pDC->SelectObject(m_pPen);
			pOldBr = pDC->SelectObject(m_pBrFill);
			pDC->Polygon(pP, 3);
			pDC->SelectObject(pOldPen);
			pDC->SelectObject(pOldBr);
			break;
		case OBJECT_MODE_ERASE:
			break;
		}
		m_LastMode = mode;
	}
}

int CCadArrow::CheckSelected(CPoint p,CSize Off)
{
	CCadPolygon Cp(3);
	Cp.AddPoint(m_P1+Off);
	CPoint P3, P4;
	CalcPoints(P3, P4, Off, CScale(1.0, 1.0));
	Cp.AddPoint(P3);
	Cp.AddPoint(P4);
	return Cp.CheckSelected(p);
}

CPoint CCadArrow::GetReference()
{
	return m_P1;
}

void CCadArrow::AddObject(CCadObject *pO)
{

}

void CCadArrow::RemoveObject(CCadObject *pO)
{

}

CCadObject *CCadArrow::GetHead(void)
{
	return 0;
}

void CCadArrow::MakeDirty(void)
{
	CCadObject::MakeDirty();
}

void CCadArrow::SetSelected(int Flag)
{
	CCadObject::SetSelected(Flag);
}

void CCadArrow::AdjustRefernce(CPoint p)
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

CRect CCadArrow::GetRect(void)
{
	CRect r;
	int i;
	CPoint P3, P4,Points[3];
	CalcPoints(P3, P4, CPoint(0, 0), CScale(1.0, 1.0));
	Points[0] = m_P1;
	Points[1] = P3;
	Points[2] = P4;
	int MinX, MinY, MaxX, MaxY;
	MinX = MaxX = m_P1.x;
	MinY = MaxY = m_P1.y;
	for (i = 1; i < 3; ++i)
	{
		if (MinX > Points[i].x) MinX = Points[i].x;
		if (MinY > Points[i].y) MinY = Points[i].y;
		if (MaxX < Points[i].x) MaxX = Points[i].x;
		if (MaxY < Points[i].y) MaxY = Points[i].y;
	}
	r.SetRect(MinX, MinY, MaxX, MaxY);
	r.NormalizeRect();
	return r;
}

char * CCadArrow::GetTypeString(void)
{
	return CCadObject::GetTypeString();
}

void CCadArrow::CalcPoints(CPoint & a, CPoint & b, CPoint Offset, CScale Scale)
{
	//-----------------------------------------------
	// CalcPoints
	//
	//	This function is used to calculate the
	// two other points that make up the triange
	// for the arrow.  We use m_P1 and m_P2 to do
	// this.
	//
	// parameters:
	//		a.....reference to the second ppoint
	//		b.....reference to the third point
	//		Offset....Reference Point
	//		Scale.....Scale factor
	//----------------------------------------------
	CSize diff;

	diff = m_P1 - m_P2;	//calculate difference

	if (diff.cy == 0)	//arrow pointing left or right
	{
		int dx = (diff.cx > 0)?m_Attrib.m_Len:-m_Attrib.m_Len;
		a = Scale * CPoint(m_P1.x-dx, m_P2.y + m_Attrib.m_LineWidth / 2) + Offset;
		b = Scale * CPoint(m_P1.x-dx, m_P2.y - m_Attrib.m_LineWidth / 2) + Offset;
	}
	else if (diff.cx == 0)	//arrow pointing up or down
	{
		int dy = (diff.cy > 0) ? m_Attrib.m_Len : -m_Attrib.m_Len;
		a = Scale * CPoint(m_P2.x + m_Attrib.m_LineWidth / 2, m_P1.y-dy) + Offset;
		b = Scale * CPoint(m_P2.x - m_Attrib.m_LineWidth / 2, m_P1.y-dy) + Offset;
	}
	else
	{
		double Angle;
		double x, y,dx,dy;
		//arrow pointing at some angle
		if (m_P1.x > m_P2.x && m_P1.y > m_P2.y)	//works
		{
			Angle =  - atan(double(m_P1.y - m_P2.y)/double(m_P1.x - m_P2.x) );
			dx = m_Attrib.m_Len * cos(Angle);
			dy = m_Attrib.m_Len * sin(Angle);
			Angle += 3.14159 / 2.0;
			x = double(m_Attrib.m_LineWidth / 2) * cos(Angle);
			y = double(m_Attrib.m_LineWidth / 2) * sin(Angle);
			a = Scale * CPoint(m_P1.x - int(dx + x), m_P1.y + int(dy + y)) + Offset;
			b = Scale * CPoint(m_P1.x - int(dx - x), m_P1.y + int(dy - y)) + Offset;
		}
		else if (m_P1.x > m_P2.x && m_P1.y < m_P2.y)	//Works
		{
			Angle = - atan(double(m_P2.y - m_P1.y) / double(m_P1.x - m_P2.x));
			dx = m_Attrib.m_Len * cos(Angle);
			dy = m_Attrib.m_Len * sin(Angle);
			Angle += 3.14159 / 2.0;
			x = double(m_Attrib.m_LineWidth / 2) * cos(Angle);
			y = double(m_Attrib.m_LineWidth / 2) * sin(Angle);
			a = Scale * CPoint(m_P1.x - int(dx + x), m_P1.y - int(dy + y)) + Offset;
			b = Scale * CPoint(m_P1.x - int(dx - x), m_P1.y - int(dy - y)) + Offset;

		}
		else if (m_P1.x < m_P2.x && m_P1.y > m_P2.y)//works
		{
			Angle = - atan(double(m_P1.y - m_P2.y) / double(m_P1.x - m_P2.x));
			dx = m_Attrib.m_Len * cos(Angle);
			dy = m_Attrib.m_Len * sin(Angle);
			Angle += 3.14159 / 2.0;
			x = double(m_Attrib.m_LineWidth / 2) * cos(Angle);
			y = double(m_Attrib.m_LineWidth / 2) * sin(Angle);
			a = Scale * CPoint(m_P1.x + int(dx + x), m_P1.y - int(dy + y)) + Offset;
			b = Scale * CPoint(m_P1.x + int(dx - x), m_P1.y - int(dy - y)) + Offset;

		}
		else if (m_P1.x < m_P2.x && m_P1.y < m_P2.y)	//works
		{
			Angle = -atan(double(m_P1.y - m_P2.y) / double(m_P1.x - m_P2.x));
			dx = m_Attrib.m_Len * cos(Angle);
			dy = m_Attrib.m_Len * sin(Angle);
			Angle += 3.14159 / 2.0;
			x = double(m_Attrib.m_LineWidth / 2) * cos(Angle);
			y = double(m_Attrib.m_LineWidth / 2) * sin(Angle);
			a = Scale * CPoint(m_P1.x + int(dx + x), m_P1.y - int(dy + y)) + Offset;
			b = Scale * CPoint(m_P1.x + int(dx - x), m_P1.y - int(dy - y)) + Offset;

		}
	}
}



CCadArrow CCadArrow::operator=(CCadArrow & Ca)
{

	m_Attrib.m_Color = Ca.m_Attrib.m_Color;
	m_Attrib.m_Len = Ca.m_Attrib.m_Len;
	m_Attrib.m_LineWidth = Ca.m_Attrib.m_LineWidth;
	m_P1 = Ca.m_P1;
	m_P2 = Ca.m_P2;
	return *this;
}

void CCadArrow::RenderEnable(int e)
{
	CCadArrow::m_RenderEnable = e;
}

CPoint CCadArrow::GetCenter()
{
	CRect rect = this->GetRect();
	return rect.CenterPoint();
}

// Moves the center of the object to the spcified point
void CCadArrow::ChangeCenter(CSize p)
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


CSize CCadArrow::GetSize()
{
	CRect rect = GetRect();
	return rect.Size();
}


void CCadArrow::ChangeSize(CSize Sz)
{
	m_P2.x += Sz.cx;
	m_P2.y += Sz.cy;
}
