// FrontCad.h : main header file for the FRONTCAD application
//

#if !defined(AFX_FRONTCAD_H__E7F90698_BBB2_4535_A812_3FB737CDB14E__INCLUDED_)
#define AFX_FRONTCAD_H__E7F90698_BBB2_4535_A812_3FB737CDB14E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"       // main symbols
#include "FrontCadView.h"
#include "MainFrm.h"
#include "LibFormView.h"
#include "UtilView.h"
#include "CadArc.h"	// Added by ClassView
#include "CadRoundRect.h"	// Added by ClassView
#include "CadLine.h"	// Added by ClassView
#include "CadElipse.h"	// Added by ClassView
#include "CadRect.h"	// Added by ClassView
#include "CadPolygon.h"	// Added by ClassView
#include "CadPolygonFill.h"
#include "CadHoleRound.h"	// Added by ClassView
#include "CadHoleRnd2Flat.h"
#include "CadHoleRnd1Flat.h"
#include "CadRectHole.h"
#include "ScaleWizDialog.h"
#include "CadText.h"
#include "CadArrow.h"
#include "CadOrigin.h"
#include "CadDimension.h"


/////////////////////////////////////////////////////////////////////////////
// CFrontCadApp:
// See FrontCad.cpp for the implementation of this class
//

class CFrontCadApp : public CWinApp
{
	char *fname;
public:
	///------------------------------
	/// Program settings
	///------------------------------
	RndRectAttb m_RndRectAttributes;
	TextAttributes m_TextAttributes;
	SCALEWIZattributes m_SCALEWIZattributes;
	PolyAttributes m_PolyAttributes;
	RectHoleAttributes m_RectHoleAttributes;
	HoleRnd1FlatAttributes m_HoleRnd1FlatAttributes;
	HoleRnd2FlatAttributes m_HoleRnd2FlatAttributes;
	HoleRoundAttributes m_HoleRoundAttributes;
	ArcAttributes m_ArcAttributes;
	PolygonFillAttributes m_PolyFillAttributes;
	RectAttributes m_RectAttributes;
	ElipseAttributes m_EllipseAttributes;
	LineAttrib m_LineAttrib;
	ArrowAttributes m_ArrowAttrib;
	OriginAttrib m_OriginAttrib;
	DimAttrib m_DimAttrib;
public:
	CFrontCadView *m_pMainView;
	CLibFormView *m_pLibView;
	CUtilView *m_UtilView;
	CFrontCadDoc *m_pDoc;
	CFrontCadApp();
	void SaveSettings(void);
	void LoadSettings(void);
	void WriteToLogFile(char * pS);
	CMainFrame *GetMainFrame(void) { return (CMainFrame *)m_pMainWnd; }
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CFrontCadApp)
	public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();
	//}}AFX_VIRTUAL

// Implementation
	//{{AFX_MSG(CFrontCadApp)
	afx_msg void OnAppAbout();
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
	FILE *m_pLogFile;
};

extern CFrontCadApp theApp;
extern double ArcTan(double x, double y);
extern int CheckAngle(double Start, double End,double Angle);

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_FRONTCAD_H__E7F90698_BBB2_4535_A812_3FB737CDB14E__INCLUDED_)
