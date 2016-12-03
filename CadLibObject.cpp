// CadLibObject.cpp: implementation of the CCadLibObject class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "FrontCad.h"
#include "CadLibObject.h"
#include "FrontCadDefines.h"
#include "CadArcCentered.h"
#include "CadHoleRound.h"
#include "CadRectHole.h"
#include "CadHoleRnd1Flat.h"
#include "CadHoleRnd2flat.h"
#include "CadRoundRect.h"
#include "CadElipse.h"
#include "CadLine.h"
#include "CadRect.h"
#include "CadText.h"
#include "CadPolygon.h"
#include "CadPolygonFill.h"
#include "CadArc.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif


//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

int CCadLibObject::m_RenderEnable = 1;

CCadLibObject::CCadLibObject():CCadObject(OBJECT_TYPE_LIBCOMP)
{
	m_pStart = 0;
	m_pEnd = 0;
	m_pName = 0;
	m_Ref = CPoint(0,0);
	m_pRefPen = 0;
}

CCadLibObject::CCadLibObject(CCadObject *pChain)
{
	SetType(OBJECT_TYPE_LIBCOMP);
	m_Ref = CPoint(0,0);
	m_pStart = 0;
	m_pEnd = 0;
	m_pName = 0;
	m_pRefPen = 0;
	//copy from this object
	CCadObject *pLobj = pChain;
	while(pLobj)
	{
		switch(pLobj->GetType())
		{
			case OBJECT_TYPE_LINE:
				{
					CCadLine *pL;
					pL = (CCadLine *)((CCadLine *)pLobj)->CopyObject();
					AddObject(pL);
				}
				break;
			case OBJECT_TYPE_RECT:
				{
					CCadRect *pR = new CCadRect;
					*pR = *(CCadRect *)pLobj;
					AddObject(pR);
				}
				break;
			case OBJECT_TYPE_ELLIPSE:
				{
					CCadElipse *pE = new CCadElipse;
					*pE = *(CCadElipse *)pLobj;
					AddObject(pE);
				}
				break;
			case OBJECT_TYPE_RNDRECT:
				{
					CCadRoundRect *pR = new CCadRoundRect;
					*pR = *(CCadRoundRect *)pLobj;
					AddObject(pR);
				}
				break;
			case OBJECT_TYPE_POLY:
				{
					CCadPolygon *pP = new CCadPolygon;
					*pP = *(CCadPolygon *)pLobj;
					AddObject(pP);
				}
				break;
			case OBJECT_TYPE_POLYFILL:
				{
					CCadPolygonFill *pP = new CCadPolygonFill;
					*pP = *(CCadPolygonFill *)pLobj;
					AddObject(pP);
				}
				break;
			case OBJECT_TYPE_LIBCOMP:
				{
				}
				break;
			case OBJECT_TYPE_HOLEROUND:
				{
					CCadHoleRound *pH = new CCadHoleRound;
					*pH = *(CCadHoleRound *)pLobj;
					AddObject(pH);
				}
				break;
			case OBJECT_TYPE_HOLERECT:
				{
					CCadRectHole *pH = new CCadRectHole;
					*pH = *(CCadRectHole *)pLobj;
					AddObject(pH);
				}
				break;
			case OBJECT_TYPE_HOLERND1FLAT:
				{
					CCadHoleRnd1Flat *pH = new CCadHoleRnd1Flat;
					*pH = *(CCadHoleRnd1Flat *)pLobj;
					AddObject(pH);
				}
				break;
			case OBJECT_TYPE_HOLERND2FLAT:
				{
					CCadHoleRnd2Flat *pH = new CCadHoleRnd2Flat;
					*pH = *(CCadHoleRnd2Flat *)pLobj;
					AddObject(pH);
				}
				break;
			case OBJECT_TYPE_TEXT:
				{
					CCadText *pT = new CCadText;
					*pT = *(CCadText *)pLobj;
					AddObject(pT);
				}
				break;
			case OBJECT_TYPE_ARC:
				{
					CCadArc *pH = new CCadArc;
					*pH = *(CCadArc *)pLobj;
					AddObject(pH);
				}
				break;
			case OBJECT_TYPE_ARCCENTERED:
				{
					CCadArcCentered *pH = new CCadArcCentered;
					*pH = *(CCadArcCentered *)pLobj;
					AddObject(pH);
				}
				break;
			case OBJECT_TYPE_ARROW:
				{

				}
				break;
		}
		pLobj = pLobj->m_pNext;
	}
}

