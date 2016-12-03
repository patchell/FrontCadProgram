// CadBitmap.cpp: implementation of the CCadBitmap class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "frontcad.h"
#include "CadBitmap.h"
#include <stdio.h>
#include "FrontCadDefines.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

int CCadBitmap::m_RenderEnable = 1;

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CCadBitmap::CCadBitmap():CCadObject(OBJECT_TYPE_BITMAP)
{
	m_pBM = 0;
	m_pFilename = new char[512];
	m_MaintainAspectRatio = 1;
}


CCadBitmap::CCadBitmap(CCadBitmap &v):CCadObject(OBJECT_TYPE_BITMAP)
{
	m_pBM = 0;
	m_P1 = v.m_P1;
	m_P2 = v.m_P2;
	m_pFilename = new char[512];
	strcpy_s(m_pFilename,512,v.m_pFilename);
	LoadImage(m_pFilename);
	this->m_MaintainAspectRatio = v.m_MaintainAspectRatio;
}	


CCadBitmap CCadBitmap::operator =(CCadBitmap &v)
{
	m_P1 = v.m_P1;
	m_P2 = v.m_P2;
	m_pFilename = new char[512];
	strcpy_s(m_pFilename,512,v.m_pFilename);
	LoadImage(m_pFilename);
	this->m_MaintainAspectRatio = v.m_MaintainAspectRatio;
	return *this;
}

CCadBitmap::~CCadBitmap()
{
	if(m_pBM) delete m_pBM;
	delete[] m_pFilename;
}

void CCadBitmap::Move(CPoint p)
{
	CSize Diff = p - m_P1;
	m_P1 += Diff;
	m_P2 += Diff;
}

void CCadBitmap::Save(FILE *pO)
{
	fprintf(pO,"BITMAP(POINT(%d,%d),POINT(%d,%d),FILE(\"%s\"))\n",
		m_P1.x,m_P1.y,m_P2.x,m_P2.y,m_pFilename);
}

void CCadBitmap::SetVertex(int Vi,CPoint p)
{
	if(Vi)
		m_P2 += p - m_P2;
	else
		m_P1 += p - m_P1;
	if(m_MaintainAspectRatio)
		RestoreAspectRatio();
}

int CCadBitmap::GrabVertex(CPoint point)
{
	int rV = -1;
	CSize Diff(6,6);
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

void CCadBitmap::Draw(CDC *pDC,int mode,CPoint Offset,CScale Scale)
{
	CPen pen,*oldpen;
	CRect rect;
	CPoint P1,P2;

	if (CCadBitmap::m_RenderEnable)
	{
		if (mode == OBJECT_MODE_SKETCH || m_LastMode == OBJECT_MODE_SKETCH)
		{
			double AspectRatioBM;
			AspectRatioBM = double(m_Size.cy) / double(m_Size.cx);
			m_P2.y = int(AspectRatioBM * double(abs(m_P2.x - m_P1.x))) + m_P1.y;
		}
		P1 = Scale * m_P1 + Offset;
		P2 = Scale * m_P2 + Offset;
		rect.SetRect(P1, P2);

		switch (mode)
		{
		case OBJECT_MODE_FINAL:
		{
			CDC bmDC;
			CBitmap *pOld;
			bmDC.CreateCompatibleDC(pDC);
			pOld = bmDC.SelectObject(this->m_pBM);
			pDC->StretchBlt(P1.x, P1.y, rect.Width(), rect.Height(), &bmDC, 0, 0, m_Size.cx, m_Size.cy, SRCCOPY);
			bmDC.SelectObject(pOld);
		}
		break;
		case OBJECT_MODE_SELECTED:
		{
			CPen bPen;
			CRect Vrect;
			bPen.CreatePen(PS_SOLID, 1, RGB(0, 0, 0));
			pen.CreatePen(PS_SOLID, 3, RGB(255, 0, 0));
			oldpen = pDC->SelectObject(&pen);
			CDC bmDC;
			CBitmap *pOld;
			bmDC.CreateCompatibleDC(pDC);
			pOld = bmDC.SelectObject(this->m_pBM);
			pDC->StretchBlt(P1.x, P1.y, rect.Width(), rect.Height(), &bmDC, 0, 0, m_Size.cx, m_Size.cy, SRCCOPY);
			pDC->MoveTo(P1);
			pDC->LineTo(P1.x, P2.y);
			pDC->LineTo(P2);
			pDC->LineTo(P2.x, P1.y);
			pDC->LineTo(P1);
			pDC->SelectObject(&bPen);
			Vrect.SetRect(P1 + CSize(6, 6), P1 + CSize(-6, -6));
			pDC->Rectangle(Vrect);
			Vrect.SetRect(P2 + CSize(6, 6), P2 + CSize(-6, -6));
			pDC->Rectangle(Vrect);
			bmDC.SelectObject(pOld);
			pDC->SelectObject(oldpen);
		}
		break;
		case OBJECT_MODE_SKETCH:
			pen.CreatePen(PS_SOLID, 1, RGB(0, 0, 0));
			oldpen = pDC->SelectObject(&pen);
			pDC->DrawDragRect(&rect, CSize(1, 1), NULL, CSize(1, 1));
			pDC->SelectObject(oldpen);
			break;
		}
		this->m_LastMode = mode;
	}
}

int CCadBitmap::CheckSelected(CPoint p,CSize Off)
{
	CRect rect;
	int rV;
	CPoint P1 = m_P1+Off;
	CPoint P2 = m_P2+Off;
	rect.SetRect(P1,P2);
	rect.NormalizeRect();
	rV = (int)rect.PtInRect(p);
	return rV;
}

CPoint CCadBitmap::GetReference()
{
	return m_P1;
}

void CCadBitmap::AddObject(CCadObject *pO)
{
}

void CCadBitmap::RemoveObject(CCadObject *pO)
{
}

void CCadBitmap::MakeDirty(void)
{
	CCadObject::MakeDirty();
}

void CCadBitmap::SetSelected(int Flag)
{
	CCadObject::SetSelected(Flag);
}

void CCadBitmap::AdjustRefernce(CPoint p)
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

CRect CCadBitmap::GetRect(void)
{
	CRect rR;
	rR.SetRect(m_P1,m_P2);
	rR.NormalizeRect();
	return rR;
}

void CCadBitmap::LoadImage(char *path)
{
	if(m_pBM) delete m_pBM;
	m_pBM = new CMyBitmap;
	m_pBM->LoadBitmap(path);
	strcpy_s(this->m_pFilename,512,path);
	BITMAP bm;
	m_pBM->GetBitmap(&bm);
	m_Size = CSize(bm.bmWidth,bm.bmHeight);
}

void CCadBitmap::RestoreAspectRatio()
{
	CPoint P1,P2;
	double AspectRatioBM;
	AspectRatioBM = double(m_Size.cy)/double(m_Size.cx);
	m_P2.y = int(AspectRatioBM * double(abs(m_P2.x-m_P1.x) ) )+ m_P1.y;
	m_MaintainAspectRatio = 1;
}

void CCadBitmap::RenderEnable(int e)
{
	CCadBitmap::m_RenderEnable = e;
}

CPoint CCadBitmap::GetCenter()
{
	CRect rect = this->GetRect();
	return rect.CenterPoint();
}

// Moves the center of the object to the spcified point
void CCadBitmap::ChangeCenter(CSize p)
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


CSize CCadBitmap::GetSize()
{
	CRect rect = GetRect();
	return rect.Size();
}


void CCadBitmap::ChangeSize(CSize Sz)
{
	m_P2.x += Sz.cx;
	m_P2.y += Sz.cy;
}
