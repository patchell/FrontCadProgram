// CadObject.h: interface for the CCadObject class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_CADOBJECT_H__9AC56957_97CD_408B_B000_B41BC4FF00F9__INCLUDED_)
#define AFX_CADOBJECT_H__9AC56957_97CD_408B_B000_B41BC4FF00F9__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "Scale.h"

class CCadObject
{
public:
	int m_Dirty;
	int m_LastMode;
	CPoint m_P2;
	CPoint m_P1;
	int m_Selected;
	CCadObject * m_pPrev;
	CCadObject * m_pNext;
	CCadObject * m_pSelNext;
	CCadObject();
	CCadObject(int type);
	virtual ~CCadObject();
	virtual void Move(CPoint p);
	virtual void Save(FILE *pO);
	virtual void SetVertex(int Vi,CPoint p);
	virtual int GrabVertex(CPoint p);
	virtual void Draw(CDC *pDC,int mode=0,CPoint Offset=CPoint(0,0),CScale Scale=CScale(0.1,0.1));
	virtual int CheckSelected(CPoint p,CSize Offset=CSize(0,0));
	virtual CPoint GetReference();
	virtual void AddObject(CCadObject *pO);
	virtual void RemoveObject(CCadObject *pO);
	virtual CCadObject *GetHead(void){return 0;}
	virtual void MakeDirty(void);
	virtual void SetSelected(int Flag=0);
	virtual void AdjustRefernce(CPoint Ref);
	virtual CRect GetRect(void);
	virtual char * GetTypeString(void);
	void SetType(int t){m_Type = t;}
	int GetType(void){return m_Type;}
	CCadObject operator=(CCadObject &v);
	CPoint ScalePoint(CPoint p,int Scale,int ScaleDiv);
	CCadObject * CopyObject(void);
	void SetRect(CRect & rect, CPoint P1, CPoint P2, CSize Lw);
	virtual void RenderEnable(int e);
private:
	int m_Type;
public:
	CCadObject * PopHead();
	CCadObject * PopTail();
	virtual CPoint GetCenter();
	// Moves the center of the object to the spcified point
	virtual void ChangeCenter(CSize p);
	virtual CSize GetSize();
	virtual void ChangeSize(CSize Sz);
};

#endif // !defined(AFX_CADOBJECT_H__9AC56957_97CD_408B_B000_B41BC4FF00F9__INCLUDED_)
