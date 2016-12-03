// CadArcCentered.cpp: implementation of the CCadArcCentered class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "FrontCad.h"
#include "CadArcCentered.h"
#include "FrontCadDefines.h"
#include <math.h>

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

int CCadArcCentered::m_RenderEnable = 1;

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CCadArcCentered::CCadArcCentered():CCadObject(OBJECT_TYPE_ARCCENTERED)
{
	m_pPenLine = 0;
}

CCadArcCentered::CCadArcCentered(CCadArcCentered &arc):CCadObject(OBJECT_TYPE_ARCCENTERED)
{
	m_P1 = arc.m_P1;
	m_P2 = arc.m_P2;
	m_atrb.m_End = arc.m_atrb.m_End;
	m_atrb.m_EndAngle = arc.m_atrb.m_EndAngle;
	m_atrb.m_LineColor = arc.m_atrb.m_LineColor;
	m_atrb.m_Start = arc.m_atrb.m_Start;
	m_atrb.m_StartAngle = arc.m_atrb.m_StartAngle;
	m_atrb.m_Width = arc.m_atrb.m_Width;
	m_pPenLine = 0;
}

CCadArcCentered::~CCadArcCentered()
{
	if(m_pPenLine) delete m_pPenLine;
}

void CCadArcCentered::Draw(CDC *pDC, int mode,CPoint O,CScale Scale)
{
	CPen *pOld;
	CRect rect;
	CSize rectLWcomp;
	CPoint p1,p2;
	int dx,dy;
	CPoint P1,P2,Start,End;
	int Lw;

	if (CCadArcCentered::m_RenderEnable)
	{
		P1 = Scale * m_P1 + O;
		P2 = Scale * m_P2 + O;
		Start = Scale * m_atrb.m_Start + O;
		End = Scale * m_atrb.m_End + O;
		Lw = int(m_atrb.m_Width * Scale.m_ScaleX);
		if (Lw <= 1 || OBJECT_MODE_SKETCH == mode)
		{
			Lw = 1;
			rectLWcomp = CSize(0, 0);
		}
		else
			rectLWcomp = CSize(Lw / 2, Lw / 2);

		if (P1.x >= P2.x)
		{
			if (P1.y > P2.y)
			{
				dx = P1.x - P2.x;
				dy = P1.y - P2.y;
			}
			else if (P1.y == P2.y)
			{
				dx = P1.x - P2.x;
				dy = P1.y - P2.y;
			}
			else
			{
				dx = P1.x - P2.x;
				dy = P2.y - P1.y;
			}
		}
		else
		{
			if (P1.y > P2.y)
			{
				dx = P2.x - P1.x;
				dy = P1.y - P2.y;
			}
			else if (P1.y == P2.y)
			{
				dx = P2.x - P1.x;
				dy = P1.y - P2.y;
			}
			else
			{
				dx = P2.x - P1.x;
				dy = P2.y - P1.y;
			}
		}
		p1 = P1 + CPoint(-dx, -dy);
		p2 = P1 + CPoint(dx, dy);
		SetRect(rect, p1, p2, rectLWcomp);
		//	rect.SetRect(p1,p2);
		if ((m_LastMode != mode) || m_Dirty)
		{
			if (m_pPenLine) delete m_pPenLine;
			switch (mode)
			{
			case OBJECT_MODE_FINAL:
				m_pPenLine = new CPen(PS_SOLID, Lw, m_atrb.m_LineColor);
				break;
			case OBJECT_MODE_SELECTED:
				m_pPenLine = new CPen(PS_SOLID, Lw, RGB(200, 50, 50));
				break;
			case OBJECT_MODE_SKETCH:
				m_pPenLine = new CPen(PS_DOT, 1, m_atrb.m_LineColor);
				break;
			case OBJECT_MODE_ARCSTART:
			case OBJECT_MODE_ARCEND:
				m_pPenLine = new CPen(PS_DOT, 1, m_atrb.m_LineColor);
				break;
			}
			m_Dirty = 0;
		}
		switch (mode)
		{
		case OBJECT_MODE_FINAL:
			pOld = pDC->SelectObject(m_pPenLine);
			pDC->Arc(&rect, Start, End);
			pDC->SelectObject(pOld);
			break;
		case OBJECT_MODE_SELECTED:
			pOld = pDC->SelectObject(m_pPenLine);
			pDC->Arc(&rect, Start, End);
			pDC->SelectObject(pOld);
			break;
		case OBJECT_MODE_SKETCH:
			pOld = pDC->SelectObject(m_pPenLine);
			pDC->Rectangle(&rect);
			pDC->SelectObject(pOld);
			break;
		case OBJECT_MODE_ERASE:
			break;
		case OBJECT_MODE_ARCSTART:
			pOld = pDC->SelectObject(m_pPenLine);
			pDC->Ellipse(&rect);
			pDC->MoveTo(P1);
			pDC->LineTo(Start);
			pDC->SelectObject(pOld);
			break;
		case OBJECT_MODE_ARCEND:
			pOld = pDC->SelectObject(m_pPenLine);
			pDC->Arc(&rect, Start, End);
			pDC->MoveTo(P1);
			pDC->LineTo(End);
			pDC->SelectObject(pOld);
			break;
		}
		m_LastMode = mode;
	}
}

