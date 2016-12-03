// CadObject.cpp: implementation of the CCadObject class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "FrontCad.h"
#include "CadObject.h"
#include "FrontCadDefines.h"
#include "CadLibObject.h"
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
#include "CadBitmap.h"
#include "CadOrigin.h"
#include "CadDimension.h"
#include "CadPrintRect.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif


//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CCadObject::CCadObject()
{
	m_pNext = 0;
	m_pPrev = 0;
	m_pSelNext = 0;
	m_LastMode = -1;
	m_Selected = 0;
	m_Type = OBJECT_TYPE_NONE;
	m_Dirty = 0;
	m_P1 = CPoint(0,0);
	m_P2 = CPoint(0,0);
}

CCadObject::CCadObject(int type)
{
	m_pNext = 0;
	m_pPrev = 0;
	m_pSelNext = 0;
	m_LastMode = -1;
	m_Selected = 0;
	m_Type = type;
	m_Dirty = 0;
	m_P1 = CPoint(0,0);
	m_P2 = CPoint(0,0);
}

CCadObject::~CCadObject()
{

}

void CCadObject::Draw(CDC *pDC,int mode,CPoint Offset,CScale Scale)
{
	///---------------------------------------------
	///	Draw
	///		This function draws the object onto the
	///	specified device context.
	/// Base Class Methode does nothing
	///
	/// parameters:
	///		pDC......pointer to the device context
	///		mode.....mode to use when drawing
	///		Offset...Offset to add to points
	///		Scale....Sets Units to Pixels ratio
	///---------------------------------------------

}

int CCadObject::CheckSelected(CPoint p,CSize Offset)
{
	return 0;
}

void CCadObject::Save(FILE *pO)
{

}

CCadObject CCadObject::operator=(CCadObject &v)
{
	return *this;
}

void CCadObject::Move(CPoint p)
{
}

int CCadObject::GrabVertex(CPoint point)
{
	return -1;
}

void CCadObject::SetVertex(int Vi, CPoint p)
{

}

CPoint CCadObject::GetReference()
{
	return CPoint(0,0);
}

CPoint CCadObject::ScalePoint(CPoint p, int Scale, int ScaleDiv)
{
	p.x = (Scale * p.x)/ ScaleDiv;
	p.y = (Scale * p.y)/ ScaleDiv;
	return p;
}

void CCadObject::AddObject(CCadObject *pO)
{
}

void CCadObject::RemoveObject(CCadObject *pO)
{
}

void CCadObject::MakeDirty()
{
	this->m_Dirty = 1;
}

void CCadObject::SetSelected(int Flag)
{
	m_Selected = Flag;
}

static char *TypeString[] = {
"None",
"Line",
"Rect",
"Ellipse",
"Round Rect",
"Polygon",
"Filled Polybon",
"Part",
"Round Hole",
"Rect Hole",
"Round Hole 1 Flat",
"Round Hole 2 Flats",
"Text",
"Arc",
"Centered Arc",
"Drawing",
"Bitmap",
"Arrow",
"Origin",
"Dimension",
"Print Rectangle"
};

char * CCadObject::GetTypeString()
{
	return TypeString[m_Type];
}

void CCadObject::AdjustRefernce(CPoint p)
{
}

