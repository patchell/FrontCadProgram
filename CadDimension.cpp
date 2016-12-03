#include "stdafx.h"
#include "FrontCad.h"
#include "CadDimension.h"
#include "FrontCadDefines.h"

int CCadDimension::m_RenderEnable = 1;

CCadDimension::CCadDimension():CCadObject(OBJECT_TYPE_DIMENSION)
{
	m_pLine = 0;
	m_pText = new CCadText;
	m_Dirty = 0;
	m_LastMode = 0;
	TextAttributes atrb;
	
	atrb.m_Angle = 0;
	atrb.m_BkColor = RGB(255, 255, 255);
	atrb.m_Color = RGB(0, 0, 0);
	atrb.m_FontHeight = 200;
	atrb.m_FontWidth = 0;
	atrb.m_Format = DT_BOTTOM | DT_SINGLELINE;
	atrb.m_pFontName = new char[LF_FACESIZE];
	strcpy_s(atrb.m_pFontName, LF_FACESIZE, "Arial");
	atrb.m_pText = new char[32];
	atrb.m_Transparent = 1;
	atrb.m_Weight = FW_DEMIBOLD;
	m_pText->SettAttrib(atrb);

}

CCadDimension::CCadDimension(CCadDimension &cd) :CCadObject(OBJECT_TYPE_DIMENSION)
{
	m_pLine = 0;
	m_P1 = cd.m_P1;
	m_P2 = cd.m_P2;
	m_pText = new CCadText(*cd.m_pText);
	m_Dirty = 0;
	m_LastMode = 0;

}

CCadDimension::~CCadDimension()
{
	if (m_pText) delete m_pText;
}

CCadDimension CCadDimension::operator=(CCadDimension &v)
{
	m_P1 = v.m_P1;
	m_P2 = v.m_P2;
	*m_pText = *v.m_pText;
	return *this;
}

void CCadDimension::Move(CPoint p)
{
	CPoint Diff;
	Diff = p - m_P1;
	m_P1 += Diff;
	m_P2 += Diff;
	m_pText->Move(m_pText->m_P1 + Diff);
}

void CCadDimension::Save(FILE *pO)
{
	fprintf(pO, "DIMENSION(POINT(%d,%d),POINT(%d,%d),WIDTH(%d),COLOR(%d,%d,%d)){\n", m_P1.x, m_P1.y, m_P2.x, m_P2.y,
		m_Atrib.m_Width,
		RED(m_Atrib.m_Color),
		GREEN(m_Atrib.m_Color),
		BLUE(m_Atrib.m_Color));
	m_pText->Save(pO);
	fprintf(pO, "}\n");
}

void CCadDimension::SetVertex(int Vi, CPoint p)
{
}

int CCadDimension::GrabVertex(CPoint p)
{
	int rV = -1;
	return rV;
}

void CCadDimension::Draw(CDC *pDC, int mode, CPoint Offset, CScale Scale)
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

	if (CCadDimension::m_RenderEnable)
	{
		P1 = (Scale * m_P1) + Offset;
		P2 = (Scale * m_P2) + Offset;
		Lw = int(Scale.m_ScaleX * m_Atrib.m_Width);
		if (Lw < 1) Lw = 1;
		if ((m_LastMode != mode) || m_Dirty)
		{
			if (m_pLine) delete m_pLine;
			switch (mode)
			{
			case OBJECT_MODE_FINAL:
				m_pLine = new CPen(PS_SOLID, Lw, m_Atrib.m_Color);
				break;
			case OBJECT_MODE_SELECTED:
				m_pLine = new CPen(PS_SOLID, Lw, RGB(0, 255, 50));
				break;
			case OBJECT_MODE_SKETCH:
				m_pLine = new CPen(PS_DOT, 1, RGB(0, 0, 255));
				break;
			}
		}
		switch (mode)
		{
		case OBJECT_MODE_SELECTED:
		case OBJECT_MODE_FINAL:
			pOld = pDC->SelectObject(m_pLine);
			pDC->MoveTo(P1);
			pDC->LineTo(P2);
			pDC->SelectObject(pOld);
			m_pText->Draw(pDC, mode, Offset, Scale);
			break;
		case OBJECT_MODE_SKETCH:
			pOld = pDC->SelectObject(m_pLine);
			pDC->MoveTo(P1);
			pDC->LineTo(P2);
			pDC->SelectObject(pOld);
			break;
		case OBJECT_MODE_ERASE:
			break;
		}
		m_LastMode = mode;
	}
}

