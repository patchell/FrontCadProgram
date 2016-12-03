// CadArc.cpp: implementation of the CCadArc class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "FrontCad.h"
#include "CadArc.h"
#include "math.h"
#include "FrontCadDefines.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

int CCadArc::m_RenderEnable = 1;

extern double ArcTan(double X, double Y);

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CCadArc::CCadArc():CCadObject(OBJECT_TYPE_ARC)
{
	m_pPenLine = 0;
}

CCadArc::CCadArc(CCadArc &arc):CCadObject(OBJECT_TYPE_ARC)
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

CCadArc::~CCadArc()
{
	if(m_pPenLine) delete m_pPenLine;
}

void CCadArc::Draw(CDC *pDC,int mode,CPoint O,CScale Scale)
{
	///------------------------------------------
	/// Draw
	///		This function draws an Arc on the
	///	specified device context
	///
	///	parameters:
	///		pDC......pointer to the device context
	///		mode.....Drawing mode, Final, Selected, etdc
	///		O........Offset objects by this amount
	///		Scale....Scale Drawing by this much
	///------------------------------------------
	CPen *pOld;
	CRect rect;
	CSize rectLWcomp;
	CPoint Center;
	CPoint P1,P2,Start,End;
	int Lw;

	if (CCadArc::m_RenderEnable)
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
		SetRect(rect, P1, P2, rectLWcomp);
		//	rect.SetRect(P1,P2);
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
			Center = rect.CenterPoint();
			pDC->MoveTo(Center);
			pDC->LineTo(Start);
			pDC->SelectObject(pOld);
			break;
		case OBJECT_MODE_ARCEND:
			pOld = pDC->SelectObject(m_pPenLine);
			pDC->Arc(&rect, Start, End);
			Center = rect.CenterPoint();
			pDC->MoveTo(Center);
			pDC->LineTo(End);
			pDC->SelectObject(pOld);
			break;
		}
		m_LastMode = mode;
	}
}

int CCadArc::CheckSelected(CPoint p,CSize Offset)
{
	///------------------------------------------
	/// CheckSelected
	///		This function is used to see if the
	/// the given point is withing the confines
	/// of the object.  This function does not
	/// actually check to see if it is selected.
	/// Guess I did not pick a very good name for
	/// this function.
	///
	///	parameter:
	///		p......Is this point in this object?
	///	returns:
	///		True if point is inside, false if outside
	///------------------------------------------
	int rV = 0;

	double StartAngle,EndAngle,Angle;
	double CenterX,CenterY;
	CPoint P1,P2,Start,End;

	P1 = m_P1+Offset;
	P2 = m_P2+Offset;
	Start = m_atrb.m_Start+Offset;
	End = m_atrb.m_End+Offset;

	CenterX = double(P1.x) + double(P2.x - P1.x)/2.0;
	CenterY = double(P1.y) + double(P2.y - P1.y)/2.0;
	StartAngle = ArcTan((double(Start.x)-CenterX),-(double(Start.y)-CenterY));
	EndAngle = ArcTan((double(End.x)-CenterX),-(double(End.y)-CenterY));
	Angle = ArcTan(double(p.x)-CenterX,-(double(p.y)-CenterY));
	rV = CheckAngle(StartAngle,EndAngle,Angle);
	CRect rect;
	rect.SetRect(P1,P2);
	rect.NormalizeRect();

	/// 1 = x^2/A^2 + y^2/B^2
	double W = double(this->GetWidth())/2.0;
	if( W < 50.0) W = 50.0;
	double A,B;
	double X,Y,X1,Y1,X2,Y2;	
	A = double(rect.Width())/2.0;
	B = double(rect.Height())/2.0;
	X = double(p.x) - CenterX;
	Y = double(p.y) - CenterY;
	X1 = double(rect.right) - CenterX + W;
	Y1 = 0.0;
	X2 = double(rect.right) - CenterX - W;
	Y2 =  0.0;
	double Rad = (X*X)/(A*A) + (Y*Y)/(B*B);
	double Rad1 = (X1*X1)/(A*A) + (Y1 * Y1)/(B*B);
	double Rad2 = (X2*X2)/(A*A) + (Y2 * Y2)/(B*B);
	if(Rad < Rad2 || Rad > Rad1) rV = 0;
	return rV;
}

