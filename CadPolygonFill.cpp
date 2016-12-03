// CadPolygonFill.cpp: implementation of the CCadPolygonFill class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "FrontCad.h"
#include "CadPolygonFill.h"
#include "FrontCadDefines.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//int CCadPolygonFill::m_RenderEnable;

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CCadPolygonFill::CCadPolygonFill():CCadPolygon(100)
{
	m_pBrFill = 0;
	SetType(OBJECT_TYPE_POLYFILL);
}

CCadPolygonFill::CCadPolygonFill(int size):CCadPolygon(size)
{
	m_pBrFill = 0;
	SetType(OBJECT_TYPE_POLYFILL);
}

CCadPolygonFill::~CCadPolygonFill()
{
	if(m_pBrFill)delete m_pBrFill;
}

void CCadPolygonFill::Draw(CDC *pDC,int mode,CPoint Offset,CScale Scale)
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
	if (CCadPolygonFill::m_RenderEnable)
	{
		CPen *oldpen;
		CBrush *oldBrush;
		int Lw;
		if ((Lw = int(Scale.m_ScaleX * m_Width)) < 1) Lw = 1;
		int i = 0;
		//-----------------------
		// need to scale the
		// verticies
		//-----------------------
		CPoint *pP = new CPoint[m_Count];
		for (i = 0; i < m_Count; ++i)
			pP[i] = Scale * m_pVertex[i] + Offset;

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
		if ((m_pBrFill == 0) || m_Dirty)
		{
			m_Dirty = 0;
			if (m_pBrFill) delete m_pBrFill;
			m_pBrFill = new CBrush(m_FillColor);
		}
		switch (mode)
		{
		case OBJECT_MODE_FINAL:
			oldpen = pDC->SelectObject(m_pPenLine);
			oldBrush = pDC->SelectObject(m_pBrFill);
			pDC->Polygon(pP, m_Count);
			pDC->SelectObject(oldBrush);
			pDC->SelectObject(oldpen);
			m_LastMode = mode;
			break;
		case OBJECT_MODE_SELECTED:
			oldpen = pDC->SelectObject(m_pPenLine);
			i = 0;
			pDC->MoveTo(pP[i]);
			for (i = 1; i < this->m_Count; ++i)
				pDC->LineTo(pP[i]);
			pDC->LineTo(pP[0]);
			for (i = 0; i < this->m_Count; ++i)
			{
				CPoint p1, p2, p;
				p = pP[i];
				p1 = p + CPoint(4, 4);
				p2 = p - CPoint(4, 4);
				CRect rect;
				rect.SetRect(p2, p1);
				pDC->Rectangle(&rect);
			}
			pDC->SelectObject(oldpen);
			m_LastMode = mode;
			break;
		case OBJECT_MODE_SKETCH:
			oldpen = pDC->SelectObject(m_pPenLine);
			i = 0;
			pDC->MoveTo(pP[i]);
			for (i = 1; i < this->m_Count; ++i)
				pDC->LineTo(pP[i]);
			pDC->SelectObject(oldpen);
			m_LastMode = mode;
			break;
		case OBJECT_MODE_ERASE:
			break;
		}
	}
}

void CCadPolygonFill::Save(FILE *pO)
{
	fprintf(pO,"POLYFILL(VERTEX(%d),",m_Count);
	int i;
	for(i=0;i<m_Count;++i)
	{
		fprintf(pO,"POINT(%d,%d),",m_pVertex[i].x,m_pVertex[i].y);
	}
	fprintf(pO,"COLOR(%d,%d,%d),COLOR(%d,%d,%d),WIDTH(%d))\n",
		(m_LineColor & 0x0ff0000) >> 16,
		(m_LineColor & 0x0ff00) >> 8,
		m_LineColor & 0x0ff,
		(m_FillColor & 0x0ff0000) >> 16,
		(m_FillColor & 0x0ff00) >> 8,
		m_FillColor & 0x0ff,
		m_Width);
}

CCadPolygonFill CCadPolygonFill::operator=(CCadPolygonFill &v)
{
	this->m_pPenLine =0;
	this->m_pBrFill = 0;
	m_size = v.m_size;
	m_Count = v.m_Count;
	m_Width = v.m_Width;
	this->m_FillColor = v.m_FillColor;
	m_LineColor = v.m_LineColor;
	m_MaxY = v.m_MaxY;
	m_MinY = v.m_MinY;
	m_MaxX = v.m_MaxX;
	m_MinX = v.m_MinX;
	m_PolyID = v.m_PolyID;
	for(int i=0;i<m_Count	;++i)
		m_pVertex[i] = v.m_pVertex[i];
	return *this;
}


CPoint CCadPolygonFill::GetCenter()
{
	CRect rect = this->GetRect();
	return rect.CenterPoint();
}

// Moves the center of the object to the spcified point
void CCadPolygonFill::ChangeCenter(CSize p)
{
	m_P1 -= p;
	int i;
	for (i = 0; i<m_Count; ++i)
		m_pVertex[i] -= p;
	UpdateMinMax();
}



CSize CCadPolygonFill::GetSize()
{
	CRect rect = GetRect();
	return rect.Size();
}


void CCadPolygonFill::ChangeSize(CSize Sz)
{
}