int CCadDimension::CheckSelected(CPoint p,CSize O)
{
	int rV = 0;
	CPoint P1 = m_P1+O;
	CPoint P2 = m_P2+O;
	int s1, s2;
	if (P1.x == P2.x)
	{
		s1 = 20;
		s2 = 0;
	}
	else if (P1.y == P2.y)
	{
		s2 = 20;
		s1 = 0;
	}
	CSize rOff = CSize(s1, s2);
	P1 += rOff;
	P2 -= rOff;
	CRect rect;
	rect.SetRect(P1, P2);
	rect.NormalizeRect();
	rV = rect.PtInRect(p);
	rV |= m_pText->CheckSelected(p);
	return rV;
}

CPoint CCadDimension::GetReference()
{
	return m_P1;
}

void CCadDimension::MakeDirty(void)
{
	m_Dirty = 1;
	if (m_pText)m_pText->MakeDirty();
}

void CCadDimension::SetSelected(int Flag)
{
	m_Selected = Flag;
	m_pText->SetSelected(Flag);
}

void CCadDimension::AdjustRefernce(CPoint Ref)
{
	///-----------------------------------------
	///	AdjustRefernce
	///		Thhis function is used to normalize
	///	the location of points in the object
	/// relative to a point choseen on the
	/// drawing.
	///	parameters:
	///		Ref.....selected reference point
	///-----------------------------------------
	m_P1 -= Ref;
	m_P2 -= Ref;
	m_pText->AdjustRefernce(Ref);
	CPoint Org = ((CFrontCadApp *)AfxGetApp())->m_pMainView->GetOrigin();
	UpdateText(Org);
}

CRect CCadDimension::GetRect(void)
{
	CRect rR;
	return rR;
}

void CCadDimension::UpdateText(CPoint Org)
{
	CPoint P1, P2;
	P1 = m_P1;
	P2 = m_P2;
	int Dim,ofx;
	CRect rect;
	CSize off;

	if (P1.x == P2.x)	//vertical facing dimension
	{
		Dim = P1.x - Org.x;
		SetValue(Dim, 3);
		m_pText->SetAngle(-900);
		rect = m_pText->GetRect();
		if (P1.y > P2.y)
			ofx = -(rect.Width() + 60);
		else
			ofx = 60;
		off = CSize(-rect.Height() / 2, -ofx);
	}
	else if (P1.y == P2.y)	//horizontal facing dimension
	{
		Dim = -(P2.y - Org.y);
		SetValue(Dim, 3);
		rect = m_pText->GetRect();
		if (P1.x > P2.x)
			ofx = rect.Width()+60;
		else
			ofx = -60;
		off = CSize(ofx, rect.Height() / 2);
	}
	m_pText->m_P1 = P2 - off;
}

void CCadDimension::SetValue(int v, int dp)
{
	int Intpart, Fracpart, Div;

	Div = 1;

	int i;

	for (i = 0; i < dp; ++i)
		Div *= 10;
	Intpart = v / Div;
	Fracpart = v % Div;
	Fracpart = int(((double)Fracpart / (double)Div) * double(Div));
	char *s = new char[32];
	sprintf_s(s,32, "%d.%03d", Intpart,Fracpart);
	m_pText->SetText(s);
	delete[] s;

}
void CCadDimension::AddObject(CCadObject *pO)
{
	if (pO->GetType() == OBJECT_TYPE_TEXT)
	{
		m_pText = (CCadText *)pO;
	}
	else
	{
		MessageBox(NULL, "Only TEXT objects in DIMENSION", "ERROR", MB_OK | MB_ICONHAND);
	}
}

void CCadDimension::RemoveObject(CCadObject *pO)
{
	if (pO->GetType() == OBJECT_TYPE_TEXT)
	{
		delete m_pText;
		m_pText = 0;
	}
	else
	{
		MessageBox(NULL, "Remove Only TEXT objects in DIMENSION", "ERROR", MB_OK | MB_ICONHAND);
	}
}
void CCadDimension::RenderEnable(int e)
{
	CCadDimension::m_RenderEnable = e;
}


void CCadDimension::AlignToPoint(CPoint p)
{
	if (m_P1.x == m_P2.x)	//vertical dimension
	{
		m_P1.x = m_P2.x = p.x;
	}
	else if (m_P1.y == m_P2.y)	//Horizontal dimension
	{
		m_P1.y = m_P2.y = p.y;
	}
}

CPoint CCadDimension::GetCenter()
{
	return m_P1;
}

// Moves the center of the object to the spcified point
void CCadDimension::ChangeCenter(CSize p)
{
}


CSize CCadDimension::GetSize()
{
	CRect rect = GetRect();
	return rect.Size();
}


void CCadDimension::ChangeSize(CSize Sz)
{
}


void CCadDimension::SetFontName(char * pN)
{
	m_pText->SetFontName(pN);
}


char * CCadDimension::GetFontName()
{
	return m_pText->GetFontName();
}