void CCadArcCentered::Save(FILE *pO)
{
	fprintf(pO,"ARCCENT(POINT(%d,%d),POINT(%d,%d),POINT(%d,%d),POINT(%d,%d),COLOR(%d,%d,%d),WIDTH(%d))\n",
		m_P1.x,m_P1.y,m_P2.x,m_P2.y,m_atrb.m_Start.x,m_atrb.m_Start.y,m_atrb.m_End.x,m_atrb.m_End.y,(m_atrb.m_LineColor & 0x0ff0000)>> 16,
		(m_atrb.m_LineColor & 0x0ff00)>> 8,(m_atrb.m_LineColor & 0x0ff),m_atrb.m_Width);
}

CCadArcCentered CCadArcCentered::operator=(CCadArcCentered &v)
{
	m_P1 = v.m_P1;
	m_P2 = v.m_P2;
	m_atrb.m_LineColor = v.m_atrb.m_LineColor;
	m_atrb.m_End = v.m_atrb.m_End;
	m_atrb.m_Start = v.m_atrb.m_Start;
	m_atrb.m_Width = v.m_atrb.m_Width;
	return *this;
}

int CCadArcCentered::CheckSelected(CPoint p,CSize Offset)
{
	extern double ArcTan(double X, double Y);
	double StartAngle,EndAngle,Angle;
	int rV = 0;
	CPoint P1,P2,Start,End;

	P1 = m_P1+Offset;
	P2 = m_P2+Offset;
	Start = m_atrb.m_Start+Offset;
	End = m_atrb.m_End+Offset;
	StartAngle = ArcTan(double( Start.x-P1.x ),double(P1.y - Start.y));
	EndAngle = ArcTan(double(End.x-P1.x ),double(P1.y - End.y));
	Angle = ArcTan(double(p.x-P1.x),double(P1.y - p.y));
	rV = CheckAngle(StartAngle,EndAngle,Angle);

	/// 1 = x^2/A^2 + y^2/B^2
	double W = double(this->GetWidth())/2.0;
	if( W < 50.0) W = 50.0;
	double A,B;
	double X,Y,X1,Y1,X2,Y2;	
	A = double(P2.x-P1.x);
	B = double(P2.y - P1.y);
	X = double(p.x) - double(P1.x);
	Y = double(p.y) - double(P1.y);
	X1 = double(P2.x) - double(P1.x) + W;
	Y1 = 0.0;
	X2 = double(P2.x) - double(P1.x) - W;
	Y2 =  0.0;
	double Rad = (X*X)/(A*A) + (Y*Y)/(B*B);
	double Rad1 = (X1*X1)/(A*A) + (Y1 * Y1)/(B*B);
	double Rad2 = (X2*X2)/(A*A) + (Y2 * Y2)/(B*B);
	if(Rad < Rad2 || Rad > Rad1) rV = 0;
	
	return rV;
}

void CCadArcCentered::Move(CPoint p)
{
	CPoint Diff = p-m_P1;
	m_P1 += Diff;
	m_P2 += Diff;
	m_atrb.m_Start += Diff;
	m_atrb.m_End += Diff;
}


int CCadArcCentered::GrabVertex(CPoint point)
{
	int rV = -1;
	return rV;
}

void CCadArcCentered::SetVertex(int Vi, CPoint p)
{

}

CPoint CCadArcCentered::GetReference()
{
	return m_P1;
}


void CCadArcCentered::SetStartAngle(int Angle)
{

}

void CCadArcCentered::SetEndAngle(int Angle)
{

}

void CCadArcCentered::SetStartPoint(CPoint p)
{
	m_atrb.m_Start = p;
	//----------------------------
	// calculate the Angle
	// associated with this start
	// point
	//---------------------------
	double Pi = atan(1) * 4.0;
	double A = ArcTan(double(m_atrb.m_Start.x ) - double(m_P1.x), double(m_P1.y)-double(m_atrb.m_Start.y));

	A *= 180.0 / Pi;
	m_atrb.m_StartAngle = int(A * 10.0);
}

void CCadArcCentered::SetEndPoint(CPoint p)
{
	m_atrb.m_End = p;
	double Pi = atan(1) * 4.0;
	double A = ArcTan(double(m_atrb.m_End.x ) - double(m_P1.x),double(m_P1.y)-double(m_atrb.m_End.y));
	A *= 180.0 / Pi;
	m_atrb.m_EndAngle = int(A * 10.0);
}

void CCadArcCentered::AdjustRefernce(CPoint p)
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
	this->m_atrb.m_Start -= p;
	this->m_atrb.m_End -= p;
}

void CCadArcCentered::RenderEnable(int e)
{
	CCadArcCentered::m_RenderEnable = e;
}

CPoint CCadArcCentered::GetCenter()
{
	return m_P1;
}

// Moves the center of the object to the spcified point
void CCadArcCentered::ChangeCenter(CSize p)
{
}


CSize CCadArcCentered::GetSize()
{
	CRect rect = GetRect();
	return rect.Size();
}


void CCadArcCentered::ChangeSize(CSize Sz)
{
	m_P2.x += Sz.cx;
	m_P2.y += Sz.cy;
}