CCadObject * CCadObject::CopyObject(void)
{
	CCadObject *pRobj;

	switch (this->m_Type)
	{
	case OBJECT_TYPE_LINE:
	{
		CCadLine *pL = new CCadLine;
		*pL = *(CCadLine *)this;
		pRobj = pL;
	}
	break;
	case OBJECT_TYPE_RECT:
	{
		CCadRect *pR = new CCadRect;
		*pR = *(CCadRect *)this;
		pRobj = pR;
	}
	break;
	case OBJECT_TYPE_ELLIPSE:
	{
		CCadElipse *pE = new CCadElipse;
		*pE = *(CCadElipse *)this;
		pRobj = pE;
	}
	break;
	case OBJECT_TYPE_RNDRECT:
	{
		CCadRoundRect *pR = new CCadRoundRect;
		*pR = *(CCadRoundRect *)this;
		pRobj = pR;
	}
	break;
	case OBJECT_TYPE_POLY:
	{
		CCadPolygon *pP = new CCadPolygon;
		*pP = *(CCadPolygon *)this;
		pRobj = pP;
	}
	break;
	case OBJECT_TYPE_POLYFILL:
	{
		CCadPolygonFill *pP = new CCadPolygonFill;
		*pP = *(CCadPolygonFill *)this;
		pRobj = pP;
	}
	break;
	case OBJECT_TYPE_LIBCOMP:
	{
		CCadLibObject *pLO = new CCadLibObject(((CCadLibObject *)this)->GetHead());
		pLO->m_P1 = ((CCadLibObject *)this)->m_P1;
		pLO->SetName(((CCadLibObject *)this)->m_pName);
		pLO->m_Ref = ((CCadLibObject *)this)->m_Ref;
		pRobj = pLO;
	}
	break;
	case OBJECT_TYPE_HOLEROUND:
	{
		CCadHoleRound *pH = new CCadHoleRound;
		*pH = *(CCadHoleRound *)this;
		pRobj = pH;
	}
	break;
	case OBJECT_TYPE_HOLERECT:
	{
		CCadRectHole *pH = new CCadRectHole;
		*pH = *(CCadRectHole *)this;
		pRobj = pH;
	}
	break;
	case OBJECT_TYPE_HOLERND1FLAT:
	{
		CCadHoleRnd1Flat *pH = new CCadHoleRnd1Flat;
		*pH = *(CCadHoleRnd1Flat *)this;
		pRobj = pH;
	}
	break;
	case OBJECT_TYPE_HOLERND2FLAT:
	{
		CCadHoleRnd2Flat *pH = new CCadHoleRnd2Flat;
		*pH = *(CCadHoleRnd2Flat *)this;
		pRobj = pH;
	}
	break;
	case OBJECT_TYPE_TEXT:
	{
		CCadText *pT = new CCadText;
		*pT = *(CCadText *)this;
		pRobj = pT;
	}
	break;
	case OBJECT_TYPE_ARC:
	{
		CCadArc *pH = new CCadArc;
		*pH = *(CCadArc *)this;
		pRobj = pH;
	}
	case OBJECT_TYPE_ARCCENTERED:
	{
		CCadArcCentered *pH = new CCadArcCentered;
		*pH = *(CCadArcCentered *)this;
		pRobj = pH;
	}
	break;
	case OBJECT_TYPE_BITMAP:
	{
		CCadBitmap *pBM = new CCadBitmap;
		*pBM = *(CCadBitmap *)this;
		pRobj = pBM;
	}
	break;
	case OBJECT_TYPE_ARROW:
	{
		CCadArrow *pA = new CCadArrow;
		*pA = *(CCadArrow *)this;
		pRobj = pA;
	}
	break;
	case OBJECT_TYPE_ORIGIN:
			{
				CCadOrigin *pO = new CCadOrigin;
				*pO = *(CCadOrigin *)this;
				pRobj = pO;
			}
			break;
		case OBJECT_TYPE_DIMENSION:
			{
				CCadDimension *pD = new CCadDimension;
				*pD = *(CCadDimension *)this;
				pRobj = pD;
			}
			break;
		case OBJECT_TYPE_PRINTRECT:
			{
				CCadPrintRect *pR = new CCadPrintRect;
				*pR = *(CCadPrintRect *)this;
				pRobj = pR;
			}
			break;
	}
	pRobj->m_pNext = 0;
	pRobj->m_pPrev = 0;
	pRobj->m_pSelNext = 0;
	return pRobj;
}

CRect CCadObject::GetRect()
{
	CRect Rect;
	Rect.SetRect(m_P1,m_P2);
	Rect.NormalizeRect();
	return Rect;
}

void CCadObject::SetRect(CRect & rect, CPoint P1, CPoint P2, CSize Lw)
{
	if (((P1.x > P2.x) && (P1.y > P2.y)))
	{
		CPoint temp;
		temp = P2;
		P2 = P1;
		P1 = temp;
	}
	else if (((P1.x > P2.x) && (P1.y < P2.y)))
	{
		CPoint t1, t2;
		t1 = P1;
		t2 = P2;
		P1.x = t2.x;
		P2.x = t1.x;
	}
	else if ((P1.x < P2.x) && (P1.y > P2.y))
	{
		CPoint t1, t2;
		t1 = P1;
		t2 = P2;
		P1.y = t2.y;
		P2.y = t1.y;
	}
	rect.SetRect(P1 + Lw, P2 - Lw + CSize(1, 1));
}

void CCadObject::RenderEnable(int e)
{
}


CCadObject * CCadObject::PopHead()
{
	return nullptr;
}


CCadObject * CCadObject::PopTail()
{
	return nullptr;
}


CPoint CCadObject::GetCenter()
{
	return CPoint();
}


// Moves the center of the object to the spcified point
void CCadObject::ChangeCenter(CSize p)
{
}


CSize CCadObject::GetSize()
{
	return CSize();
}


void CCadObject::ChangeSize(CSize Sz)
{
}
