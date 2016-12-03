#include "stdafx.h"
#include "CadOrigin.h"
#include "FrontCadView.h"
#include "FrontCadDefines.h"

int CCadOrigin::m_RenderEnable=1;

CCadOrigin::CCadOrigin() :CCadObject(OBJECT_TYPE_ORIGIN)
{
	m_pMainView = 0;
	m_pPenLine = 0;
	m_Atrib.m_Width = 1;
	m_Atrib.m_Color = RGB(0, 0, 0);
}

CCadOrigin::CCadOrigin(CCadOrigin &v) : CCadObject(OBJECT_TYPE_ORIGIN)
{
	m_pMainView = 0;
	m_pPenLine = 0;
	m_P1 = v.m_P1;
}

CCadOrigin::~CCadOrigin()
{
	if (m_pPenLine) delete m_pPenLine;
}

void CCadOrigin::Move(CPoint p)
{
	m_P1 = p;
}

void CCadOrigin::Save(FILE *pO)
{
	fprintf(pO, "ORIGIN(POINT(%d,%d))\n", m_P1.x, m_P1.y);
}

void CCadOrigin::Draw(CDC *pDC, int mode, CPoint Offset, CScale Scale)
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
	CPoint P1, P2;
	int Lw;

	if (CCadOrigin::m_RenderEnable)
	{
		P1 = (Scale * m_P1) + Offset + CSize(10, 10);
		P2 = (Scale * m_P1) + Offset - CSize(10, 10);
		Lw = int(Scale.m_ScaleX * m_Atrib.m_Width);
		if (Lw < 1) Lw = 1;
		rect.SetRect(P1, P2);
		rect.NormalizeRect();
		if ((m_LastMode != mode) || m_Dirty)
		{
			if (m_pPenLine) delete m_pPenLine;
			switch (mode)
			{
			case OBJECT_MODE_FINAL:
				m_pPenLine = new CPen(PS_SOLID, Lw, m_Atrib.m_Color);
				break;
			case OBJECT_MODE_SELECTED:
				m_pPenLine = new CPen(PS_SOLID, Lw, RGB(0, 255, 50));
				break;
			case OBJECT_MODE_SKETCH:
				m_pPenLine = new CPen(PS_DOT, 1, RGB(0, 0, 255));
				break;
			}
		}
		switch (mode)
		{
		case OBJECT_MODE_FINAL:
		case OBJECT_MODE_SELECTED:
		case OBJECT_MODE_SKETCH:
			pOld = pDC->SelectObject(m_pPenLine);
			pDC->Ellipse(&rect);
			P1 = (Scale * m_P1) + Offset;
			pDC->MoveTo(P1.x, P1.y + 15);
			pDC->LineTo(P1.x, P1.y - 15);
			pDC->MoveTo(P1.x + 15, P1.y);
			pDC->LineTo(P1.x - 15, P1.y);
			pDC->SelectObject(pOld);
			break;
		case OBJECT_MODE_ERASE:
			break;
		}
		m_LastMode = mode;
	}
}

int CCadOrigin::CheckSelected(CPoint p,CSize O)
{
	int rV = 0;
	CRect rect;
	CPoint P1, P2;
	P1 = m_P1+O;
	P2 = m_P1+O;
	P1 += CSize(100, 100);
	P2 -= CSize(100, 100);
	rect.SetRect(P1, P2);
	rect.NormalizeRect();
	rV = rect.PtInRect(p);
	return rV;
}

CPoint CCadOrigin::GetReference()
{
	return m_P1;
}

void CCadOrigin::MakeDirty(void)
{
	m_Dirty = 1;
}

void CCadOrigin::SetSelected(int Flag)
{
	m_Selected = Flag;
}

void CCadOrigin::AdjustRefernce(CPoint Ref)
{
	m_P1 -= Ref;
	if (m_pMainView) m_pMainView->PostMessageA(WM_UPDATEDIMENSIONS);
}

CRect CCadOrigin::GetRect(void)
{
	CRect rR;
	CPoint P1, P2;
	P1 = m_P1 + CSize(100, 100);
	P2 = m_P1 - CSize(100, 100);
	rR.SetRect(P1, P2);
	rR.NormalizeRect();
	return rR;
}

void CCadOrigin::RenderEnable(int e)
{
	CCadOrigin::m_RenderEnable = e;
}

CPoint CCadOrigin::GetCenter()
{
	return m_P1;
}

// Moves the center of the object to the spcified point
void CCadOrigin::ChangeCenter(CSize p)
{
	m_P1 -= p;
}


CSize CCadOrigin::GetSize()
{
	CRect rect = GetRect();
	return rect.Size();
}


void CCadOrigin::ChangeSize(CSize Sz)
{
	m_P2.x += Sz.cx;
	m_P2.y += Sz.cy;
}
