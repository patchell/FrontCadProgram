// CadText.cpp: implementation of the CCadText class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "FrontCad.h"
#include "CadText.h"
#include <string.h>
#include <math.h>
#include "FrontCadDefines.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

extern FILE *pO;
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

int CCadText::m_RenderEnable = 1;

CCadText::CCadText():CCadObject(OBJECT_TYPE_TEXT)
{
	m_atrb.m_pText = 0;
	m_atrb.m_pFontName = new char[LF_FACESIZE];
	m_pFont = 0;
	m_pSelPen = 0;
	m_atrb.m_Transparent = 1;

}

CCadText::CCadText(CCadText& v):CCadObject(OBJECT_TYPE_TEXT)
{

	m_atrb.m_pText = 0;
	m_atrb.m_pFontName = new char[LF_FACESIZE];
	m_pFont = 0;
	m_pSelPen = 0;
	m_P1 = v.m_P1;
	CopyAttributes(&this->m_atrb,&v.m_atrb);
	this->m_SelRect = v.m_SelRect;
}

CCadText::~CCadText()
{
	if(m_atrb.m_pText) delete[] m_atrb.m_pText;
	if(m_atrb.m_pFontName) delete[] m_atrb.m_pFontName;
	if(m_pFont) delete m_pFont;
}

void CCadText::Draw(CDC *pDC, int mode,CPoint Offset,CScale Scale)
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
	CFont *pOldFont;
	CPen *pOldPen;
	COLORREF OldColor,OldBk;
	int FontHeight,FontWidth;
	CPoint P1;
	int OldMode;

	if (CCadText::m_RenderEnable)
	{
		P1 = (Scale * m_P1) + Offset;
		FontHeight = int(Scale.m_ScaleX * m_atrb.m_FontHeight);
		FontWidth = int(Scale.m_ScaleX * m_atrb.m_FontWidth);
		if (m_LastMode != mode || m_Dirty || (m_LastScaleX != Scale.m_ScaleX))
		{
			CRect rect = GetTextRectangle(pDC, Scale);
			Rotate(m_atrb.m_Angle, rect, m_SelRect);
			switch (mode)
			{
			case OBJECT_MODE_SELECTED:
				if (m_pSelPen == 0)
				{
					m_pSelPen = new CPen;
					m_pSelPen->CreatePen(PS_SOLID, 1, RGB(255, 0, 0));
				}
			case OBJECT_MODE_SKETCH:
			case OBJECT_MODE_FINAL:
				if (this->m_pFont) delete m_pFont;
				m_pFont = new CFont;
				if (Scale.m_ScaleY > 0.0)
					m_pFont->CreateFont(FontHeight, FontWidth, m_atrb.m_Angle, m_atrb.m_Angle,
						m_atrb.m_Weight, 0, 0, 0, DEFAULT_CHARSET, OUT_CHARACTER_PRECIS,
						CLIP_CHARACTER_PRECIS, PROOF_QUALITY, DEFAULT_PITCH,
						m_atrb.m_pFontName);
				else
					m_pFont->CreateFont(FontHeight, FontWidth, -m_atrb.m_Angle, -m_atrb.m_Angle,
						m_atrb.m_Weight, 0, 0, 0, DEFAULT_CHARSET, OUT_CHARACTER_PRECIS,
						CLIP_CHARACTER_PRECIS, PROOF_QUALITY, DEFAULT_PITCH,
						m_atrb.m_pFontName);

				break;
			}
			m_Dirty = 0;
			m_LastMode = mode;
			m_LastScaleX = Scale.m_ScaleX;
		}
		OldColor = pDC->SetTextColor(m_atrb.m_Color);
		OldBk = pDC->SetBkColor(m_atrb.m_BkColor);
		pOldFont = pDC->SelectObject(m_pFont);
		if (m_atrb.m_Transparent)
			OldMode = pDC->SetBkMode(TRANSPARENT);
		else
			OldMode = pDC->SetBkMode(OPAQUE);
		pDC->TextOut(P1.x, P1.y, m_atrb.m_pText, strlen(m_atrb.m_pText));
		pDC->SetBkMode(OldMode);
		if (mode == OBJECT_MODE_SELECTED)
		{
			pOldPen = pDC->SelectObject(m_pSelPen);
			pDC->MoveTo(Scale * m_SelRect.m_pVertex[0] + Offset);
			pDC->LineTo(Scale * m_SelRect.m_pVertex[1] + Offset);
			pDC->LineTo(Scale * m_SelRect.m_pVertex[2] + Offset);
			pDC->LineTo(Scale * m_SelRect.m_pVertex[3] + Offset);
			pDC->LineTo(Scale * m_SelRect.m_pVertex[0] + Offset);
			pDC->SelectObject(pOldPen);
		}
		pDC->SetBkColor(OldBk);
		pDC->SetTextColor(OldColor);
		pDC->SelectObject(pOldFont);
	}
}