void CCadArc::Save(FILE *pO)
{
	///------------------------------------------
	/// Save
	///		This fucntion is called whenever the
	/// user saves a document of library.
	///
	/// parameter:
	///		pO.....pointer to Output File Stream
	///------------------------------------------
	fprintf(pO,"ARC(POINT(%d,%d),POINT(%d,%d),POINT(%d,%d),POINT(%d,%d),COLOR(%d,%d,%d),WIDTH(%d))\n",
		m_P1.x,m_P1.y,m_P2.x,m_P2.y,m_atrb.m_Start.x,m_atrb.m_Start.y,
		m_atrb.m_End.x,m_atrb.m_End.y,(m_atrb.m_LineColor & 0x0ff0000)>> 16,
		(m_atrb.m_LineColor & 0x0ff00)>> 8,(m_atrb.m_LineColor & 0x0ff),m_atrb.m_Width);
}

CCadArc CCadArc::operator=(CCadArc &v)
{
	///------------------------------------------
	/// operator=
	///		This function implements the assign
	///	operator
	///------------------------------------------
	m_P1 = v.m_P1;
	m_P2 = v.m_P2;
	m_atrb.m_Start = v.m_atrb.m_Start;
	m_atrb.m_End = v.m_atrb.m_Start;
	m_atrb.m_Width = v.m_atrb.m_Width;
	m_atrb.m_LineColor = v.m_atrb.m_LineColor;
	return *this;
}

void CCadArc::Move(CPoint p)
{
	CPoint Diff = p-m_P1;
	m_P1 += Diff;
	m_P2 += Diff;
	m_atrb.m_Start += Diff;
	m_atrb.m_End += Diff;
}

int CCadArc::GrabVertex(CPoint point)
{
	int rV = -1;
	return rV;
}

void CCadArc::SetVertex(int Vi, CPoint p)
{

}

CPoint CCadArc::GetReference()
{
	return m_P1;
}

void CCadArc::SetStartAngle(int Angle)
{
	//------------------------------
	// SetStartAngle
	//
	//	This function allows you to
	// specify the angle that the
	// starts and and it calculates
	// the StartPoint that will give
	// you that angle.
	//------------------------------
}

void CCadArc::SetEndAngle(int Angle)
{
}

void CCadArc::SetStartPoint(CPoint p)
{
	m_atrb.m_Start = p;
	//----------------------------
	// calculate the Angle
	// associated with this start
	// point
	//---------------------------
	double Pi = atan(1) * 4.0;
	double CenterX,CenterY;
	CenterX = double(m_P2.x - m_P1.x)/2.0;
	CenterX += double(m_P1.x);
	CenterY = double(m_P2.y - m_P1.y)/2.0;
	CenterY += double(m_P1.y);
	double A = ArcTan(double(m_atrb.m_Start.x ) - CenterX, CenterY-double(m_atrb.m_Start.y));

	A *= 180.0 / Pi;
	m_atrb.m_StartAngle = int(A * 10.0);
}

void CCadArc::SetEndPoint(CPoint p)
{
	m_atrb.m_End = p;
	double Pi = atan(1) * 4.0;
	double CenterX,CenterY;
	CenterX = double(m_P2.x - m_P1.x)/2.0;
	CenterX += double(m_P1.x);
	CenterY = double(m_P2.y - m_P1.y)/2.0;
	CenterY += double(m_P1.y);
	double A = ArcTan(double(m_atrb.m_End.x ) - CenterX,CenterY-double(m_atrb.m_End.y));
	A *= 180.0 / Pi;
	m_atrb.m_EndAngle = int(A * 10.0);
}

void CCadArc::AdjustRefernce(CPoint p)
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

void CCadArc::RenderEnable(int e)
{
	CCadArc::m_RenderEnable = e;
}

CPoint CCadArc::GetCenter()
{
	///--------------------------------------
	/// GetCecnter
	///		Returns the centerpoint of the ARC
	///	Because of the nature of the beast, the
	/// returned point may not be the exact center.
	///	returns:
	///		Point in the center of the arc.
	///--------------------------------------
	CRect rect(m_P1, m_P2);
	return rect.CenterPoint();
}

// Moves the center of the object to the spcified point
void CCadArc::ChangeCenter(CSize p)
{
}


CSize CCadArc::GetSize()
{
	CRect rect = GetRect();
	return rect.Size();
}


void CCadArc::ChangeSize(CSize Sz)
{
	m_P2.x += Sz.cx;
	m_P2.y += Sz.cy;
}
