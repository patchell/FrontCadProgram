#pragma once
#include "CadObject.h"
#include "CadText.h"

typedef struct {
	int m_Width;
	COLORREF m_Color;
	TextAttributes m_Text;
}DimAttrib;

class CCadDimension :public CCadObject
{
	DimAttrib m_Atrib;
	CPen *m_pLine;
	CCadText *m_pText;
public:
	static int m_RenderEnable;
	CCadDimension();
	CCadDimension(CCadDimension &cd);
	virtual ~CCadDimension();
	CCadDimension operator=(CCadDimension &cd);
	virtual void Move(CPoint p);
	virtual void Save(FILE *pO);
	virtual void SetVertex(int Vi, CPoint p);
	virtual int GrabVertex(CPoint p);
	virtual void Draw(CDC *pDC, int mode = 0, CPoint Offset = CPoint(0, 0), CScale Scale = CScale(0.1, 0.1));
	virtual int CheckSelected(CPoint p, CSize Offset = CSize(0, 0));
	virtual CPoint GetReference();
	virtual void AddObject(CCadObject *pO);
	virtual void RemoveObject(CCadObject *pO);
	virtual CCadObject *GetHead(void) { return (CCadObject *)m_pText; }
	virtual void MakeDirty(void);
	virtual void SetSelected(int Flag = 0);
	virtual void AdjustRefernce(CPoint Ref);
	virtual CRect GetRect(void);
	void UpdateText(CPoint Org);
	void SetValue(int v, int dp);
	inline void SetColor(COLORREF c) { m_Atrib.m_Color = c; }
	inline COLORREF GetColor(void) { return m_Atrib.m_Color; }
	inline void SetWidth(int w) { m_Atrib.m_Width = w; }
	inline int GetWidth(void) { return m_Atrib.m_Width; }
	inline CCadText *GetText(void) { return m_pText; }
	DimAttrib &GetAttrib(void) { return m_Atrib; }
	virtual void RenderEnable(int e);
	virtual CPoint GetCenter();
	// Moves the center of the object to the spcified point
	virtual void ChangeCenter(CSize p);
	void AlignToPoint(CPoint p);
	virtual CSize GetSize();
	virtual void ChangeSize(CSize Sz);
	void SetFontName(char * pN);
	char * GetFontName();
};

