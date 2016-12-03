#if !defined(AFX_LIBFORMVIEW_H__75F48209_C93E_41ED_B043_058B6517975C__INCLUDED_)
#define AFX_LIBFORMVIEW_H__75F48209_C93E_41ED_B043_058B6517975C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// LibFormView.h : header file
//
#include "StaticPreview.h"
#include "CadLibrary.h"
#include "ComboBoxLibPartSelector.h"
#include "ComboBoxLibrary.h"

/////////////////////////////////////////////////////////////////////////////
// CLibFormView form view

#ifndef __AFXEXT_H__
#include <afxext.h>
#endif

class CLibFormView : public CFormView
{
	CCadLibrary *m_pHead;
	CCadLibrary *m_pTail;
protected:
	CLibFormView();           // protected constructor used by dynamic creation
	DECLARE_DYNCREATE(CLibFormView)

// Form Data
public:
	//{{AFX_DATA(CLibFormView)
	enum { IDD = IDD_DIALOG_LIBVIEW };
	CComboBoxLibPartSelector	m_Combo_SelPart;
	CComboBoxLibrary	m_Combo_SelLIb;
	CStaticPreview	m_Static_Preview;
	//}}AFX_DATA

// Attributes
public:

// Operations
public:
	CCadLibObject * GetCurrentPart(void);
	CCadLibrary * GetCurrentLibrary(void);
	void AddPartToCurLib(CCadLibObject *pObj);
	void DeleteLibrary(CCadLibrary *pL);
	void AddLibrary(CCadLibrary *pL);
	CCadLibrary *GetLibraryList(void){return m_pHead;}
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CLibFormView)
	public:
	virtual void OnInitialUpdate();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	LRESULT OnMainViewGotReference(WPARAM wP,LPARAM lP);
	virtual ~CLibFormView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

	// Generated message map functions
	//{{AFX_MSG(CLibFormView)
	afx_msg void OnButtonLibviewCreatecomp();
	afx_msg void OnButtonLibviewPlace();
	afx_msg void OnButtonLibviewNewlib();
	afx_msg void OnButtonLibviewRemove();
	afx_msg void OnButtonLibviewDeletepart();
	afx_msg void OnSelchangeComboLibviewComponent();
	afx_msg void OnSelchangeComboLibviewLibrary();
	afx_msg void OnDblclkComboLibviewComponent();
	afx_msg void OnButtonLibviewSavelibrary();
	//}}AFX_MSG
public:
	afx_msg void OnButtonAddlibrary();
protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnViewRenderenable();
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_LIBFORMVIEW_H__75F48209_C93E_41ED_B043_058B6517975C__INCLUDED_)
