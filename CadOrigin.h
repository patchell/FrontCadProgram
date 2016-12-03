#pragma once
#include "CadObject.h"

#ifndef ORIGINATRIB
#define ORIGINATRIB
typedef struct {
	int m_Width;
	COLORREF m_Color;
} OriginAttrib;

#endif

class CFrontCadView;

class CCadOrigin :public CCadObject
{
	OriginAttrib m_Atrib;
	CFrontCadView *m_pMainView;
	CPen *m_pPenLine;
public:
	static int m_RenderEnable;
	CCadOrigin();
	CCadOrigin(CCadOrigin &v);
	virtual ~CCadOrigin();
	virtual void Move(CPoint p);
	virtual void Save(FILE *pO);
	virtual void Draw(CDC *pDC, int mode = 0, CPoint Offset = CPoint(0, 0), CScale Scale = CScale(0.1, 0.1));
	virtual int CheckSelected(CPoint p, CSize Offset = CSize(0, 0));
	virtual CPoint GetReference();
	virtual CCadObject *GetHead(void) { return 0; }
	virtual void MakeDirty(void);
	virtual void SetSelected(int Flag = 0);
	virtual void AdjustRefernce(CPoint Ref);
	virtual CRect GetRect(void);
	inline void SetParent(CFrontCadView *pW) { m_pMainView = pW; }
	inline void SetColor(COLORREF c) { m_Atrib.m_Color = c; }
	inline COLORREF GetColor(void) { return m_Atrib.m_Color; }
	inline void SetWidth(int w) { m_Atrib.m_Width = w; }
	inline int GetWidth(void) { return m_Atrib.m_Width; }
	virtual void RenderEnable(int e);
	virtual CPoint GetCenter();
	// Moves the center of the object to the spcified point
	virtual void ChangeCenter(CSize p);
	virtual CSize GetSize();
	virtual void ChangeSize(CSize Sz);
};