void CCadText::SetText(char *s)
{
	///----------------------------------------
	/// SetText
	///		This function changes the text that
	/// is displayed by the CCadTexst oibject
	///
	/// parameter:
	///		s......pointer to string to display
	///----------------------------------------
	if(m_atrb.m_pText) delete[] m_atrb.m_pText;
	int len = strlen(s) + 1;
	m_atrb.m_pText = new char[len];
	strcpy_s(m_atrb.m_pText,len,s);
	/// create the selection rectangle
	CRect rect = GetTextRectangle();
	Rotate(m_atrb.m_Angle,rect,m_SelRect);
}

void CCadText::SetAngle(int e)
{
	///----------------------------------------
	/// SetAngle
	///		This function sets the angle to paint
	/// the text at.
	///
	/// parameters:
	///		e.....Angle, in tenths of a degree
	///----------------------------------------
	m_atrb.m_Angle = e;
	CRect rect = GetTextRectangle();
	this->Rotate(m_atrb.m_Angle,rect,this->m_SelRect);
}

void CCadText::GetText(char *s,int n)
{
	///---------------------------------------
	/// GetText
	///		This function retrieves the text
	/// thatis being displayed.
	///
	/// parameters:
	///		s.....pointer to array to put text into
	///		n.....maximum number of chars to copy
	///---------------------------------------
	strncpy(s,m_atrb.m_pText,n);
}

void CCadText::SettAttrib(TextAttributes &atrb)
{
	///--------------------------------------
	/// AetAttrib
	///
	///		This function sets the CCadText
	/// attributes.
	///
	/// parameters:
	///		atrb....reference to attribute structure
	///--------------------------------------
	m_atrb.m_Color = atrb.m_Color;
	m_atrb.m_BkColor = atrb.m_BkColor;
	m_atrb.m_Angle = atrb.m_Angle;
	m_atrb.m_FontHeight = atrb.m_FontHeight;
	m_atrb.m_FontWidth = atrb.m_FontWidth;
	m_atrb.m_Transparent = atrb.m_Transparent;
	m_atrb.m_Weight = atrb.m_Weight;
	SetFontName(atrb.m_pFontName);
	SetText(atrb.m_pText);
}

void CCadText::Save(FILE *pO)
{
	///-----------------------------------------
	/// Save
	///		This is the functtion that is called
	/// when ever you save a file or library
	///
	/// parameter:
	///		pO...Pointer to file to store data
	///-----------------------------------------
	fprintf(pO,"TEXT(\"%s\",POINT(%d,%d),COLOR(%d,%d,%d),COLOR(%d,%d,%d),FONT(\"%s\"),WEIGHT(%d),HEIGHT(%d),WIDTH(%d),ANGLE(%d),TRANSPARENT(%d))\n",
		m_atrb.m_pText,m_P1.x,m_P1.y,(m_atrb.m_Color & 0x0ff0000) >> 16,
		(m_atrb.m_Color & 0x0ff00) >> 8,m_atrb.m_Color & 0x0ff,
		(m_atrb.m_BkColor & 0x0ff0000) >> 16,
		(m_atrb.m_BkColor & 0x0ff00) >> 8,m_atrb.m_BkColor & 0x0ff,
		m_atrb.m_pFontName,m_atrb.m_Weight,
		m_atrb.m_FontHeight,m_atrb.m_FontWidth,
		m_atrb.m_Angle,m_atrb.m_Transparent);
}

void CCadText::SetFontName(char *s)
{
	///-----------------------------------------
	/// SetFontName
	///		This function is useed to set the
	/// name of the font that is used to paint
	/// the text.
	///
	/// parameter:
	///		s.....pointer to new font name
	///-----------------------------------------
	strcpy_s(m_atrb.m_pFontName,LF_FACESIZE,s);
}

int CCadText::CheckSelected(CPoint p,CSize O)
{
	///-------------------------------------------
	/// CheckSelected
	///		This function is used to check to see
	/// if the mouse pointer is over this object.
	/// It does not check to see if the m_Selected
	/// is set and does not set that flag.
	///
	/// parameter:
	///		p......point to check
	///-------------------------------------------
	return m_SelRect.PointEnclosed(p);
}

TextAttributes * CCadText::GetAttributes()
{
	///-------------------------------------------
	/// GetAttributes
	///		This function gets a pointer to the
	///	text attribute structure
	///-------------------------------------------
	return &m_atrb;
}