CCadLibObject::CCadLibObject(CCadLibObject &LibObj)
{
	SetType(OBJECT_TYPE_LIBCOMP);
	m_Ref = LibObj.m_Ref;
	m_pStart = 0;
	m_pEnd = 0;
	int len = strlen(LibObj.m_pName) + 1;
	m_pName = new char[len];
	strcpy_s(m_pName,len,LibObj.m_pName);
	m_pRefPen = 0;
	//copy from this object
	CCadObject *pLobj = LibObj.GetHead();
	while(pLobj)
	{
		switch(pLobj->GetType())
		{
			case OBJECT_TYPE_LINE:
				{
					CCadLine *pL = new CCadLine;
					*pL = *(CCadLine *)pLobj;
					AddObject(pL);
				}
				break;
			case OBJECT_TYPE_RECT:
				{
					CCadRect *pR = new CCadRect;
					*pR = *(CCadRect *)pLobj;
					AddObject(pR);
				}
				break;
			case OBJECT_TYPE_ELLIPSE:
				{
					CCadElipse *pE = new CCadElipse;
					*pE = *(CCadElipse *)pLobj;
					AddObject(pE);
				}
				break;
			case OBJECT_TYPE_RNDRECT:
				{
					CCadRoundRect *pR = new CCadRoundRect;
					*pR = *(CCadRoundRect *)pLobj;
					AddObject(pR);
				}
				break;
			case OBJECT_TYPE_POLY:
				{
					CCadPolygon *pP = new CCadPolygon;
					*pP = *(CCadPolygon *)pLobj;
					AddObject(pP);
				}
				break;
			case OBJECT_TYPE_POLYFILL:
				{
					CCadPolygonFill *pP = new CCadPolygonFill;
					*pP = *(CCadPolygonFill *)pLobj;
					AddObject(pP);
				}
				break;
			case OBJECT_TYPE_LIBCOMP:
				{
				}
				break;
			case OBJECT_TYPE_HOLEROUND:
				{
					CCadHoleRound *pH = new CCadHoleRound;
					*pH = *(CCadHoleRound *)pLobj;
					AddObject(pH);
				}
				break;
			case OBJECT_TYPE_HOLERECT:
				{
					CCadRectHole *pH = new CCadRectHole;
					*pH = *(CCadRectHole *)pLobj;
					AddObject(pH);
				}
				break;
			case OBJECT_TYPE_HOLERND1FLAT:
				{
					CCadHoleRnd1Flat *pH = new CCadHoleRnd1Flat;
					*pH = *(CCadHoleRnd1Flat *)pLobj;
					AddObject(pH);
				}
				break;
			case OBJECT_TYPE_HOLERND2FLAT:
				{
					CCadHoleRnd2Flat *pH = new CCadHoleRnd2Flat;
					*pH = *(CCadHoleRnd2Flat *)pLobj;
					AddObject(pH);
				}
				break;
			case OBJECT_TYPE_TEXT:
				{
					CCadText *pT = new CCadText;
					*pT = *(CCadText *)pLobj;
					AddObject(pT);
				}
				break;
			case OBJECT_TYPE_ARC:
				{
					CCadArc *pH = new CCadArc;
					*pH = *(CCadArc *)pLobj;
					AddObject(pH);
				}
				break;
			case OBJECT_TYPE_ARCCENTERED:
				{
					CCadArcCentered *pH = new CCadArcCentered;
					*pH = *(CCadArcCentered *)pLobj;
					AddObject(pH);
				}
				break;
			case OBJECT_TYPE_ARROW:
				{

				}
				break;

		}
		pLobj = pLobj->m_pNext;
	}
}

CCadLibObject::~CCadLibObject()
{
	CCadObject *pO,*pNO;

	pO = m_pStart;
	while(pO)
	{
		pNO = pO->m_pNext;
		delete pO;
		pO = pNO;
	}
	if(m_pName) delete[] m_pName;
	if(m_pRefPen) delete m_pRefPen;
}

