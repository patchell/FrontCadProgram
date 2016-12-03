// FrontCadDoc.h : interface of the CFrontCadDoc class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_FRONTCADDOC_H__A897E098_6F57_478C_952F_DF84F475B43E__INCLUDED_)
#define AFX_FRONTCADDOC_H__A897E098_6F57_478C_952F_DF84F475B43E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "CadObject.h"
#include "CadDrawing.h"

class CFrontCadView;

class CFrontCadDoc : public CDocument
{
protected: // create from serialization only
	CFrontCadDoc();
	DECLARE_DYNCREATE(CFrontCadDoc)

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CFrontCadDoc)
	public:
	virtual BOOL OnNewDocument();
	virtual void Serialize(CArchive& ar);
	virtual BOOL OnSaveDocument(LPCTSTR lpszPathName);
	virtual BOOL OnOpenDocument(LPCTSTR lpszPathName);
	virtual BOOL CanCloseFrame(CFrameWnd* pFrame);
	//}}AFX_VIRTUAL

// Implementation
public:
	CCadDrawing * m_pDrawing;
	CFrontCadView * m_pDocView;
	virtual ~CFrontCadDoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	//{{AFX_MSG(CFrontCadDoc)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
public:
	void InsertObject(CCadObject * pObj);
	void AddObject(CCadObject * pObj);
	void RemoveObject(CCadObject * pObj);
	CCadPrintRect * GetPrintRect();
	COLORREF GetBkColor();
	void SetBkColor(COLORREF color);
	void MakeDirty();
	int CheckSelected(CPoint p, CCadObject **ppSelList, int n, int flag = 0) { return m_pDrawing->CheckSelected(p, ppSelList, n, flag); }
	CCadObject *GetHead(void) { return m_pDrawing->GetHead(); }
	void Print(CDC *pDC, int mode, CPoint Offset, CScale Scale) { m_pDrawing->Print(pDC, mode, Offset, Scale); }
	void CheckPrev(void) { m_pDrawing->CheckPrev(); }
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_FRONTCADDOC_H__A897E098_6F57_478C_952F_DF84F475B43E__INCLUDED_)