void CCadText::Rotate(int Angle,CRect rect,CCadPolygon &Poly)
{
	///----------------------------------------------
	/// Rotate
	///		Thhis function rotates the selection
	/// rectangle.
	///
	///		parameters:
	///			Angle....Angle to rotate by in tenths of a degree
	///			rect.....None Rotated Rectangle
	///			Poly.....Reference to a Polygon that
	///						The rotated rectangle is put in
	///----------------------------------------------
	Poly.Reset();
	double Ang = double(Angle)/ 10.0;
	double Pi = atan(1.0) * 4;
	Ang = Ang * Pi / 180.0;
	CPoint p,p1;
	p.x = (int)(rect.Width() * cos(Ang)) + rect.TopLeft().x;
	p.y = -(int)(rect.Width() * sin(Ang)) + rect.TopLeft().y;
	Poly.AddPoint(p);
	Poly.AddPoint(rect.TopLeft());
	p1.x = (int)(rect.Height() * sin(Ang)) + rect.TopLeft().x;
	p1.y = (int)(rect.Height() * cos(Ang)) + rect.TopLeft().y;
	Poly.AddPoint(p1);
	p.x = (int)(rect.Width() * cos(Ang)) + rect.TopLeft().x;
	p.y = -(int)(rect.Width() * sin(Ang)) + rect.TopLeft().y;
	p = p + p1 - rect.TopLeft();
	Poly.AddPoint(p);
}

CCadText CCadText::operator=(CCadText &v)
{
	///-----------------------------------------------
	/// operator=
	///		This function overides the assignment
	///	operator to make a copy of the CCadText
	/// object.
	///
	/// parameter:
	///		v.....reference to CCadText object to copy
	///-----------------------------------------------
	m_atrb.m_pText = 0;
	m_pFont = 0;
	m_pSelPen = 0;
	m_P1 = v.m_P1;
	CopyAttributes(&this->m_atrb,&v.m_atrb);
	this->m_SelRect =v.m_SelRect;
	return *this;
}

void CCadText::CopyAttributes(TextAttributes *d, TextAttributes *s)
{
	///------------------------------------------------
	/// CopyAttriutes
	///		This function copies a TextAttributes
	/// Structure.
	///
	///	parameters:
	///		d.....Detination pointer
	///		s.....Source Pointer
	///------------------------------------------------
	d->m_Color = s->m_Color;
	d->m_BkColor = s->m_BkColor;
	d->m_Angle = s->m_Angle;
	d->m_FontHeight = s->m_FontHeight;
	d->m_FontWidth = s->m_FontWidth;
	d->m_Format = s->m_Format;
	d->m_Transparent = s->m_Transparent;
	d->m_Weight = s->m_Weight;
	strcpy_s(d->m_pFontName, LF_FACESIZE,s->m_pFontName);
	if(s->m_pText)
	{
		if(d->m_pText) delete[] d->m_pText;
		int len = strlen(s->m_pText) + 1;
		d->m_pText = new char[len];
		strcpy_s(d->m_pText,len,s->m_pText);
	}
}

void CCadText::Move(CPoint p)
{
	///-----------------------------------------------
	/// Move
	///		Move the object to a new location.
	///
	/// parameters:
	///		p.....New location.
	///-----------------------------------------------
	m_P1 = p;
	CRect rect = GetTextRectangle();
	Rotate(m_atrb.m_Angle,rect,m_SelRect);
}

int CCadText::GrabVertex(CPoint point)
{
	///-----------------------------------------------
	/// GrabVertex
	///		This function returns an index to a vertex
	/// that the mouse is over.  CCadText does not
	/// have any so it always returns negatory.
	///
	/// parameters:
	///		point....point to check against.
	///		Scale....current scale factor
	/// returns:
	///		Returns -1 if no vertex is selected
	///		Returns an index , starting at 0 and uup
	///		if a vertex is selected
	///-----------------------------------------------
	return -1;
}

void CCadText::SetVertex(int Vi, CPoint p)
{
	///-----------------------------------------------
	/// SetVertex
	///		This function is used to move a vertex to
	/// a new location.
	///
	///	parameters:
	///		Vi.....Vertex Index
	///		p......New location
	///-----------------------------------------------
}

CPoint CCadText::GetReference()
{
	///-----------------------------------------
	///	GetReference
	///		This function was added mostly so that
	/// I could have a virtual function to do this
	/// for all CCadObjects.  Generally, it is jmust
	/// m_P1.
	///
	///	returns:
	///		m_P1
	///-----------------------------------------
	return m_P1;
}

