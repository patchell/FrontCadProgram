// Polygon.h: interface for the CPolygon class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_POLYGON_H__6B41D47F_05BB_4035_A037_9D6862C9C946__INCLUDED_)
#define AFX_POLYGON_H__6B41D47F_05BB_4035_A037_9D6862C9C946__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


#include "CadObject.h"

typedef struct
{
	int m_Width;
	COLORREF m_LineColor;
}PolyAttributes;

class CCadPolygonFill;
class CFileParser;
class CCadText;

class CCadPolygon:public CCadObject
{
	friend CCadText;
	friend CFileParser;
	friend CCadPolygonFill;
	int m_MaxY;
	int m_MinY;
	int m_MaxX;
	int m_MinX;
	int m_PolyID;
	CPoint * m_pVertex;
	CPen *m_pPenLine;
	int m_size;
	int m_Count;
	int m_Width;
	COLORREF m_LineColor;
public:
	static int m_RenderEnable;
	CCadPolygon();
	CCadPolygon(CCadPolygon &v);
	CCadPolygon(int size);
	virtual ~CCadPolygon();
	virtual void Draw(CDC *pDC,int mode = 0,CPoint Offset=CPoint(0,0),CScale Scale=CScale(0.1,0.1));
	virtual int CheckSelected(CPoint p, CSize Offset = CSize(0, 0));
	virtual void Save(FILE *pO);
	virtual CPoint GetReference();
	virtual void Move(CPoint p);
	virtual void SetVertex(int Vi,CPoint p);
	virtual int GrabVertex(CPoint p);
	virtual void AdjustRefernce(CPoint Ref);
	virtual CRect GetRect(void);
	void UpdateMinMax(void);
	void SetCurPoint(CPoint p);
	void Reset(void);
	inline void SetCount(int cnt){m_Count = cnt;}
	inline void SetPolySize(int sz){m_size = sz;}
	inline int GetPolySize(void){return m_size;}
	void Copy(CCadPolygon *pP);
	CPoint * GetPoints(void);
	int CompareToLast(CPoint nP);
	int DeleteLastPoint(void);
	int GetCount(void);
	void AddPoint(CPoint nP);
	BOOL PointEnclosed(CPoint,CSize Offset=CSize(0,0));
	void Create(CPoint *);
	inline void SetWidth(int w){m_Width = w;}
	inline int GetWidth(void){return m_Width;}
	inline void SetLineColor(COLORREF c){m_LineColor = c;}
	inline COLORREF GetLineColor(void){return m_LineColor;}
	CCadPolygon operator=(CCadPolygon &v);
	virtual void RenderEnable(int e);
	virtual CPoint GetCenter();
	// Moves the center of the object to the spcified point
	virtual void ChangeCenter(CSize p);
	virtual CSize GetSize();
	virtual void ChangeSize(CSize Sz);
};

#endif // !defined(AFX_POLYGON_H__6B41D47F_05BB_4035_A037_9D6862C9C946__INCLUDED_)