void CCadLibObject::Draw(CDC *pDC, int mode,CPoint Offset,CScale Scale)
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
	if (CCadLibObject::m_RenderEnable)
	{
		CCadObject *pO = m_pStart;
		CPoint p1;
		p1 = Scale * m_P1;
		CPoint Doff = p1 + Offset;
		while (pO)
		{
			pO->Draw(pDC, mode, Doff, Scale);
			pO = pO->m_pNext;
		}
		///--------------------------
		/// mark the reference point
		///--------------------------
		if (m_pRefPen == 0)
		{
			m_pRefPen = new CPen;
			m_pRefPen->CreatePen(PS_SOLID, 3, RGB(255, 0, 0));
		}
		CPen *pOldPen;
		CPoint Ref = m_Ref + Offset + m_P1;
		pOldPen = pDC->SelectObject(m_pRefPen);
		pDC->MoveTo(Ref.x - 2, Ref.y);
		pDC->LineTo(Ref.x + 2, Ref.y);
		pDC->MoveTo(Ref.x, Ref.y - 2);
		pDC->LineTo(Ref.x, Ref.y + 2);
		pDC->SelectObject(pOldPen);
	}
}

int CCadLibObject::CheckSelected(CPoint p, CSize Offset)
{
	int rV = 0;
	CCadObject *pO = GetHead();
	Offset = m_P1+Offset;
	while(pO && !rV)
	{
		if(pO->CheckSelected(p,Offset)) rV = 1;
		else
			pO = pO->m_pNext;
	}
	return rV;
}

void CCadLibObject::AddObject(CCadObject *pObj)
{
	if(m_pStart)
	{
		///--------------------------
		///add object to end of list
		///--------------------------
		m_pEnd->m_pNext = pObj;
		pObj->m_pPrev = m_pEnd;
		m_pEnd = pObj;
	}
	else	///first object
	{
		m_pStart = pObj;
		m_pEnd = pObj;
	}
	if(pObj->GetType() == OBJECT_TYPE_ARCCENTERED)
	{
		CCadArcCentered *pA = (CCadArcCentered *)pObj;
		m_P2 = CPoint(pA->m_P2);
	}
}

void CCadLibObject::RemoveObject(CCadObject *pO)
{
}

void CCadLibObject::Save(FILE *pO)
{
	fprintf(pO,"LIBPART(\"%s\",REF(%d,%d),POINT(%d,%d) ){\n",
		m_pName,m_Ref.x,m_Ref.y,m_P1.x,m_P1.y);
	CCadObject *pOb = m_pStart;
	while(pOb)
	{
		pOb->Save(pO);
		pOb = pOb->m_pNext;
	}
	fprintf(pO,"}\n");
}

void CCadLibObject::SetName(char *pName)
{
	if(m_pName) delete[] m_pName;
	int len = strlen(pName) + 1;
	m_pName = new char[len];
	strcpy_s(m_pName,len,pName);
}

void CCadLibObject::Move(CPoint p)
{
	m_P1 = p;
}

int CCadLibObject::GrabVertex(CPoint point)
{
	return -1;
}

void CCadLibObject::SetVertex(int Vi, CPoint p)
{

}

CPoint CCadLibObject::GetReference()
{
	return m_P1 + m_Ref;
}

void CCadLibObject::MakeDirty()
{
	m_Dirty = 1;
	CCadObject *pCO = this->m_pStart;
	while(pCO)
	{
		pCO->MakeDirty();
		pCO = pCO->m_pNext;
	}
}

void CCadLibObject::AdjustRefernce(CPoint Ref)
{
	m_P1 -= Ref;
}

CRect CCadLibObject::GetRect()
{
	CRect rect;
	CSize d = m_P2;
	rect.SetRect(m_P1 + d,m_P1-d);
	rect.NormalizeRect();
	return rect;
}

void CCadLibObject::RenderEnable(int e)
{
	CCadLibObject::m_RenderEnable = e;
}

CPoint CCadLibObject::GetCenter()
{
	CRect rect = this->GetRect();
	return rect.CenterPoint();
}

// Moves the center of the object to the spcified point
void CCadLibObject::ChangeCenter(CSize p)
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



CSize CCadLibObject::GetSize()
{
	CRect rect = GetRect();
	return rect.Size();
}


void CCadLibObject::ChangeSize(CSize Sz)
{
}