char * CCadText::GetText()
{
	///----------------------------------------
	/// GetText
	///		This function returns a pointer to
	/// the string that is being displayed by
	/// by the text object
	///----------------------------------------
	return this->m_atrb.m_pText;
}


CRect CCadText::GetTextRectangle()
{
	///------------------------------------------
	/// GetTextRectangle
	///		This function is used to get the
	/// rectangle that surrounds the text displayed
	/// bu the text object.
	///------------------------------------------
	CWnd *pW = CWnd::GetActiveWindow();
	CDC *pDC = pW->GetDC();
	//	CDC *pDC = ((CFrontCadApp *)AfxGetApp())->m_pMainView->GetDC();
	CFont font,*old;
	font.CreateFont(m_atrb.m_FontHeight,m_atrb.m_FontWidth,0,0,
			m_atrb.m_Weight,0,0,0,DEFAULT_CHARSET,OUT_CHARACTER_PRECIS,
			CLIP_CHARACTER_PRECIS,PROOF_QUALITY,DEFAULT_PITCH,
			m_atrb.m_pFontName);
	old = pDC->SelectObject(&font);
	CSize cz = pDC->GetTextExtent(m_atrb.m_pText,strlen(m_atrb.m_pText));
	pDC->SelectObject(old);
	CRect rect;
	rect.SetRect(m_P1,m_P1 + CPoint(cz.cx,cz.cy));
	char *s = new char[256];
	sprintf(s, "Text Rect:p1=%d,%d p2=%d,%d", rect.left, rect.top, rect.right, rect.bottom);
	((CFrontCadApp *)AfxGetApp())->WriteToLogFile(s);

	delete[] s;
	return rect;
}

CRect CCadText::GetTextRectangle(CDC *pDC, CScale Scale)
{
	///------------------------------------------
	/// GetTextRectangle
	///		This function is used to get the
	/// rectangle that surrounds the text displayed
	/// bu the text object.
	///
	///	parameters:
	///		pDC.....pointer to the device context
	///		Scale...Scale factor for display
	///------------------------------------------
	CFont font,*old;
	font.CreateFont(int(m_atrb.m_FontHeight * Scale.m_ScaleX),int(m_atrb.m_FontWidth * Scale.m_ScaleX),0,0,
			m_atrb.m_Weight,0,0,0,DEFAULT_CHARSET,OUT_CHARACTER_PRECIS,
			CLIP_CHARACTER_PRECIS,PROOF_QUALITY,DEFAULT_PITCH,
			m_atrb.m_pFontName);
	old = pDC->SelectObject(&font);
	CSize cz = pDC->GetTextExtent(m_atrb.m_pText,strlen(m_atrb.m_pText));
	pDC->SelectObject(old);
	CRect rect;
	Scale.m_ScaleX = 1.0/Scale.m_ScaleX;
	Scale.m_ScaleY = 1.0/Scale.m_ScaleY;
	cz = Scale * cz;
	rect.SetRect(m_P1,m_P1 + CPoint(cz.cx,cz.cy));
	return rect;
}

CPoint CCadText::CalcTextShiftonRotation(CPoint p1, CPoint Center, double angle)
{
	///--------------------------------------------
	///	CalcTextShiftonRotation
	///		This function is just a little complicated.
	/// What this does is it calculates the point to
	/// draw text at for a rotated text object.  This
	/// makes it so the text apears to be centered
	/// at point p1 at the given angle from the
	/// center.
	///
	/// parameters:
	///		p1...place you want text
	///		Center...Center of the text rectangle
	///		Angle...Angle in degree
	///
	/// returns:
	///		point at wich to paint text.
	///--------------------------------------------
	CPoint rP;
	double r2;
	double dx = double(p1.x - Center.x);
	double dy = double(p1.y - Center.y);

	r2 = sqrt(dx*dx+dy*dy);
	rP.x = int(r2 * cos(angle - 90.0));
	rP.y = int(r2 * sin(angle - 90.0));
	return p1 - rP;
}

void CCadText::AdjustRefernce(CPoint p)
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


CRect CCadText::GetRect()
{
	return GetTextRectangle();
}

void CCadText::RenderEnable(int e)
{
	CCadText::m_RenderEnable = e;
}

CPoint CCadText::GetCenter()
{
	CRect rect = this->GetRect();
	return rect.CenterPoint();
}

// Moves the center of the object to the spcified point
void CCadText::ChangeCenter(CSize p)
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


CSize CCadText::GetSize()
{
	CRect rect = GetRect();
	return rect.Size();
}


void CCadText::ChangeSize(CSize Sz)
{
//	m_P2.x += Sz.cx;
//	m_P2.y += Sz.cy;
}
