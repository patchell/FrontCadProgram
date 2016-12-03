// CadDrawing.h: interface for the CCadDrawing class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_CADDRAWING_H__158D9EB0_BE4F_471A_B22E_2423339E764A__INCLUDED_)
#define AFX_CADDRAWING_H__158D9EB0_BE4F_471A_B22E_2423339E764A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "CadObject.h"

#define CADDRAWUBG_CHECKSEL_FLAG_ALL	0
#define CADDRAWUBG_CHECKSEL_FLAG_UNSEL	1
#define CADDRAWUBG_CHECKSEL_FLAG_SEL	2


class CFileParser;
class CCadPrintRect;

class CCadDrawing : public CCadObject  
{
	friend CFileParser;
	CCadObject *m_pHead;
	CCadObject *m_pTail;
	COLORREF m_BkColor;
public:
	static int m_RenderEnable;
	int m_Error;
	void CheckPrev(void);
	CCadLibObject * CreatePartFromSelected(char *name);
	virtual void MakeDirty();
	int CheckSelected(CPoint p,CCadObject **ppSelList,int n,int flag= CADDRAWUBG_CHECKSEL_FLAG_ALL);
	void Print(CDC *pDC,int mode,CPoint Offset,CScale Scale);
	virtual void Save(FILE *pO);
	virtual void Draw(CDC *pDC,int mode,CPoint Offset,CScale Scale);
	CCadDrawing();
	virtual ~CCadDrawing();
	virtual void AddObject(CCadObject *pO);
	virtual void RemoveObject(CCadObject *pO);
	virtual void InsertObject(CCadObject *pObj);
	virtual CCadObject *GetHead(void){return m_pHead;}
	void SetBkColor(COLORREF c){m_BkColor = c;}
	COLORREF GetBkColor(void){return m_BkColor;}
	CCadPrintRect *GetPrintRect();
};

#endif // !defined(AFX_CADDRAWING_H__158D9EB0_BE4F_471A_B22E_2423339E764A__INCLUDED_)
