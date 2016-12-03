// FrontCadView.cpp : implementation of the CFrontCadView class
//

#include "stdafx.h"
#include <string.h>
#include "FrontCadDefines.h"
#include "FrontCad.h"

#include "FrontCadDoc.h"
#include "FrontCadView.h"

#include "MainFrm.h"
#include "CadObject.h"
#include "CadRect.h"
#include "CadLine.h"
#include "CadElipse.h"
#include "UtilView.h"
#include "CadRoundRect.h"
#include "CadArcCentered.h"
#include "CadHoleRound.h"
#include "CadHoleRnd2Flat.h"
#include "CadHoleRnd1Flat.h"
#include "LibFormView.h"
#include "ScaleWizDialog.h"
#include "DialogGridSettings.h"
#include "MoveObjects.h"
#include "CadBitmap.h"
#include "CadOrigin.h"
#include "CadDimension.h"
#include "DialogRenderEnable.h"
#include "RepeatDlg.h"
#include "CadPrintRect.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define ID_OBJECTSEL		3120

#define ZOOM_NORMAL         1

#define INCHES_DECPLACE		3
#define DEGREE_DECPLACE		1

double ZF[MAX_ZOOM] = {
	1.0,		//1000 pixels per inch
	0.50,		// 500 pixels per inch
	0.20,		// 200 pixels per inch
	0.10,		// 100 pixels per inch
	0.05,		//  50 pixels per inch
	0.02		//	20 pixels per inch
};

int ZoomScale[MAX_ZOOM] = {
	1,
	1,
	1,
	1,
	1,
	1
};

int ZoomScaleDiv[MAX_ZOOM] = {
	1,
	2,
	5,
	10,
	20,
	50,
};



/////////////////////////////////////////////////////////////////////////////
// CFrontCadView

IMPLEMENT_DYNCREATE(CFrontCadView, CView)

BEGIN_MESSAGE_MAP(CFrontCadView, CView)
	//{{AFX_MSG_MAP(CFrontCadView)
	ON_WM_ERASEBKGND()
	ON_WM_HSCROLL()
	ON_WM_KEYDOWN()
	ON_WM_KEYUP()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_MOUSEMOVE()
	ON_WM_VSCROLL()
	ON_COMMAND(ID_TOOLBAR_ZOOMOUT, OnToolbarZoomout)
	ON_UPDATE_COMMAND_UI(ID_TOOLBAR_ZOOMOUT, OnUpdateToolbarZoomout)
	ON_COMMAND(ID_TOOLBAR_ZOOMIN, OnToolbarZoomin)
	ON_UPDATE_COMMAND_UI(ID_TOOLBAR_ZOOMIN, OnUpdateToolbarZoomin)
	ON_COMMAND(ID_TOOLBAR_TOGGLERULER, OnToolbarToggleruler)
	ON_COMMAND(ID_TOOLBAR_SELECT, OnToolbarSelect)
	ON_COMMAND(ID_TOOLBAR_RECT, OnToolbarRect)
	ON_COMMAND(ID_TOOLBAR_LINE, OnToolbarLine)
	ON_COMMAND(ID_TOOLBAR_ELLIPSE, OnToolbarEllipse)
	ON_WM_CONTEXTMENU()
	ON_COMMAND(ID_TOOLBAR_RNDRECT, OnToolbarRndrect)
	ON_COMMAND(ID_TOOLBAR_GRIDONOFF, OnToolbarGridonoff)
	ON_COMMAND(ID_TOOLBAR_GRIDSETTINGS, OnToolbarGridsettings)
	ON_COMMAND(ID_TOOLBAR_LIBPART, OnToolbarLibpart)
	ON_COMMAND(ID_TOOLBAR_POLYGON, OnToolbarPolygon)
	ON_COMMAND(ID_TOOLBAR_SCALEWIZ, OnToolbarScalewiz)
	ON_COMMAND(ID_TOOLBAR_ARC, OnToolbarArc)
	ON_COMMAND(ID_TOOLBAR_POLYWITHFILL, OnToolbarPolywithfill)
	ON_COMMAND(ID_TOOLBAR_ARC_CENTER, OnToolbarArcCenter)
	ON_COMMAND(ID_TOOLBAR_HOLERND, OnToolbarHolernd)
	ON_COMMAND(ID_TOOLBAR_RECTHOLE, OnToolbarRecthole)
	ON_COMMAND(ID_TOOLBAR_RND_1FLAT, OnToolbarRnd1flat)
	ON_COMMAND(ID_TOOLBAR_TWOFLATS, OnToolbarTwoflats)
	ON_COMMAND(ID_TOOLBAR_DRAWTEXT, OnToolbarDrawtext)
	ON_WM_KILLFOCUS()
	ON_WM_SIZE()
	ON_COMMAND(ID_EDIT_COPY, OnEditCopy)
	ON_UPDATE_COMMAND_UI(ID_EDIT_COPY, OnUpdateEditCopy)
	ON_COMMAND(ID_EDIT_CUT, OnEditCut)
	ON_UPDATE_COMMAND_UI(ID_EDIT_CUT, OnUpdateEditCut)
	ON_COMMAND(ID_EDIT_PASTE, OnEditPaste)
	ON_UPDATE_COMMAND_UI(ID_EDIT_PASTE, OnUpdateEditPaste)
	ON_COMMAND(ID_TOOLBAR_SELECTREGION, OnToolbarSelectregion)
	ON_COMMAND(ID_TOOLBAR_BITMAP, OnToolbarBitmap)
	//}}AFX_MSG_MAP
	// Standard printing commands
	ON_COMMAND(ID_FILE_PRINT, CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, CView::OnFilePrintPreview)
	ON_MESSAGE(WM_UTILMESSAGE,OnUtilMessage)
	ON_MESSAGE(WM_LIBVIEW_GETREF,OnLibViewRequestReferencePoint)
	ON_COMMAND(ID_FILE_PRINTTOCLIPBOARD, &CFrontCadView::OnFilePrinttoclipboard)
	ON_COMMAND(ID_BUTTON_ARROWOBJ, &CFrontCadView::OnButtonArrowobj)
	ON_COMMAND(ID_BUTTON_DIMENSION, &CFrontCadView::OnButtonDimension)
	ON_COMMAND(ID_BUTTON_ORIGIN, &CFrontCadView::OnButtonOrigin)
	ON_MESSAGE(WM_UPDATEDIMENSIONS, &CFrontCadView::OnUpdatedimensions)
	ON_COMMAND(ID_VIEW_RENDERENABLE, &CFrontCadView::OnViewRenderenable)
	ON_COMMAND(ID_ALIGN_CENTERSVERTICAL, &CFrontCadView::OnAlignCentersvertical)
	ON_COMMAND(ID_ALIGN_CENTERSHORIZONTAL, &CFrontCadView::OnAlignCentershorizontal)
	ON_COMMAND(ID_ALIGN_CENTERS, &CFrontCadView::OnAlignCenters)
	ON_UPDATE_COMMAND_UI(ID_ALIGN_CENTERS, &CFrontCadView::OnUpdateAlignCentersUI)
	ON_UPDATE_COMMAND_UI(ID_ALIGN_CENTERSHORIZONTAL, &CFrontCadView::OnUpdateAlignCentershorizontal)
	ON_UPDATE_COMMAND_UI(ID_ALIGN_CENTERSVERTICAL, &CFrontCadView::OnUpdateAlignCentersvertical)
	ON_COMMAND(ID_ALIGN_BOTTOM, &CFrontCadView::OnAlignBottom)
	ON_COMMAND(ID_ALIGN_LEFT, &CFrontCadView::OnAlignLeft)
	ON_COMMAND(ID_ALIGN_RIGHT, &CFrontCadView::OnAlignRight)
	ON_COMMAND(ID_ALIGN_SIZEBOTH, &CFrontCadView::OnAlignSizeboth)
	ON_COMMAND(ID_ALIGN_SIZEX, &CFrontCadView::OnAlignSizex)
	ON_COMMAND(ID_ALIGN_SIZEY, &CFrontCadView::OnAlignSizey)
	ON_COMMAND(ID_ALIGN_TOP, &CFrontCadView::OnAlignTop)
	ON_UPDATE_COMMAND_UI(ID_ALIGN_BOTTOM, &CFrontCadView::OnUpdateAlignBottom)
	ON_UPDATE_COMMAND_UI(ID_ALIGN_LEFT, &CFrontCadView::OnUpdateAlignLeft)
	ON_UPDATE_COMMAND_UI(ID_ALIGN_RIGHT, &CFrontCadView::OnUpdateAlignRight)
	ON_UPDATE_COMMAND_UI(ID_ALIGN_SIZEBOTH, &CFrontCadView::OnUpdateAlignSizeboth)
	ON_UPDATE_COMMAND_UI(ID_ALIGN_SIZEX, &CFrontCadView::OnUpdateAlignSizex)
	ON_UPDATE_COMMAND_UI(ID_ALIGN_SIZEY, &CFrontCadView::OnUpdateAlignSizey)
	ON_UPDATE_COMMAND_UI(ID_ALIGN_TOP, &CFrontCadView::OnUpdateAlignTop)
	ON_COMMAND(ID_BUTTON_PRINT_RECTANGLE, &CFrontCadView::OnButtonPrintRectangle)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CFrontCadView construction/destruction

CFrontCadView::CFrontCadView()
{
	// TODO: add construction code here
	m_pClipBoard = 0;
	m_pParent = NULL;
	m_bMoveImage = FALSE;
	m_fZoomFactor = 1.0;
	m_bShowRulers = TRUE;
	m_pMemDC = NULL;
	m_pDCbm = NULL;
	m_SelRegion_P1 = CPoint(0, 0);
	m_SelRegion_P2 = CPoint(0, 0);
	m_pMoveObj = 0;
	m_ZoomLevel = 3;
	m_GrabbedVertex = -1;
	m_DragObject = 0;
	m_Drawmode = 0;
	m_pDrawObject = 0;
	m_SnapOff = 0;		//snap grid is on
	m_Grid = 1000;		//1 inch grid
	m_GridColor = RGB(128, 255, 255);
	m_GridOn = 1;		//grid is on
	m_SnapGrid = 250;	//1/4 inch grid
	m_DragState = 0;
	m_DocSize = CSize(24000, 24000);
	m_pTempCadBitmap = 0;
	///Selected Object List
	m_pSelObjList = 0;
	m_pSelObjEnd = 0;
	m_nSelRegionLock = 0;
	m_SelectionCount = 0;
}

CFrontCadView::~CFrontCadView()
{
}

BOOL CFrontCadView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return CView::PreCreateWindow(cs);
}

/////////////////////////////////////////////////////////////////////////////
// CFrontCadView drawing

void CFrontCadView::OnPrint(CDC* pDC, CPrintInfo* pInfo)
{
///------------------------------------------
///	OnPrint
///		This is the draw function that is
/// called when the device context is
/// the printer.
///
///	parameters:
///		pDC....pointer to the device context
///		pInfo..poointer to printer information
///------------------------------------------
	// TODO: Add your specialized code here and/or call the base class
	int HPix = pDC->GetDeviceCaps(HORZRES);
	int VPix = pDC->GetDeviceCaps(VERTRES);
	int DpiX = pDC->GetDeviceCaps(LOGPIXELSX);
	int DpiY = pDC->GetDeviceCaps(LOGPIXELSY);
	int OffX = pDC->GetDeviceCaps(PHYSICALOFFSETX);
	int OffY = pDC->GetDeviceCaps(PHYSICALOFFSETY);
	pDC->SetMapMode(MM_HIENGLISH);
	CFrontCadDoc* pDoc = GetDocument();
	CPoint offset;
	CBrush br;
	CRect rect;
	CCadPrintRect *pagerect;
	if ((pagerect = pDoc->GetPrintRect()) == NULL)
	{
		offset.SetPoint(0, 0);
	}
	else
	{
		offset = pagerect->GetRect().TopLeft();
		pagerect->RenderEnable(0);
	}
	rect.SetRect(0,0,HPix-1,VPix-1);
	br.CreateSolidBrush(pDoc->GetBkColor());
	pDC->FillRect(&rect,&br);
	//----------------------------------
	// Make all of the objects dirty
	///---------------------------------
	pDoc->MakeDirty();
	//----------------------------------
	// Print the document
	//----------------------------------
	offset.x = -offset.x;
	pDoc->Print(pDC,OBJECT_MODE_FINAL,offset,CScale(1.0,-1.0));
	//make dirty all over again.
	if (pagerect)pagerect->RenderEnable(1);

	pDoc->MakeDirty();


	CView::OnPrint(pDC, pInfo);
}

void CFrontCadView::OnDraw(CDC* pDC)
{
	///----------------------------------------
	/// OnDraw
	///		This is the function that is called
	/// when the screen needs to be redrawn.
	/// To minimize screen flicker, all drawing
	/// is done to a Memory DC, and then it is
	/// blt to the screen.
	///
	///	parameters:
	///		pDC....pointer to the device context
	///----------------------------------------
	CDC *pDCm = new CDC;
	CBitmap bm;
	CFrontCadDoc* pDoc = GetDocument();
	CCadObject *pCO = pDoc->m_pDrawing;
	//	ASSERT_VALID(pDoc);
	CRect rect;
	//Set Background Color
	GetClientRect(&rect);
	///create a compatable memory device context
	bm.CreateBitmap(rect.Width(), rect.Height(), 1, 32, NULL);
	pDCm->CreateCompatibleDC(pDC);
	pDCm->SelectObject(&bm);
	CBrush br;
	br.CreateSolidBrush(((CCadDrawing *)pCO)->GetBkColor());
	pDCm->FillRect(&rect, &br);
	pDCm->SetBkColor(((CCadDrawing *)pCO)->GetBkColor());
	DrawGrid(pDCm, 0, rect);
	CPoint Off = CPoint(m_HScrollPos, m_VScrollPos);

	pCO->Draw(pDCm, OBJECT_MODE_FINAL, -Off, CScale(ZF[m_ZoomLevel], ZF[m_ZoomLevel]));
	int mode;
	// TODO: add draw code for native data here
	if (m_DrawState == DRAWSTATE_MOVE  && m_pDrawObject)
	{
		mode = OBJECT_MODE_SKETCH;
		m_pDrawObject->Draw(pDCm, mode, -Off, CScale(ZF[m_ZoomLevel], ZF[m_ZoomLevel]));
	}
	else if ( m_DrawState == DRAWSTATE_ARCSTART)
	{
		mode = OBJECT_MODE_ARCSTART;
		m_pDrawObject->Draw(pDCm, mode, -Off, CScale(ZF[m_ZoomLevel], ZF[m_ZoomLevel]));
	}
	else if (m_DrawState == DRAWSTATE_ARCEND)
	{
		mode = OBJECT_MODE_ARCEND;
		m_pDrawObject->Draw(pDCm, mode, -Off, CScale(ZF[m_ZoomLevel], ZF[m_ZoomLevel]));
	}
	if (m_pMoveObj)
	{
		m_pMoveObj->Draw(pDCm, SnapToScreen(m_SnapPos) - Off, CScale(ZF[m_ZoomLevel], ZF[m_ZoomLevel]));
	}
	if ((m_Drawmode == DRAWMODE_SELECTREGION) && ((m_MouseState == MOUSESTATE_DOWN)|| (m_nSelRegionLock) ) )
	{
		int bkm = pDCm->SetBkMode(TRANSPARENT);
		CRect RRect;
		CSize Offset;
		CPoint P1, P2;
		P1 = CScale(ZF[m_ZoomLevel],ZF[m_ZoomLevel]) * m_SelRegion_P1;
		P2 = CScale(ZF[m_ZoomLevel], ZF[m_ZoomLevel]) * m_SelRegion_P2;
		Offset.cx = -m_HScrollPos;
		Offset.cy = -m_VScrollPos;
		RRect.SetRect(P1 +Offset,P2 +Offset);
		CPen DotPen;
		DotPen.CreatePen(PS_DOT,1,RGB(0,0,0));
		CPen *oP;
		oP = pDCm->SelectObject(&DotPen);
		pDCm->DrawDragRect(&RRect,CSize(1,1),NULL,CSize(1,1));
		pDCm->SelectObject(oP);
		pDCm->SetBkMode(bkm);
	}
	this->DrawCrosshairs(pDCm,&rect, SnapToScreen(m_SnapPos));
	pDC->BitBlt(0,0,rect.Width(),rect.Height(),pDCm,0,0,SRCCOPY);
	delete pDCm;
}

void CFrontCadView::OnInitialUpdate()
{
	CView::OnInitialUpdate();

	// TODO: calculate the total size of this view


    m_pParent = ((CMainFrame*)GetParentFrame());
	CFrontCadApp *pA = (CFrontCadApp *)AfxGetApp();
    m_pParent->ShowRulers(TRUE);
	CRect clientrect;
	GetClientRect(&clientrect);
	m_DocSize = CSize(24000,24000);	//document size is in mils
	m_ImageSize = clientrect.Size();
	((CFrontCadApp *)AfxGetApp())->m_pMainView = this;
	ShowScrollBar(SB_BOTH,TRUE);
	UpdateScrollbarInfo();
	EnableScrollBarCtrl(SB_VERT,TRUE);
	EnableScrollBarCtrl(SB_HORZ,TRUE);
	//----------------------------------
	// Create a memory device context
	// so we can get rid of the flicker
	//----------------------------------
	CDC *pDC = GetDC();
	if(m_pMemDC) delete m_pMemDC;
	m_pMemDC = new CDC;
	m_pMemDC->CreateCompatibleDC(pDC);
	if(m_pDCbm) delete m_pDCbm;
	m_pDCbm = new CBitmap;
	m_pDCbm->CreateBitmap(clientrect.Width(),clientrect.Height(),1,32,NULL);
	m_pMemDC->SelectObject(m_pDCbm);
	CBrush br;
	br.CreateSolidBrush(RGB(255,255,255));
	m_pMemDC->FillRect(&clientrect,&br);

	CUtilView *pUV = GetUtilityView();
	pUV->ShowHide(0);
	pUV->m_pCadView = this;

	CFrontCadDoc *pDoc = GetDocument();
	pUV->m_Static_Background.SetColor(pDoc->GetBkColor());
	pUV->m_Static_Background.SetupMessaging(this,WM_UTILMESSAGE,UTIL_BACKGROUND);

	pUV->m_Edit_X3.SetupMessaging(this,WM_UTILMESSAGE,UTIL_X3);
	pUV->m_Edit_X3.SetDecimalPlaces(INCHES_DECPLACE);
	pUV->m_Edit_Y3.SetupMessaging(this,WM_UTILMESSAGE,UTIL_Y3);
	pUV->m_Edit_Y3.SetDecimalPlaces(INCHES_DECPLACE);
	pUV->m_Edit_X2.SetupMessaging(this,WM_UTILMESSAGE,UTIL_X2);
	pUV->m_Edit_X2.SetDecimalPlaces(INCHES_DECPLACE);
	pUV->m_Edit_Y2.SetupMessaging(this,WM_UTILMESSAGE,UTIL_Y2);
	pUV->m_Edit_Y2.SetDecimalPlaces(INCHES_DECPLACE);
	pUV->m_Edit_X1.SetupMessaging(this,WM_UTILMESSAGE,UTIL_X1);
	pUV->m_Edit_X1.SetDecimalPlaces(INCHES_DECPLACE);
	pUV->m_Edit_Y1.SetupMessaging(this,WM_UTILMESSAGE,UTIL_Y1);
	pUV->m_Edit_Y1.SetDecimalPlaces(INCHES_DECPLACE);
	pUV->m_Edit_LineThickness.SetupMessaging(this,WM_UTILMESSAGE,UTIL_LINEWIDTH);
	pUV->m_Edit_LineThickness.SetDecimalPlaces(INCHES_DECPLACE);
	pUV->m_Static_LineColor.SetupMessaging(this,WM_UTILMESSAGE,UTIL_LINECOLOR);
	pUV->m_Static_FillColor.SetupMessaging(this,WM_UTILMESSAGE,UTIL_FILLCOLOR);
	pUV->m_Static_TextColor.SetupMessaging(this,WM_UTILMESSAGE,UTIL_TEXTCOLOR);
	pUV->m_Static_BkGrndColor.SetupMessaging(this,WM_UTILMESSAGE,UTIL_TEXTBKCOLOR);
	pUV->m_Edit_FlatToCenterDist.SetupMessaging(this,WM_UTILMESSAGE,UTIL_FLATDIST);
	pUV->m_Edit_FlatToCenterDist.SetDecimalPlaces(INCHES_DECPLACE);
	pUV->m_Edit_HoleRadius.SetupMessaging(this,WM_UTILMESSAGE,UTIL_HOLERADIUS);
	pUV->m_Edit_HoleRadius.SetDecimalPlaces(INCHES_DECPLACE);
	pUV->m_Edit_Text.SetupMessaging(this,WM_UTILMESSAGE,UTIL_TEXT);
	pUV->m_Edit_FontHeight.SetupMessaging(this,WM_UTILMESSAGE,UTIL_FONTHEIGHT);
	pUV->m_Edit_FontHeight.SetDecimalPlaces(INCHES_DECPLACE);
	pUV->m_Edit_FontWidth.SetupMessaging(this,WM_UTILMESSAGE,UTIL_FONTWIDTH);
	pUV->m_Edit_FontWidth.SetDecimalPlaces(INCHES_DECPLACE);
	pUV->m_Edit_TextAngle.SetupMessaging(this,WM_UTILMESSAGE,UTIL_TEXTANGLE);
	pUV->m_Edit_TextAngle.SetDecimalPlaces(DEGREE_DECPLACE);
	pUV->m_Check_TransparentFont.SetupMessaging(this,WM_UTILMESSAGE,UTIL_TEXTTRANSPARENT);

	pUV->m_Combo_PrintRect.SetupMessaging(this, WM_UTILMESSAGE, UTIL_PRINTRECT);
	pUV->m_Combo_FontWeight.SetupMessaging(this,WM_UTILMESSAGE,UTIL_FONTWEIGHT);
	pUV->m_Button_Font.SetWindowText(pA->m_TextAttributes.m_pFontName);

	pDoc->m_pDocView = this;

    OnUpdate(NULL, 0, NULL);
}

/////////////////////////////////////////////////////////////////////////////
// CFrontCadView printing

BOOL CFrontCadView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// default preparation
	return DoPreparePrinting(pInfo);
}

void CFrontCadView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add extra initialization before printing
}

void CFrontCadView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add cleanup after printing
}

/////////////////////////////////////////////////////////////////////////////
// CFrontCadView diagnostics

#ifdef _DEBUG
void CFrontCadView::AssertValid() const
{
	CView::AssertValid();
}

void CFrontCadView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CFrontCadDoc* CFrontCadView::GetDocument() // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CFrontCadDoc)));
	return (CFrontCadDoc*)m_pDocument;
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CFrontCadView message handlers

BOOL CFrontCadView::OnEraseBkgnd(CDC* pDC)
{
	///--------------------------------------
	///	OnEraseBkgnd
	///		We override this function so that
	///	the window is not erased.
	///--------------------------------------
	// TODO: Add your message handler code here and/or call default
//	return CView::OnEraseBkgnd(pDC);
	return TRUE;
}

void CFrontCadView::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	///----------------------------------------
	/// OnKeyDown
	///		This function is used to intercept
	/// key presses, looking for  HHotkeys
	/// that do certain funcrtions
	///----------------------------------------
	// TODO: Add your message handler code here and/or call default
	switch(nChar)	///check the key press
	{
		case VK_SHIFT:	///shift key
			m_SnapOff = 1;	///turn of snapo grid
			break;
		case VK_DELETE:
			break;
		case VK_LEFT:
			break;
		case VK_RIGHT:
			break;
		case VK_UP:
			break;
		case VK_DOWN:
			break;
		case VK_CONTROL:	///control key
			if(GetTopSelection())
			{
				CCadObject *pO = GetTopSelection();
				if(pO->GetType() == OBJECT_TYPE_BITMAP)
				{
					///turn off maintain aspect ratio
					/// when resizing bitmaps
					((CCadBitmap *)pO)->m_MaintainAspectRatio=0;
				}
			}
			break;
	}
	CView::OnKeyDown(nChar, nRepCnt, nFlags);
}

void CFrontCadView::OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	///--------------------------------------------
	/// OnKeyUp
	///		This function intercepts key releases.
	///--------------------------------------------
	// TODO: Add your message handler code here and/or call default
	int id;

	switch(nChar)	///check released character
	{
		case VK_SHIFT:	///shift key
			m_SnapOff = 0;	///turn snap back on
			break;
		case VK_DELETE:	///delete selected objects
			{
				id = MessageBox("Delete Selected Objects?","DELETE OBJECTS",MB_YESNO);
				if(id == IDYES)
				{
					CCadObject *pObj = GetTopSelection();
					CFrontCadDoc *pDoc = GetDocument();
					while(pObj)
					{
						RemoveObject(pObj);
						pDoc->RemoveObject(pObj);
						delete pObj;
						pObj = GetTopSelection();
					}
				}
			}
			break;
		case VK_LEFT:	///left arrow, move cursor
			NextSnap(VK_LEFT,m_SnapGrid);
			SetCursorPos(m_MousePos);
			Invalidate();
			break;
		case VK_RIGHT:	///right arrow, move cursor
			NextSnap(VK_RIGHT,m_SnapGrid);
			SetCursorPos(m_MousePos);
			Invalidate();
			break;
		case VK_UP:	///up arrow, move cursor
			NextSnap(VK_UP,m_SnapGrid);
			SetCursorPos(m_MousePos);
			Invalidate();
			break;
		case VK_DOWN:	///down arrow,movecursor
			NextSnap(VK_DOWN,m_SnapGrid);
			SetCursorPos(m_MousePos);
			Invalidate();
			break;
		case 'A':	///A key, draw arc
			OnToolbarArc();
			break;
		case 'E':	/// E key, draw Ellipse
			OnToolbarEllipse();
			break;
		case 'G':	///G key, toggle grid
			this->m_GridOn ^= 1;
			break;
		case 'L':	///L key, draw line
			OnToolbarLine();
			break;
		case 'P':	///P key, draw polygon
			OnToolbarPolywithfill();
			break;
		case 'R':	///R key, draw rectangle
			OnToolbarRect();
			break;
		case 'S':	///S key, toggle Snap Grid
			this->m_SnapOff ^= 1;
			break;
	}
	CView::OnKeyUp(nChar, nRepCnt, nFlags);
}

void CFrontCadView::OnLButtonDown(UINT nFlags, CPoint point)
{
	///-------------------------------------------
	/// OnLButtonDown
	///		This function perfroms the functions
	/// when the mouse button is presses.
	///
	///	parameters:
	///		nFlags.....status flaGs of mouse press
	///		point......point where mouse was pressed
	///-------------------------------------------
	// TODO: Add your message handler code here and/or call default
	CFrontCadApp *pA = (CFrontCadApp *)AfxGetApp();
	CFrontCadDoc *pDoc = GetDocument();
	CUtilView *pUV = this->GetUtilityView();

	/// correct mouse position for scrolling, etc
	m_MousePos = CorrectMousePosition(point);
	/// Snap the mouse posiition
	m_SnapPos = Snap(m_MousePos,m_SnapGrid);
	this->m_MouseState = MOUSESTATE_DOWN;
	///do actions depending of Drawmode state
	switch(m_Drawmode)
	{
		case DRAWMODE_NONE:
			break;
		case DRAWMODE_LINE:
			switch (m_DrawState)
			{
				case DRAWSTATE_WAITMOUSE_DOWN:
					{
						CCadLine *pLine;

						pA->m_LineAttrib.m_Width = pUV->m_Edit_LineThickness.GetValue();
						pA->m_LineAttrib.m_Color = pUV->m_Static_LineColor.GetColor();
						pLine = new CCadLine();
						pLine->SetLineColor(pA->m_LineAttrib.m_Color);
						pLine->SetOutLineWidth(pA->m_LineAttrib.m_Width);
						this->m_pDrawObject = (CCadObject *)pLine;
						m_pDrawObject->m_P1 = m_SnapPos;
						m_pDrawObject->m_P2 = m_pDrawObject->m_P1;
					}
					break;
				case DRAWSTATE_MOVE:
					break;
			}
			break;
		case DRAWMODE_RECT:	///Button Down
			switch (m_DrawState)
			{
				case DRAWSTATE_WAITMOUSE_DOWN:
					{
						CCadRect *pCR = new CCadRect();
						pA->m_RectAttributes.m_Width = pUV->m_Edit_LineThickness.GetValue();
						pA->m_RectAttributes.m_LineColor = pUV->m_Static_LineColor.GetColor();
						pA->m_RectAttributes.m_FillColor = pUV->m_Static_FillColor.GetColor();
						pCR->SetFillColor(pA->m_RectAttributes.m_FillColor);
						pCR->SetLineColor(pA->m_RectAttributes.m_LineColor);
						pCR->SetOutLineWidth(pA->m_RectAttributes.m_Width);
						m_pDrawObject = (CCadObject *)pCR;
						m_pDrawObject->m_P1 = m_SnapPos;
						m_pDrawObject->m_P2 = m_pDrawObject->m_P1;
					}
					break;
				case DRAWSTATE_MOVE:
					break;
			}
			break;
		case DRAWMODE_ELIPSE:
			switch (m_DrawState)
			{
				case DRAWSTATE_WAITMOUSE_DOWN:
					{
						CUtilView *pUV = GetUtilityView();
						CCadElipse *pCE = new CCadElipse();
						pA->m_EllipseAttributes.m_Width = pUV->m_Edit_LineThickness.GetValue();
						pA->m_EllipseAttributes.m_LineColor = pUV->m_Static_LineColor.GetColor();
						pA->m_EllipseAttributes.m_FillColor = pUV->m_Static_FillColor.GetColor();
						pCE->SetOutLineWidth(pA->m_EllipseAttributes.m_Width);
						pCE->SetFillColor(pA->m_EllipseAttributes.m_FillColor);
						pCE->SetLineColor(pA->m_EllipseAttributes.m_LineColor);
						m_pDrawObject = (CCadObject *)pCE;
						m_pDrawObject->m_P1 = m_SnapPos;
						m_pDrawObject->m_P2 = m_pDrawObject->m_P1;
				}
					break;
				case DRAWSTATE_MOVE:
					break;
			}
			break;
		case DRAWMODE_SELECT:///MOUSE DOWN
			//----------------------------------
			// Now we are getting complicated.
			// We do not know, at this junksure
			// if the user is just selecting an
			// object, or wants to maybe drag
			// something.
			//----------------------------------
			if(m_pSelObjList)	///object selected?
			{
				CCadObject *pOb = m_pSelObjList;
				m_GrabbedVertex = pOb->GrabVertex(m_SnapPos);
				if(m_GrabbedVertex < 0)
				{
					if(pOb->CheckSelected(Scale(m_MousePos)))
						m_DragObject = 1;
				}
			}
			break;
		case DRAWMODE_RNDRECT:
			switch (m_DrawState)
			{
				case DRAWSTATE_WAITMOUSE_DOWN:
					{
						///update rounded rect paramters
						pA->m_RndRectAttributes.m_OutLineWidth = pUV->m_Edit_LineThickness.GetValue();
						pA->m_RndRectAttributes.m_X3 = pUV->m_Edit_X3.GetValue();
						pA->m_RndRectAttributes.m_Y3 = pUV->m_Edit_Y3.GetValue();
						pA->m_RndRectAttributes.m_LineColor = pUV->m_Static_LineColor.GetColor();
						pA->m_RndRectAttributes.m_FillColor = pUV->m_Static_FillColor.GetColor();
						CCadRoundRect *pRRect = new CCadRoundRect;
						pRRect->SetArc(CPoint(pA->m_RndRectAttributes.m_X3, pA->m_RndRectAttributes.m_Y3));
						pRRect->SetLineWidth(pA->m_RndRectAttributes.m_OutLineWidth);
						pRRect->SetLineColor(pA->m_RndRectAttributes.m_LineColor);
						pRRect->SetFillColor(pA->m_RndRectAttributes.m_FillColor);
						m_pDrawObject = (CCadObject *)pRRect;
						m_pDrawObject->m_P1 = m_SnapPos;
						m_pDrawObject->m_P2 = m_pDrawObject->m_P1;
				}
					break;
				case DRAWSTATE_MOVE:
					break;
			}
			break;
		case DRAWMODE_POLYGON:///MOUSE DOWN
			{
				switch(m_DrawState)
				{
					case DRAWSTATE_WAITMOUSE_DOWN:	///This is where we start
						{
							CUtilView *pUV = this->GetUtilityView();
							pA->m_PolyAttributes.m_Width = pUV->m_Edit_LineThickness.GetValue();;
							pA->m_PolyAttributes.m_LineColor = pUV->m_Static_LineColor.GetColor();
							CCadPolygon *pCP = new CCadPolygon(100);
							pCP->AddPoint(m_SnapPos);
							pCP->SetLineColor(pA->m_PolyAttributes.m_LineColor);
							pCP->SetWidth(pA->m_PolyAttributes.m_Width);
							m_pDrawObject = (CCadObject *)pCP;
							m_PolyStart = m_SnapPos;
						}
						break;
					case DRAWSTATE_MOVE:
						{
							CCadPolygon *pCP = (CCadPolygon *)m_pDrawObject;
							pCP->AddPoint(m_SnapPos);
						}
						break;
				}
			}
			break;
		case DRAWMODE_POLYFILL:	//mouse down
			{
				switch(m_DrawState)
				{
					case DRAWSTATE_WAITMOUSE_DOWN:
						{
							CUtilView *pUV = this->GetUtilityView();
							pA->m_PolyFillAttributes.m_Width = pUV->m_Edit_LineThickness.GetValue();;
							pA->m_PolyFillAttributes.m_LineColor = pUV->m_Static_LineColor.GetColor();
							pA->m_PolyFillAttributes.m_FillColor = pUV->m_Static_FillColor.GetColor();
							CCadPolygonFill *pCP = new CCadPolygonFill(100);
							pCP->AddPoint(m_SnapPos);
							pCP->SetLineColor(pA->m_PolyFillAttributes.m_LineColor);
							pCP->SetFillColor(pA->m_PolyFillAttributes.m_FillColor);
							pCP->SetWidth(pA->m_PolyFillAttributes.m_Width);
							m_pDrawObject = (CCadObject *)pCP;
							m_PolyStart = m_SnapPos;
						}
						break;
					case DRAWSTATE_MOVE:
						{
							CCadPolygonFill *pCP = (CCadPolygonFill *)m_pDrawObject;
							pCP->AddPoint(m_SnapPos);
						}
						break;
				}
			}
			break;
		case DRAWMODE_ARC:	//mouse down
			{
				CCadArc *pCA;
				switch(m_DrawState)
				{
					case DRAWSTATE_WAITMOUSE_DOWN:
						{
							pA->m_ArcAttributes.m_Width = pUV->m_Edit_LineThickness.GetValue();;
							pA->m_ArcAttributes.m_LineColor = pUV->m_Static_LineColor.GetColor();
							pCA = new CCadArc;
							pCA->SetLineColor(pA->m_ArcAttributes.m_LineColor);
							pCA->m_P1 = m_SnapPos;
							pCA->m_P2 = m_SnapPos;
							pCA->SetWidth(pA->m_ArcAttributes.m_Width);
							m_pDrawObject = (CCadObject *)pCA;
						}
						break;
					case DRAWSTATE_MOVE:
						break;
					case DRAWSTATE_ARCSTART:
						pCA = (CCadArc *)m_pDrawObject;
						pCA->SetStartPoint(m_SnapPos);
						break;
					case DRAWSTATE_ARCEND:
						break;
				}
			}
			break;
		case DRAWMODE_ARC_CENTER:	//Mouse Down
			{
				switch(m_DrawState)
				{
					case DRAWSTATE_WAITMOUSE_DOWN:
						{
							pA->m_ArcAttributes.m_Width = pUV->m_Edit_LineThickness.GetValue();
							pA->m_ArcAttributes.m_LineColor = pUV->m_Static_LineColor.GetColor();
							CCadArcCentered *pCA = new CCadArcCentered;
							pCA->SetLineColor(pA->m_ArcAttributes.m_LineColor);
							pCA->m_P1 = pCA->m_P2 = m_SnapPos;
							pCA->SetWidth(pA->m_ArcAttributes.m_Width);
							m_pDrawObject = (CCadObject *)pCA;
						}
						break;
					case DRAWSTATE_MOVE:
						break;
					case DRAWSTATE_ARCSTART:
						{
							CCadArcCentered *pCA = (CCadArcCentered *)m_pDrawObject;
							pCA->SetStartPoint(m_SnapPos);
						}
						break;
					case DRAWSTATE_ARCEND:
					{
						CCadArcCentered *pCA = (CCadArcCentered *)m_pDrawObject;
						pCA->SetEndPoint(m_SnapPos);
					}
					break;
				}
			}
			break;
		case DRAWMODE_HOLE_ROUND:	//Mouse Down
			{
				CCadHoleRound *pHR;
				switch (m_DrawState)
				{
					case DRAWSTATE_WAITMOUSE_DOWN:
						pA->m_HoleRoundAttributes.m_Radius = pUV->m_Edit_HoleRadius.GetValue();
						pA->m_HoleRoundAttributes.m_Width = pUV->m_Edit_LineThickness.GetValue();
						pA->m_HoleRoundAttributes.m_LineColor = pUV->m_Static_LineColor.GetColor();
						pHR = new CCadHoleRound;
						pHR->m_P1 = m_SnapPos;
						pHR->SetRadius(pA->m_HoleRoundAttributes.m_Radius);
						pHR->SetLineColor(pA->m_HoleRoundAttributes.m_LineColor);
						pHR->SetWidth(pA->m_HoleRoundAttributes.m_Width);
						m_pDrawObject = (CCadObject *)pHR;
						Invalidate();
						break;
					case DRAWSTATE_MOVE:
						Invalidate();
						break;
				}
			}
			break;
		case DRAWMODE_HOLE_RECT:
			{
				CCadRectHole *pRH;

				switch (m_DrawState)
				{
					case DRAWSTATE_WAITMOUSE_DOWN:
						pRH = new CCadRectHole;
						pA->m_RectHoleAttributes.m_H = pUV->m_Edit_X2.GetValue();
						pA->m_RectHoleAttributes.m_W = pUV->m_Edit_Y2.GetValue();
						pA->m_RectHoleAttributes.m_Width = pUV->m_Edit_LineThickness.GetValue();
						pA->m_RectHoleAttributes.m_LineColor = pUV->m_Static_LineColor.GetColor();
						pRH->m_P1 = m_SnapPos;
						pRH->SetHieghtWidth(pA->m_RectHoleAttributes.m_H, pA->m_RectHoleAttributes.m_W);
						pRH->SetLineColor(pA->m_RectHoleAttributes.m_LineColor);
						pRH->SetOutLineWidth(pA->m_RectHoleAttributes.m_Width);
						m_pDrawObject = (CCadObject*)pRH;
						Invalidate();
						break;
					case DRAWSTATE_MOVE:
						Invalidate();
						break;
				}
			}
			break;
		case DRAWMODE_HOLE_RND1F:
			{
				CCadHoleRnd1Flat *pHR;

				switch (m_DrawState)
				{
					case DRAWSTATE_WAITMOUSE_DOWN:
						pA->m_HoleRnd1FlatAttributes.m_Radius = pUV->m_Edit_HoleRadius.GetValue();
						pA->m_HoleRnd1FlatAttributes.m_Width = pUV->m_Edit_LineThickness.GetValue();;
						pA->m_HoleRnd1FlatAttributes.m_LineColor = pUV->m_Static_LineColor.GetColor();
						pA->m_HoleRnd1FlatAttributes.m_FlatDist = pUV->m_Edit_FlatToCenterDist.GetValue();
						pHR = new CCadHoleRnd1Flat;
						pHR->m_P1 = m_SnapPos;
						pHR->SetRadius(pA->m_HoleRnd1FlatAttributes.m_Radius);
						pHR->SetLineColor(pA->m_HoleRnd1FlatAttributes.m_LineColor);
						pHR->SetWidth(pA->m_HoleRnd1FlatAttributes.m_Width);
						pHR->SetFlatDist(pA->m_HoleRnd1FlatAttributes.m_FlatDist);
						m_pDrawObject = (CCadObject *)pHR;
						Invalidate();
						break;
					case DRAWSTATE_MOVE:
						break;
				}
			}
			break;
		case DRAWMODE_HOLE_RND2F:
			{
				CCadHoleRnd2Flat *pHR;

				switch (m_DrawState)
				{
					case DRAWSTATE_WAITMOUSE_DOWN:
						pA->m_HoleRnd2FlatAttributes.m_Radius = pUV->m_Edit_HoleRadius.GetValue();
						pA->m_HoleRnd2FlatAttributes.m_Width = pUV->m_Edit_LineThickness.GetValue();
						pA->m_HoleRnd2FlatAttributes.m_LineColor = pUV->m_Static_LineColor.GetColor();
						pA->m_HoleRnd2FlatAttributes.m_FlatDist = pUV->m_Edit_FlatToCenterDist.GetValue();
						pHR = new CCadHoleRnd2Flat;
						pHR->m_P1 = m_SnapPos;
						pHR->SetRadius(pA->m_HoleRnd2FlatAttributes.m_Radius);
						pHR->SetLineColor(pA->m_HoleRnd2FlatAttributes.m_LineColor);
						pHR->SetWidth(pA->m_HoleRnd2FlatAttributes.m_Width);
						pHR->SetFlatDist(pA->m_HoleRnd2FlatAttributes.m_FlatDist);
						m_pDrawObject = (CCadObject *)pHR;
						Invalidate();
						break;
					case DRAWSTATE_MOVE:
						Invalidate();
						break;
				}
			}
			break;
		case DRAWMODE_TEXT:	//Mouse Down
			{
				char *s = new char[256];
				CUtilView *pUV = this->GetUtilityView();
				pA->m_TextAttributes.m_Angle = pUV->m_Edit_TextAngle.GetValue();
				pA->m_TextAttributes.m_FontHeight = pUV->m_Edit_FontHeight.GetValue();
				pA->m_TextAttributes.m_FontWidth = pUV->m_Edit_FontWidth.GetValue();
				pA->m_TextAttributes.m_Transparent = pUV->m_Check_TransparentFont.GetCheck();
				pA->m_TextAttributes.m_Color = pUV->m_Static_TextColor.GetColor();
				pA->m_TextAttributes.m_BkColor = pUV->m_Static_BkGrndColor.GetColor();
				pA->m_TextAttributes.m_Weight = pUV->m_Combo_FontWeight.GetFontWeight();
				pUV->m_Button_Font.GetWindowText(s,255);
				strcpy_s(pA->m_TextAttributes.m_pFontName, LF_FACESIZE,s);
				if(pA->m_TextAttributes.m_pText) delete[] pA->m_TextAttributes.m_pText;
				pUV->m_Edit_Text.GetWindowText(s,255);
				pA->m_TextAttributes.m_pText = new char[strlen(s)+1];
				strcpy_s(pA->m_TextAttributes.m_pText, strlen(s) + 1,s);
				CCadText *pCT = new CCadText;
				pCT->m_P1 = m_SnapPos;
				pCT->SettAttrib(pA->m_TextAttributes);
				pDoc->AddObject(pCT);
				delete[] s;
				Invalidate();
			}
			break;
		case DRAWMODE_LIBPART:
			{
				CLibFormView *pLFV = ((CFrontCadApp *)AfxGetApp())->m_pLibView;
				CCadLibObject *pLO = pLFV->GetCurrentPart();
				if(pLO)
				{
					CCadLibObject *pPart = new CCadLibObject(pLO->GetHead());
					pPart->SetName(pLO->m_pName);
					pPart->m_P1 = m_SnapPos;
					pDoc->AddObject(pPart);
					Invalidate();
				}
				else
				{
					m_Drawmode = DRAWMODE_SELECT;
					MessageBox("No Libraries or Parts Selected","Oppsie",MB_OK | MB_ICONHAND);
				}
			}
			break;
		case DRAWMODE_GETREF:
			break;
		case DRAWMODE_MOVE:		/// mouse down
			switch(this->m_DrawState)
			{
				case DRAWSTATE_GETREFEREMCE:
					{
						m_RefPoint = m_SnapPos;
						CCadObject *pObj = GetTopSelection();
						m_pMoveObj = new CMoveObjects;
						while(pObj)
						{
							pDoc->RemoveObject(pObj);
							pDoc->CheckPrev();
							RemoveObject(pObj);
							pDoc->CheckPrev();
							m_pMoveObj->AddObject(pObj);
							pDoc->CheckPrev();
							pObj = GetTopSelection();
							pDoc->CheckPrev();
						}
					}
					m_pMoveObj->SetRef(m_SnapPos);
//					m_pMoveObj = &m_MoveObj;
					Invalidate();
					break;
				case DRAWSTATE_DRAG:
					break;
			}
			break;
		case DRAWMODE_COPY:	///mouse button down
			switch(m_DrawState)
			{
				case DRAWSTATE_GETREFEREMCE:
					{
						///----------------------------------------------
						///for copy, leave the objects behind in the
						///drawing, but make a new copy of the objects
						///to put into the "clipboard"
						///----------------------------------------------
//						m_RefPoint = m_SnapPos;
						CCadObject *pObj = this->GetTopSelection();
						CCadObject *newObj;
						if (m_pClipBoard == NULL) m_pClipBoard = new CMoveObjects;
						m_pClipBoard->Clear(1);
						while(pObj)
						{
							newObj = pObj->CopyObject();
							m_pClipBoard->AddObject(newObj);
							RemoveObject(pObj);
							pObj->SetSelected();
							pObj = GetTopSelection();
						}
						m_pClipBoard->SetRef(m_SnapPos);
						Invalidate();
					}
					break;
				case DRAWSTATE_DRAG:
					break;
			}
			break;
		case DRAWMODE_PASTE:		//mouse down
			switch (m_DrawState)
			{
				case DRAWSTATE_PLACE:
					m_pMoveObj = new CMoveObjects;
					m_pMoveObj->Copy(*m_pClipBoard);
					Invalidate();
					break;
				case DRAWSTATE_DRAG:
					break;
			}
			break;
		case DRAWMODE_CUT:		//Mouse Down
			switch(m_DrawState)
			{
				case DRAWSTATE_GETREFEREMCE:
					{
						///----------------------------------------------
						///for copy, leave the objects behind in the
						///drawing, but make a new copy of the objects
						///to put into the "clipboard"
						///----------------------------------------------
						CCadObject *pObj = this->GetTopSelection();
						if (m_pClipBoard == 0) m_pClipBoard = new CMoveObjects;
						m_pClipBoard->Clear(1);	//delete objects in clipboard
						while(pObj)
						{
							RemoveObject(pObj);	//remove from sel list
							//remove from drawing
							pDoc->RemoveObject(pObj);
							///add to clipboard
							m_pClipBoard->AddObject(pObj);
							///get next obje3ct
							pObj = GetTopSelection();
						}
						m_pClipBoard->SetRef(m_SnapPos);
						Invalidate();
					}
					break;
				case DRAWSTATE_DRAG:
					break;
			}
			break;
		case DRAWMODE_SELECTREGION:	//Button Down
			if (m_nSelRegionLock)
			{
				m_nSelRegionLock = 0;
				m_Drawmode = DRAWMODE_SELECT;
			}
			else
			{
				m_nSelRegionLock = 1;
				m_SelRegion_P1 = m_SnapPos;
				m_SelRegion_P2 = m_SnapPos;
				ClearSelList();
			}
			break;
		case DRAWMODE_BITMAP:
			m_pDrawObject = m_pTempCadBitmap;
			m_pDrawObject->m_P1 = m_SnapPos;
			break;
		case DRAWMODE_ARROW:	///Button Down
			switch (m_DrawState)
			{
				case DRAWSTATE_WAITMOUSE_DOWN:
				{
					CCadArrow *pArrow = new CCadArrow;
					pA->m_ArrowAttrib.m_Color = pUV->m_Static_FillColor.GetColor();
					pA->m_ArrowAttrib.m_Len = pUV->m_Edit_X3.GetValue();
					pA->m_ArrowAttrib.m_LineWidth = pUV->m_Edit_Y3.GetValue();
					pArrow->SetColor(pA->m_ArrowAttrib.m_Color);
					pArrow->SetWidth(pA->m_ArrowAttrib.m_LineWidth);
					pArrow->SetLegnth(pA->m_ArrowAttrib.m_Len);
					m_pDrawObject = (CCadObject *)pArrow;
					pArrow->m_P1 = m_SnapPos;
					pArrow->m_P2 = m_SnapPos;
				}
				break;
				case DRAWSTATE_MOVE:
					break;
				}
				Invalidate();
				break;
		case DRAWMODE_ORIGIN:	//button down
			switch (m_DrawState)
			{
				case DRAWSTATE_WAITMOUSE_DOWN:
					{
						pA->m_OriginAttrib.m_Color = pUV->m_Static_LineColor.GetColor();
						pA->m_OriginAttrib.m_Width = pUV->m_Edit_LineThickness.GetValue();
						CCadOrigin *pOrg = new CCadOrigin;
						pOrg->SetParent(this);
						pOrg->SetColor(pA->m_OriginAttrib.m_Color);
						pOrg->SetWidth(pA->m_OriginAttrib.m_Width);
						m_pDrawObject = (CCadObject *)pOrg;
						m_pDrawObject->m_P1 = m_SnapPos;
					}
					break;
				case DRAWSTATE_MOVE:
					break;
				case DRAWSTATE_PLACE:
					break;
			}
			Invalidate();
			break;
		case DRAWMODE_DIMENSION:	//mouse button down
			{
				CCadDimension *pCD;
				char *s = new char[256];
				switch (m_DrawState)
				{
					case DRAWSTATE_WAITMOUSE_DOWN:
						//---------------------------------
						// Update Dimension Settings
						//---------------------------------
						// Line Attributes
						//---------------------------------
						pA->m_DimAttrib.m_Color = pUV->m_Static_LineColor.GetColor();
						pA->m_DimAttrib.m_Width = pUV->m_Edit_LineThickness.GetValue();
						//----------------------------------
						// Text Attributes
						//-----------------------------------
						
						pA->m_DimAttrib.m_Text.m_Angle = pUV->m_Edit_TextAngle.GetValue();
						pA->m_DimAttrib.m_Text.m_FontHeight = pUV->m_Edit_FontHeight.GetValue();
						pA->m_DimAttrib.m_Text.m_FontWidth = pUV->m_Edit_FontWidth.GetValue();
						pA->m_DimAttrib.m_Text.m_Transparent = pUV->m_Check_TransparentFont.GetCheck();
						pA->m_DimAttrib.m_Text.m_Color = pUV->m_Static_TextColor.GetColor();
						pA->m_DimAttrib.m_Text.m_BkColor = pUV->m_Static_BkGrndColor.GetColor();
						pA->m_DimAttrib.m_Text.m_Weight = pUV->m_Combo_FontWeight.GetFontWeight();
//						if (pA->m_DimAttrib.m_Text.m_pFontName)delete[] pA->m_DimAttrib.m_Text.m_pFontName;
						pUV->m_Button_Font.GetWindowText(s, 255);
//						pA->m_DimAttrib.m_Text.m_pFontName = new char[strlen(s) + 1];
						strcpy_s(pA->m_DimAttrib.m_Text.m_pFontName, LF_FACESIZE, s);
						if (pA->m_DimAttrib.m_Text.m_pText) delete[] pA->m_DimAttrib.m_Text.m_pText;
						pUV->m_Edit_Text.GetWindowText(s, 255);
						pA->m_DimAttrib.m_Text.m_pText = new char[strlen(s) + 1];
						strcpy_s(pA->m_DimAttrib.m_Text.m_pText, strlen(s) + 1, s);

						pCD = new CCadDimension;
						m_pDrawObject = (CCadObject *)pCD;
						pCD->m_P2 = pCD->m_P1 = m_SnapPos;
						pCD->SetColor(pA->m_DimAttrib.m_Color);
						pCD->SetWidth(pA->m_DimAttrib.m_Width);
						pCD->GetText()->SetAngle(pA->m_DimAttrib.m_Text.m_Angle);
						pCD->GetText()->SetBkColor(pA->m_DimAttrib.m_Text.m_BkColor);
						pCD->GetText()->SetColor(pA->m_DimAttrib.m_Text.m_Color);
						pCD->GetText()->SetFontHeight(pA->m_DimAttrib.m_Text.m_FontHeight);
						pCD->GetText()->SetFontName(pA->m_DimAttrib.m_Text.m_pFontName);
						pCD->GetText()->SetFontWidth(pA->m_DimAttrib.m_Text.m_FontWidth);
						pCD->GetText()->SetFormat(pA->m_DimAttrib.m_Text.m_Format);
						pCD->GetText()->SetTransparent(pA->m_DimAttrib.m_Text.m_Transparent);
						pCD->GetText()->SetWeight(pA->m_DimAttrib.m_Text.m_Weight);
						Invalidate();
						break;
					case DRAWSTATE_MOVE:
						Invalidate();
						break;
				}
				delete[] s;
			}
			break;
		case DRAWMODE_ALIGN_DIM_TO_HOLE:	//Mouse button down
			switch (m_DrawState)
			{
				case DRAWSTATE_WAITMOUSE_DOWN:
					Invalidate();
					break;
			}
			break;
		case DRAWMODE_PRINTRECT:	//Mouse Button Down
			switch (m_DrawState)
			{
			case DRAWSTATE_WAITMOUSE_DOWN:
			{
				CCadPrintRect *pPR = new CCadPrintRect;
				pPR->m_P1 = m_SnapPos;
				pPR->SetOutLineWidth(40);
				pPR->SetLineColor(RGB(0, 0, 0));
				pPR->SetSize(pUV->m_Combo_PrintRect.GetPrintRectSize());
				m_pDrawObject = (CCadObject *)pPR;
				Invalidate();
			}
				break;
			case DRAWSTATE_MOVE:
				break;
			}
			break;
	}		 
	CView::OnLButtonDown(nFlags, point);
}

void CFrontCadView::OnLButtonUp(UINT nFlags, CPoint point)
{
	///-----------------------------------------------
	/// OnLButtonUp
	///		This function is used to perfrom functions
	/// when the mouse button is released.
	///	parameters:
	///		nFlags.....Mouse flags
	///		point......point where mouse button released
	///-----------------------------------------------
	// TODO: Add your message handler code here and/or call default

	m_MousePos = CorrectMousePosition(point);
	m_SnapPos = Snap(m_MousePos,m_SnapGrid);
	CFrontCadDoc *pDoc = GetDocument();
	m_MouseState = MOUSESTATE_UP;
	switch(m_Drawmode)
	{
		case DRAWMODE_NONE:
			theApp.GetMainFrame()->UpdateStatusBar("Select Object");
			break;
		case DRAWMODE_LINE:
			switch (m_DrawState)
			{
				case DRAWSTATE_WAITMOUSE_DOWN:
					theApp.GetMainFrame()->UpdateStatusBar("Line:Place Second Point");
					m_DrawState = DRAWSTATE_MOVE;
					break;
				case DRAWSTATE_MOVE:
					theApp.GetMainFrame()->UpdateStatusBar("Line:Place First Point");
					if (m_pDrawObject)
					{
						m_pDrawObject->m_P2 = m_SnapPos;
						pDoc->AddObject(m_pDrawObject);
						m_pDrawObject = 0;
						m_DrawState = DRAWSTATE_WAITMOUSE_DOWN;
					}
					break;
			}
			Invalidate();
			break;
		case DRAWMODE_RECT:	///Button Up
		case DRAWMODE_ELIPSE:
		case DRAWMODE_RNDRECT:
			switch (m_DrawState)
			{
				case DRAWSTATE_WAITMOUSE_DOWN:
					theApp.GetMainFrame()->UpdateStatusBar("Place Second Corner");
					m_DrawState = DRAWSTATE_MOVE;
					break;
				case DRAWSTATE_MOVE:
					if (m_pDrawObject)
					{
						m_pDrawObject->m_P2 = m_SnapPos;
						pDoc->AddObject(m_pDrawObject);
						m_pDrawObject = 0;
						m_DrawState = DRAWSTATE_WAITMOUSE_DOWN;
						theApp.GetMainFrame()->UpdateStatusBar("Place First Corner");
					}
					Invalidate();
					break;
			}
			break;
		case DRAWMODE_POLYGON:	///mouse up.
			{
				CCadPolygon *pCP = (CCadPolygon *)m_pDrawObject;
				switch(m_DrawState)
				{
					case DRAWSTATE_WAITMOUSE_DOWN:
						m_DrawState = DRAWSTATE_MOVE;
						pCP->AddPoint(m_SnapPos);
						theApp.GetMainFrame()->UpdateStatusBar("PolyGon:Place Next Point");
						break;
					case DRAWSTATE_MOVE:
						{
							pCP->DeleteLastPoint();
							if ((m_SnapPos.x == m_PolyStart.x) && (m_SnapPos.y == m_PolyStart.y))
							{	///we are back at the start point, terminate
								pDoc->AddObject(m_pDrawObject);
								m_DrawState = DRAWSTATE_WAITMOUSE_DOWN;
								theApp.GetMainFrame()->UpdateStatusBar("PolyGon:Place First Point");
								m_pDrawObject = 0;
							}
							else
							{
								pCP->AddPoint(m_SnapPos);
//								m_DrawState = DRAWSTATE_NEXTSEGMENT;
							}
						}
						Invalidate();
						break;
				}
			}
			Invalidate();
			break;
		case DRAWMODE_POLYFILL:	//mouse up
			{
				CCadPolygonFill *pCP = (CCadPolygonFill *)m_pDrawObject;
				switch(m_DrawState)
				{
					case DRAWSTATE_WAITMOUSE_DOWN:
						m_DrawState = DRAWSTATE_MOVE;
						pCP->AddPoint(m_SnapPos);
						theApp.GetMainFrame()->UpdateStatusBar("PolyGon:Place Next Point");
						break;
					case DRAWSTATE_MOVE:
						{
							pCP->DeleteLastPoint();
							if((m_SnapPos.x == m_PolyStart.x) && (m_SnapPos.y == m_PolyStart.y))
							{
								pDoc->AddObject(m_pDrawObject);
								m_DrawState = DRAWSTATE_WAITMOUSE_DOWN;
								theApp.GetMainFrame()->UpdateStatusBar("PolyGon:Place First Point");
								m_pDrawObject = 0;
							}
							else
							{
								pCP->AddPoint(m_SnapPos);
							}
						}
						break;
				}
			}
			Invalidate();
			break;
		case DRAWMODE_ARC:	///Mouse Up
			{
				CCadArc *pCA = (CCadArc *)m_pDrawObject;
				switch(m_DrawState)
				{
					case DRAWSTATE_WAITMOUSE_DOWN:
						pCA->m_P1 = m_SnapPos;
						pCA->m_P2 = m_SnapPos;
						m_DrawState = DRAWSTATE_MOVE;
						theApp.GetMainFrame()->UpdateStatusBar("Arc:Place Second Corner");
						break;
					case DRAWSTATE_MOVE:
						m_pDrawObject->m_P2 = m_SnapPos;
						m_DrawState = DRAWSTATE_ARCSTART;
						theApp.GetMainFrame()->UpdateStatusBar("Arc:Place Start of Arc");
						break;
					case DRAWSTATE_ARCSTART:
						pCA->SetStartPoint(m_SnapPos);
						m_DrawState = DRAWSTATE_ARCEND;
						theApp.GetMainFrame()->UpdateStatusBar("Arc:Place End of Arc");
						break;
					case DRAWSTATE_ARCEND:
						pCA->SetEndPoint(m_SnapPos);
						pDoc->AddObject(m_pDrawObject);
						m_pDrawObject = 0;
						m_DrawState = DRAWSTATE_WAITMOUSE_DOWN;
						theApp.GetMainFrame()->UpdateStatusBar("Arc:Place First Corner");
						break;
				}
			}
			Invalidate();
			break;
		case DRAWMODE_ARC_CENTER:	//Mouse Up	
			{
				CCadArcCentered *pCA = (CCadArcCentered *)m_pDrawObject;
				switch(m_DrawState)
				{
					case DRAWSTATE_WAITMOUSE_DOWN:
						pCA->m_P1 = m_SnapPos;
						m_DrawState = DRAWSTATE_MOVE;
						theApp.GetMainFrame()->UpdateStatusBar("Arc:Place Defining Rectangle");
						break;
					case DRAWSTATE_MOVE:
						m_pDrawObject->m_P2 = m_SnapPos;
						m_DrawState = DRAWSTATE_ARCSTART;
						pCA->SetStartPoint(pCA->m_P2);
						theApp.GetMainFrame()->UpdateStatusBar("Arc:Place Start of Arc");
						break;
					case DRAWSTATE_ARCSTART:
						pCA->SetStartPoint(m_SnapPos);
						m_DrawState = DRAWSTATE_ARCEND;
						pCA->SetEndPoint(pCA->GetStartPoint());
						theApp.GetMainFrame()->UpdateStatusBar("Arc:Place End of Arc");
						break;
					case DRAWSTATE_ARCEND:
						pCA->SetEndPoint(m_SnapPos);
						m_DrawState = DRAWSTATE_WAITMOUSE_DOWN;
						pDoc->AddObject(m_pDrawObject);
						theApp.GetMainFrame()->UpdateStatusBar("Arc:Place Center of Arc");
						m_pDrawObject = 0;
						break;
				}
			}
			Invalidate();
			break;
		case DRAWMODE_SELECT:	//MOUSE UP
			m_pDrawObject = 0;
			if(m_pSelObjList && (( 0 <= m_GrabbedVertex) || m_DragObject))	//a part is selected
			{
				CCadObject *pOb = m_pSelObjList;
				if(pOb->m_Selected)
				{
					pOb->m_Selected = 0;
					this->RemoveObject(pOb);
					CUtilView *pUV = GetUtilityView();
					if(this->m_pSelObjList)
					{
						FillInUtilView(pUV,m_pSelObjList);
						pUV->ShowHideForSelected(m_pSelObjList->GetType());
					}
					else
						pUV->ShowHideForSelected(0);

					Invalidate();
				}
				m_GrabbedVertex = -1;
				m_DragObject = 0;
			}
			else
			{
				CCadObject **selObjs = new CCadObject*[8];
				int c;
				c = pDoc->CheckSelected(Scale(m_MousePos),selObjs,8);
				if(c > 1)
				{
					CMenu Menu;
					Menu.CreatePopupMenu();
					int i;
					char *s = new char[256];
					for(i=0;i<c;++i)
					{
						sprintf_s(s,256,"%s:(%d,%d)",selObjs[i]->GetTypeString(),selObjs[i]->m_P1.x,selObjs[i]->m_P1.y);
						Menu.AppendMenu(MF_STRING,ID_OBJECTSEL+i,s);
					}
					for(i=0;i<c;++i)
						Menu.EnableMenuItem(ID_OBJECTSEL+i,MF_ENABLED);
					CRect rect;
					GetWindowRect(&rect);
					CPoint Offset = point + rect.TopLeft();
					int id = Menu.TrackPopupMenu(TPM_RETURNCMD | TPM_LEFTALIGN | TPM_LEFTBUTTON | TPM_RIGHTBUTTON, Offset.x, Offset.y,this);
					if(id > 0)
					{
						selObjs[id-ID_OBJECTSEL]->m_Selected ^= 1;;
						if(selObjs[id-ID_OBJECTSEL]->m_Selected)
						{
							this->AddToSelList(selObjs[id-ID_OBJECTSEL]);
							CUtilView *pUV = GetUtilityView();
							FillInUtilView(pUV,selObjs[id-ID_OBJECTSEL]);
							pUV->ShowHideForSelected(selObjs[id-ID_OBJECTSEL]->GetType());
							pUV->SetupTextSelected(selObjs[id-ID_OBJECTSEL]->GetType());
							Invalidate();
						}
					}
					delete[] s;
				}
				else if(c==1)
				{
					selObjs[0]->m_Selected ^= 1;
					if(selObjs[0]->m_Selected)
					{
						this->AddToSelList(selObjs[0]);
						CUtilView *pUV = GetUtilityView();
						FillInUtilView(pUV,selObjs[0]);
						pUV->ShowHideForSelected(selObjs[0]->GetType());
						pUV->SetupTextSelected(selObjs[0]->GetType());

					}
					Invalidate();
				}
				//don't do anything if c==0
				delete[] selObjs;
			}
			break;
		case DRAWMODE_HOLE_ROUND:	//Mouse Up
		case DRAWMODE_HOLE_RECT:
		case DRAWMODE_HOLE_RND1F:
		case DRAWMODE_HOLE_RND2F:
			switch (m_DrawState)
			{
				case DRAWSTATE_WAITMOUSE_DOWN:
					m_DrawState = DRAWSTATE_MOVE;
					m_pDrawObject->m_P1 = m_SnapPos;
					Invalidate();
					theApp.GetMainFrame()->UpdateStatusBar("Hole:Place Center of Hole");
					break;
				case DRAWSTATE_MOVE:
					m_pDrawObject->m_P1 = m_SnapPos;
					pDoc->AddObject(m_pDrawObject);
					m_pDrawObject = 0;
					m_DrawState = DRAWSTATE_WAITMOUSE_DOWN;
					theApp.GetMainFrame()->UpdateStatusBar("Hole:Locate Center of Hole");
					Invalidate();
					break;
			}
			break;
		case DRAWMODE_TEXT:		//mouse up
			m_pDrawObject = 0;
			break;
		case DRAWMODE_LIBPART:	///mouse up
			m_pDrawObject = 0;
			break;
		case DRAWMODE_GETREF:
			m_Drawmode = 0;	// no draw mode
			{
				CPoint Ref = this->m_SnapPos;
				((CFrontCadApp *)AfxGetApp())->m_pLibView->SendMessage(WM_MAINVIEW_GOTREF,Ref.x,Ref.y);
			}
			break;
		case DRAWMODE_MOVE:		//mouse up
			switch(m_DrawState)
			{
				case DRAWSTATE_GETREFEREMCE:
					m_DrawState = DRAWSTATE_DRAG;
					break;
					theApp.GetMainFrame()->UpdateStatusBar("Move:Drag Objects");
				case DRAWSTATE_DRAG:
					{
						CCadObject *pO = m_pMoveObj->GetHead();
						pDoc->CheckPrev();
						while(pO)
						{
							pO->m_Selected = 0;
							m_pMoveObj->RemoveObject(pO);
							pDoc->AddObject(pO);
							pO->AdjustRefernce(-m_SnapPos);
							pO = m_pMoveObj->GetHead();
						}
					}
					delete m_pMoveObj;
					m_pMoveObj = 0;
					m_Drawmode = DRAWMODE_SELECT;
					theApp.GetMainFrame()->UpdateStatusBar("Select Object");
					Invalidate();
					break;
			}
			break;
		case DRAWMODE_COPY:	//mouse button up
			switch(this->m_DrawState)
			{
				case DRAWSTATE_GETREFEREMCE:
					m_Drawmode = DRAWMODE_SELECT;
					theApp.GetMainFrame()->UpdateStatusBar("Select Object");
					Invalidate();
					break;
				case DRAWSTATE_DRAG:
					Invalidate();
					break;
			}
			break;
		case DRAWMODE_PASTE:	//mouse button up
			switch (m_DrawState)
			{
				case DRAWSTATE_PLACE:
					m_DrawState = DRAWSTATE_DRAG;
					theApp.GetMainFrame()->UpdateStatusBar("Drag Object(s)");
					break;
				case DRAWSTATE_DRAG:
					{
						CCadObject *pObj;
						do
						{
							pObj = m_pMoveObj->RemoveFromHead();
							if (pObj)
							{
								pObj->m_Selected = 0;
								pObj->AdjustRefernce(-m_SnapPos);
								pDoc->AddObject(pObj);
							}
						} while (pObj);
						delete m_pMoveObj;
						m_pMoveObj = 0;
						Invalidate();
					}
					m_Drawmode = DRAWMODE_SELECT;
					m_DrawState = DRAWSTATE_WAITMOUSE_DOWN;
					theApp.GetMainFrame()->UpdateStatusBar("Select Object");
					break;
			}
			break;
		case DRAWMODE_CUT:		//mouse up
			switch (m_DrawState)
			{
				case DRAWSTATE_GETREFEREMCE:
					m_Drawmode = DRAWMODE_SELECT;
					theApp.GetMainFrame()->UpdateStatusBar("Select Object");
					break;
				case DRAWSTATE_DRAG:
					break;
			}
			Invalidate();
			break;
		case DRAWMODE_SELECTREGION:		//Mouse Button UP
			m_SelRegion_P2 = m_SnapPos;
			{
				CRect Rsel,rect;
				Rsel.SetRect(m_SelRegion_P1,m_SelRegion_P2);
				Rsel.NormalizeRect();
				CCadDrawing *pD = pDoc->m_pDrawing;
				CCadObject *pObj = pD->GetHead();
//				CScale sf = CScale(ZF[m_ZoomLevel], ZF[m_ZoomLevel]);
				CScale sf = CScale(1.0, 1.0);
				while(pObj)
				{
					rect = pObj->GetRect();
					if (Rsel.PtInRect(sf * rect.TopLeft()) && Rsel.PtInRect(sf * rect.BottomRight()))
					{
						if (OBJECT_TYPE_ORIGIN != pObj->GetType())
						{
							pObj->SetSelected(1);
							this->AddToSelList(pObj);
						}
					}
					pObj = pObj->m_pNext;
				}
			}
			Invalidate();
//			m_Drawmode = DRAWMODE_SELECT;
			break;
		case DRAWMODE_BITMAP:
			if(m_pDrawObject)
			{
				m_pDrawObject->m_P2 = m_SnapPos;
				if(m_pDrawObject->m_P1 != m_pDrawObject->m_P2)
				{
					pDoc->AddObject(m_pDrawObject);
					m_pDrawObject = 0;
					m_pTempCadBitmap = 0;
				}
				else
				{
					delete m_pTempCadBitmap;
					m_pDrawObject = 0;
					m_pTempCadBitmap = 0;
				}
			}
			Invalidate();
			break;
		case DRAWMODE_ARROW:	///Button Up
			switch (m_DrawState)
			{
			case DRAWSTATE_WAITMOUSE_DOWN:
				m_pDrawObject->m_P1 = m_SnapPos;
				m_DrawState = DRAWSTATE_MOVE;
				theApp.GetMainFrame()->UpdateStatusBar("Arrow:Set Direction Arrow Points");
				break;
			case DRAWSTATE_MOVE:
				if (m_pDrawObject)
				{
					m_pDrawObject->m_P2 = m_SnapPos;
					if (m_pDrawObject->m_P1 != m_pDrawObject->m_P2)
					{
						pDoc->AddObject(m_pDrawObject);
						m_pDrawObject = 0;
						m_pTempCadBitmap = 0;
					}
					m_DrawState = DRAWSTATE_WAITMOUSE_DOWN;
					theApp.GetMainFrame()->UpdateStatusBar("Arrow:Place Location of Point");
				}
				break;
			}


			break;
		case DRAWMODE_ORIGIN:	//Left Button Up
			switch (m_DrawState)
			{
				case DRAWSTATE_WAITMOUSE_DOWN:
					m_DrawState = DRAWSTATE_MOVE;
					theApp.GetMainFrame()->UpdateStatusBar("Origin:Place Location");
					break;
				case DRAWSTATE_MOVE:
					m_pDrawObject->m_P1 = m_SnapPos;
					pDoc->AddObject(m_pDrawObject);
					m_pDrawObject = 0;
					m_DrawState = DRAWSTATE_WAITMOUSE_DOWN;
					break;
			}
			Invalidate();
			break;
		case DRAWMODE_DIMENSION:	//Mouse Up
		{
			CCadDimension *pCD;

			switch (m_DrawState)
			{
				case DRAWSTATE_WAITMOUSE_DOWN:
					pCD = (CCadDimension *)m_pDrawObject;
					pCD->m_P2 = pCD->m_P1 = m_SnapPos;
					m_DrawState = DRAWSTATE_MOVE;
					theApp.GetMainFrame()->UpdateStatusBar("Dimension:Place Second Point of Dimension");
					Invalidate();
					break;
				case DRAWSTATE_MOVE:
					pCD = (CCadDimension *)m_pDrawObject;
					{
						CPoint NewPos, UpdatedPos;
						NewPos = m_SnapPos;
						int diffx, diffy;
						diffx = abs(NewPos.x - pCD->m_P1.x);
						diffy = abs(NewPos.y - pCD->m_P1.y);
						if (diffx > diffy)
						{
							NewPos = CPoint(NewPos.x, pCD->m_P1.y);
						}
						else
						{
							NewPos = CPoint(pCD->m_P1.x, NewPos.y);
						}
						pCD->m_P2 = NewPos;

					}
					pCD->UpdateText(GetOrigin());
					pDoc->AddObject(pCD);
					m_pDrawObject = 0;
					m_DrawState = DRAWSTATE_WAITMOUSE_DOWN;
					theApp.GetMainFrame()->UpdateStatusBar("Dimension:Place First Point of Dimension");
					Invalidate();
					break;
			}
		}
		break;
		case DRAWMODE_ALIGN_DIM_TO_HOLE:	//Mouse button up.
			switch (m_DrawState)
			{
				case DRAWSTATE_WAITMOUSE_DOWN:
					{
						CCadObject *pHole,*pHead;
						pHead = pDoc->GetHead();
						pHole = FindHole(pHead, m_SnapPos);
						if (pHole)
						{
							CCadDimension *pDim;
							if (GetTopSelection()->GetType() == OBJECT_TYPE_DIMENSION)
							{
								pDim = (CCadDimension *)GetTopSelection();
								pDim->AlignToPoint(pHole->GetReference());
								pDim->UpdateText(GetOrigin());
								
							}
							Invalidate();
							m_Drawmode = DRAWMODE_SELECT;
							theApp.GetMainFrame()->UpdateStatusBar("Select Object");
						}
					}
					break;
			}
			break;
		case DRAWMODE_PRINTRECT:
			switch (m_DrawState)
			{
			case DRAWSTATE_WAITMOUSE_DOWN:
				m_pDrawObject->m_P1 = m_SnapPos;
				m_DrawState = DRAWSTATE_MOVE;
				theApp.GetMainFrame()->UpdateStatusBar("Print Rectangle:Place Second Corner");
				break;
			case DRAWSTATE_MOVE:
				m_pDrawObject->m_P1 = m_SnapPos;
				pDoc->InsertObject(m_pDrawObject);
				m_pDrawObject = 0;
				m_DrawState = DRAWSTATE_WAITMOUSE_DOWN;
				theApp.GetMainFrame()->UpdateStatusBar("Print Rectangle:Place First Corner");
				break;
			}
			break;
	}

	CView::OnLButtonUp(nFlags, point);
}

void CFrontCadView::OnMouseMove(UINT nFlags, CPoint point)
{
	///-----------------------------------------------
	/// OnMouseMove
	///		This function handles the functionality
	/// when the mouse is moved.
	///-----------------------------------------------
	// TODO: Add your message handler code here and/or call default
	CPoint LastPosition = m_SnapPos;
	m_MousePos = CorrectMousePosition(point);
	m_SnapPos = Snap(m_MousePos,m_SnapGrid);
	CMainFrame *pMF = (CMainFrame *)((CFrontCadApp *)AfxGetApp())->m_pMainWnd;
	pMF->SetXYPos(m_SnapPos.x,m_SnapPos.y);
    UpdateRulerInfo(RW_POSITION, CPoint(m_HScrollPos,m_VScrollPos), SnapToScreen(m_SnapPos));
	switch(m_Drawmode)
	{
		case DRAWMODE_NONE:
			Invalidate();
			break;
		case DRAWMODE_LINE:
			switch (m_DrawState)
			{
				case DRAWSTATE_WAITMOUSE_DOWN:
					Invalidate();
					break;
				case DRAWSTATE_MOVE:
					if (m_pDrawObject)
						m_pDrawObject->m_P2 = m_SnapPos;
					Invalidate();
					break;
			}
		case DRAWMODE_RECT:
		case DRAWMODE_ELIPSE:
		case DRAWMODE_RNDRECT:
			switch (m_DrawState)
			{
				case DRAWSTATE_WAITMOUSE_DOWN:
					Invalidate();
					break;
				case DRAWSTATE_MOVE:
					if (m_pDrawObject)
						m_pDrawObject->m_P2 = m_SnapPos;
					Invalidate();
					break;
			}
			break;
		case DRAWMODE_POLYGON:	///mouse move
			{
				CCadPolygon *pCP = (CCadPolygon *)m_pDrawObject;
				switch(m_DrawState)
				{
					case DRAWSTATE_WAITMOUSE_DOWN:
						Invalidate();
						break;
					case DRAWSTATE_MOVE:
						pCP->DeleteLastPoint();
						pCP->SetCurPoint(m_SnapPos);
						Invalidate();
						break;
				}
			}
			break;
		case DRAWMODE_POLYFILL:	//mouse move
			{
				CCadPolygonFill *pCP = (CCadPolygonFill *)m_pDrawObject;
				switch(m_DrawState)
				{
					case DRAWSTATE_WAITMOUSE_DOWN:
						Invalidate();
						break;
					case DRAWSTATE_MOVE:
						pCP->DeleteLastPoint();
						pCP->SetCurPoint(m_SnapPos);
						Invalidate();
						break;
				}
			}
			break;
		case DRAWMODE_ARC:	//mouse move
			{
				CCadArc *pCA = (CCadArc *)m_pDrawObject;
				switch(m_DrawState)
				{
					case DRAWSTATE_WAITMOUSE_DOWN:
						if(pCA) pCA->m_P1 = pCA->m_P2 = m_SnapPos;
						Invalidate();
						break;
					case DRAWSTATE_MOVE:
						{
							pCA->m_P2 = m_SnapPos;
						}
						Invalidate();
						break;
					case DRAWSTATE_ARCSTART:
						pCA->SetStartPoint(m_SnapPos);
						Invalidate();
						break;
					case DRAWSTATE_ARCEND:
						pCA->SetEndPoint(m_SnapPos);
						Invalidate();
						break;
				}
			}
			break;
		case DRAWMODE_ARC_CENTER:	//Mouse Move
			{
				CCadArcCentered *pCA = (CCadArcCentered *)m_pDrawObject;
				switch(m_DrawState)
				{
					case DRAWSTATE_WAITMOUSE_DOWN:
						Invalidate();
						break;
					case DRAWSTATE_MOVE:
						pCA->m_P2 = m_SnapPos;
						Invalidate();
						break;
					case DRAWSTATE_ARCSTART:
						pCA->SetStartPoint(m_SnapPos);
						Invalidate();
						break;
					case DRAWSTATE_ARCEND:
						pCA->SetEndPoint(m_SnapPos);
						Invalidate();
						break;
				}
			}
			break;
		case DRAWMODE_SELECT:	//MOUSE MOVE
			if(m_DragObject && m_pSelObjList)
			{
				CPoint Diff = m_SnapPos - LastPosition;
				m_pSelObjList->Move(m_pSelObjList->GetReference()+Diff);
			}
			else if(m_pSelObjList && (0 <= m_GrabbedVertex))	///object selected?
			{
				m_pSelObjList->SetVertex(m_GrabbedVertex,m_SnapPos);
			}
			Invalidate();
			break;
		case DRAWMODE_HOLE_ROUND:	///Mouse Move
		case DRAWMODE_HOLE_RECT:
		case DRAWMODE_HOLE_RND1F:
		case DRAWMODE_HOLE_RND2F:
			if (m_pDrawObject)
			{
				switch (m_DrawState)
				{
					case DRAWSTATE_WAITMOUSE_DOWN:
						m_pDrawObject->m_P1 = m_SnapPos;
						break;
					case DRAWSTATE_MOVE:
						m_pDrawObject->m_P1 = m_SnapPos;
						break;
				}
			}
			Invalidate();
			break;
		case DRAWMODE_MOVE:	//mouse move
			switch (m_DrawState)
			{
				case DRAWSTATE_GETREFEREMCE:
					break;
				case DRAWSTATE_DRAG:
					break;
			}
			Invalidate();
			break;
		case DRAWMODE_COPY:
		case DRAWMODE_CUT:
			switch (m_DrawState)
			{
				case DRAWSTATE_GETREFEREMCE:
					break;
				case DRAWSTATE_DRAG:
					break;
			}
			Invalidate();
			break;
		case DRAWMODE_PASTE:	//mouse move
			switch (m_DrawState)
			{
				case DRAWSTATE_PLACE:
					Invalidate();
					break;
				case DRAWSTATE_DRAG:
					Invalidate();
					break;
			}
			break;
		case DRAWMODE_TEXT:
		case DRAWMODE_LIBPART:
		case DRAWMODE_GETREF:
			Invalidate();
			break;
		case DRAWMODE_SELECTREGION:
			if(m_MouseState == MOUSESTATE_DOWN)
				m_SelRegion_P2 = m_SnapPos;
			Invalidate();
			break;
		case DRAWMODE_BITMAP:
			if(m_pDrawObject)
			{
				m_pDrawObject->m_P2 = m_SnapPos;
			}
			Invalidate();
			break;
		case DRAWMODE_ARROW:	//mouse move
			switch (m_DrawState)
			{
			case DRAWSTATE_WAITMOUSE_DOWN:
				if(m_pDrawObject)
					m_pDrawObject->m_P1 = m_SnapPos;
				break;
			case DRAWSTATE_MOVE:
				m_pDrawObject->m_P2 = m_SnapPos;
				break;
			}
			Invalidate();
			break;
		case DRAWMODE_ORIGIN:	///OnMouseMove
			switch (m_DrawState)
			{
				case DRAWSTATE_WAITMOUSE_DOWN:
				break;
				case DRAWSTATE_MOVE:
					if (m_pDrawObject)
						m_pDrawObject->m_P1 = m_SnapPos;
					break;
				case DRAWSTATE_PLACE:
					break;
			}
			Invalidate();
			break;
		case DRAWMODE_DIMENSION:	//Mouse Move
		{
			CCadDimension *pCD;

			switch (m_DrawState)
			{
				case DRAWSTATE_WAITMOUSE_DOWN:
					if (m_MouseState == MOUSESTATE_DOWN)
					{
						pCD = (CCadDimension *)m_pDrawObject;
						pCD->m_P2 = pCD->m_P1 = m_SnapPos;
					}
					Invalidate();
					break;
				case DRAWSTATE_MOVE:
					if (m_MouseState == MOUSESTATE_DOWN)
					{
						pCD = (CCadDimension *)m_pDrawObject;
						CPoint NewPos, UpdatedPos;
						NewPos = m_SnapPos;
						int diffx, diffy;
						diffx = abs(NewPos.x - pCD->m_P1.x);
						diffy = abs(NewPos.y - pCD->m_P1.y);
						if (diffx > diffy)
						{
							NewPos = CPoint(NewPos.x, pCD->m_P1.y);
						}
						else
						{
							NewPos = CPoint(pCD->m_P1.x, NewPos.y);
						}
						pCD->m_P2 = NewPos;
					}
					Invalidate();
					break;
			}
		}
		break;
		case DRAWMODE_ALIGN_DIM_TO_HOLE:
			switch (m_DrawState)
			{
				case DRAWSTATE_WAITMOUSE_DOWN:
					Invalidate();
					break;
			}
			break;
		case DRAWMODE_PRINTRECT:
			switch (m_DrawState)
			{
			case DRAWSTATE_WAITMOUSE_DOWN:
				break;
			case DRAWSTATE_MOVE:
				m_pDrawObject->m_P1 = m_SnapPos;
				break;
			}
			Invalidate();
			break;
	}

	CView::OnMouseMove(nFlags, point);
}

void CFrontCadView::OnSize(UINT nType, int cx, int cy)
{
	///-----------------------------------------
	/// OnSize
	///		This function is called whenever the
	/// window has been resized.
	///
	///	parameters:
	/// 	pType......Specifies type of resizing requested
	///		cx.........X size
	///		cy.........Y size
	///-----------------------------------------
	CView::OnSize(nType, cx, cy);

	// TODO: Add your message handler code here
	int HScrollMax = 0;
	m_HScrollPos = 0;
	m_HPageSize = 0;
   if (cx < (m_DocSize.cx / ZoomScaleDiv[m_ZoomLevel])) {
        HScrollMax = (m_DocSize.cx / ZoomScaleDiv[m_ZoomLevel])- 1;
        m_HPageSize = cx;
        m_HScrollPos = min (m_HScrollPos, (m_DocSize.cx/ ZoomScaleDiv[m_ZoomLevel]) - m_HPageSize - 1);
    }
	SCROLLINFO si;
    si.fMask = SIF_PAGE | SIF_RANGE | SIF_POS;
    si.nMin = 0;
    si.nMax = HScrollMax;
    si.nPos = m_HScrollPos;
    si.nPage = m_HPageSize;
    SetScrollInfo (SB_HORZ, &si, TRUE);
	int VScrollMax=0;
	m_VScrollPos = 0;
	m_VPageSize = 0;

    if (cy < (m_DocSize.cy/ ZoomScaleDiv[m_ZoomLevel]) ) {
        VScrollMax = (m_DocSize.cy/ ZoomScaleDiv[m_ZoomLevel]) - 1;
        m_VPageSize = cy;
        m_VScrollPos = min (m_VScrollPos, (m_DocSize.cy/ ZoomScaleDiv[m_ZoomLevel]) - m_VPageSize - 1);
    }
    si.fMask = SIF_PAGE | SIF_RANGE | SIF_POS;
    si.nMin = 0;
    si.nMax = VScrollMax;
    si.nPos = m_VScrollPos;
    si.nPage = m_VPageSize;

    SetScrollInfo (SB_VERT, &si, TRUE);

}

void CFrontCadView::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	///------------------------------------------------
	/// OnHScroll
	///		This is the function that is called when
	/// the user presses the horizonalscroll bar in
	/// the view.
	///
	/// parameters:
	///		nSBCode.....part of the scrollbar clicked
	///		nPos........Position of the scrollbar
	///		pScrollBar..pointer to the scrollbar object
	///------------------------------------------------
	int Delta = 0;

	switch(nSBCode)
	{
		case SB_LINELEFT:
			Delta = -1;
			break;
		case SB_PAGELEFT:
			Delta = -50;
			break;
		case SB_THUMBTRACK:
			Delta = nPos - m_HScrollPos;
			break;
		case SB_LINERIGHT:
			Delta = 1;
			break;
		case SB_PAGERIGHT:
			Delta = 50;
			break;
	}
    int ScrollPos = m_HScrollPos + Delta;
    int MaxPos = m_DocSize.cx - m_HPageSize;
    if (ScrollPos < 0)
        Delta = -m_VScrollPos;
    else if (ScrollPos > MaxPos)
        Delta = MaxPos - m_HScrollPos;

	if(Delta)
	{
        m_HScrollPos += Delta;
        SetScrollPos (SB_HORZ, m_HScrollPos, TRUE);
	}

    UpdateRulerInfo(RW_HSCROLL, CPoint(m_HScrollPos,m_VScrollPos));
	CView::OnHScroll(nSBCode, nPos, pScrollBar);
	Invalidate(FALSE);
}

void CFrontCadView::OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	///------------------------------------------------
	/// OnVScroll
	///		This is the function that is called when
	/// the user presses the VerticalScroll bar in
	/// the view.
	///
	/// parameters:
	///		nSBCode.....part of the scrollbar clicked
	///		nPos........Position of the scrollbar
	///		pScrollBar..pointer to the scrollbar object
	///------------------------------------------------
	// TODO: Add your message handler code here and/or call default
	int Delta=0;
	switch(nSBCode)
	{
		case SB_LINEUP:
			Delta = -1;
			break;
		case SB_PAGEUP:
			Delta = -50;
			break;
		case SB_THUMBTRACK:
			Delta = nPos - m_VScrollPos;
			break;
		case SB_LINEDOWN:
			Delta = 1;
			break;
		case SB_PAGEDOWN:
			Delta = 50;
			break;
		default:
			return;
	}
    int ScrollPos = m_VScrollPos + Delta;
    int MaxPos = m_DocSize.cy - m_VPageSize;
    if (ScrollPos < 0)
        Delta = -m_VScrollPos;
    else if (ScrollPos > MaxPos)
        Delta = MaxPos - m_VScrollPos;

	if(Delta)
	{
        m_VScrollPos += Delta;
        SetScrollPos (SB_VERT, m_VScrollPos, TRUE);
	}
    UpdateRulerInfo(RW_VSCROLL, CPoint(m_HScrollPos,m_VScrollPos));

	CView::OnVScroll(nSBCode, nPos, pScrollBar);
	Invalidate(FALSE);
}

void CFrontCadView::OnToolbarZoomout()
{
	///--------------------------------------------
	/// OnToolbarZoomout
	///		This function is called when the user
	/// clicks the ZoomOut command
	///--------------------------------------------
	// TODO: Add your command handler code here
	m_ZoomLevel++;
	if(m_ZoomLevel >= MAX_ZOOM)
		m_ZoomLevel = MAX_ZOOM-1;
	this->m_fZoomFactor = (float)(ZF[m_ZoomLevel] * 10.0);
	UpdateRulerInfo(RW_ZOOM, CPoint(m_HScrollPos,m_VScrollPos), SnapToScreen(m_SnapPos));
	UpdateScrollbarInfo();
	CFrontCadDoc *pDoc = GetDocument();
	pDoc->MakeDirty();
	Invalidate();
}

void CFrontCadView::OnUpdateToolbarZoomout(CCmdUI* pCmdUI)
{
	///--------------------------------------------
	/// OnUpdateToolbarZoomout
	///		This function is called to see if the
	/// Zoomout command should be enabled or not.
	///--------------------------------------------
	// TODO: Add your command update UI handler code here
	if(m_ZoomLevel >= (MAX_ZOOM-1))
		pCmdUI->Enable(FALSE);
	else
		pCmdUI->Enable(TRUE);
}

void CFrontCadView::OnToolbarZoomin()
{
	///--------------------------------------------
	/// OnToolbarZoomin
	///		This function is called when the useer
	/// user presses the ZoomIn button.
	///--------------------------------------------
	// TODO: Add your command handler code here
	m_ZoomLevel--;
	if(m_ZoomLevel < 0)
		m_ZoomLevel = 0;
	this->m_fZoomFactor = (float)(ZF[m_ZoomLevel] * 10.0);
	UpdateRulerInfo(RW_ZOOM,CPoint(m_HScrollPos,m_VScrollPos), SnapToScreen(m_SnapPos));
	UpdateScrollbarInfo();
	CFrontCadDoc *pDoc = GetDocument();
	pDoc->MakeDirty();
	Invalidate();
}

void CFrontCadView::OnUpdateToolbarZoomin(CCmdUI* pCmdUI)
{
	///--------------------------------------------
	/// OnUpdateToolbarZoomIn
	///		This function is called to see if the
	/// Zoomin command should be enabled or not.
	///--------------------------------------------
	// TODO: Add your command update UI handler code here
	if(m_ZoomLevel <= 0)
		pCmdUI->Enable(FALSE);
	else
		pCmdUI->Enable(TRUE);

}

void CFrontCadView::OnToolbarToggleruler()
{
	// TODO: Add your command handler code here
	m_bShowRulers ^= 1;
	m_pParent->ShowRulers(this->m_bShowRulers);
}

///----------------------------------------------
/// Drawing Functions
///----------------------------------------------

void CFrontCadView::OnToolbarSelect()
{
	///--------------------------------
	/// OnToolbarSelect
	///		This function put us into
	/// the Select Mode
	///--------------------------------
	// TODO: Add your command handler code here
	m_Drawmode = DRAWMODE_SELECT;
	theApp.GetMainFrame()->UpdateStatusBar("Select Object");
}


void CFrontCadView::OnButtonDimension()
{
	// TODO: Add your command handler code here
	CUtilView *pUV = GetUtilityView();
	CFrontCadApp *pA = (CFrontCadApp *)AfxGetApp();
	//------------------------------------------------
	// Line attributes
	//------------------------------------------------
	pUV->m_Edit_LineThickness.SetValue(pA->m_DimAttrib.m_Width);
	pUV->m_Static_LineColor.SetColor(pA->m_DimAttrib.m_Color);
	//------------------------------------------------
	// Text Attributes
	//-------------------------------------------------
	pUV->m_Edit_TextAngle.SetValue(pA->m_DimAttrib.m_Text.m_Angle);
	pUV->m_Edit_FontHeight.SetValue(pA->m_DimAttrib.m_Text.m_FontHeight);
	pUV->m_Edit_FontWidth.SetValue(pA->m_DimAttrib.m_Text.m_FontWidth);
	pUV->m_Check_TransparentFont.SetCheck(pA->m_DimAttrib.m_Text.m_Transparent, 0);
	pUV->m_Static_TextColor.SetColor(pA->m_DimAttrib.m_Text.m_Color);
	pUV->m_Static_BkGrndColor.SetColor(pA->m_DimAttrib.m_Text.m_BkColor);
	pUV->m_Button_Font.SetWindowText(pA->m_DimAttrib.m_Text.m_pFontName);
	pUV->m_Combo_FontWeight.SetFontWeight(pA->m_DimAttrib.m_Text.m_Weight);

	m_Drawmode = DRAWMODE_DIMENSION;
	m_DrawState = DRAWSTATE_WAITMOUSE_DOWN;

	pUV->ShowHide(m_Drawmode);
	theApp.GetMainFrame()->UpdateStatusBar("Dimension:Place First Point");
}


void CFrontCadView::OnButtonOrigin()
{
	// TODO: Add your command handler code here
	CUtilView *pUV = GetUtilityView();
	CFrontCadApp *pA = (CFrontCadApp *)AfxGetApp();
	m_Drawmode = DRAWMODE_ORIGIN;
	m_DrawState = DRAWSTATE_WAITMOUSE_DOWN;
	pUV->m_Edit_LineThickness.SetValue(pA->m_OriginAttrib.m_Width);
	pUV->m_Static_LineColor.SetColor(pA->m_OriginAttrib.m_Color);
	pUV->ShowHide(m_Drawmode);
	theApp.GetMainFrame()->UpdateStatusBar("Origin:Place Location of Point");
}

void CFrontCadView::OnToolbarRect()
{
	///----------------------------------------
	/// OnToolbarRect
	///		This function put the program into
	/// the Draw rectangle mode
	///----------------------------------------
	// TODO: Add your command handler code here
	CUtilView *pUV = GetUtilityView();
	CFrontCadApp *pA = (CFrontCadApp *)AfxGetApp();
	m_Drawmode = DRAWMODE_RECT;
	m_DrawState = DRAWSTATE_WAITMOUSE_DOWN;
	pUV->m_Edit_LineThickness.SetValue(pA->m_RectAttributes.m_Width);
	pUV->m_Static_FillColor.SetColor(pA->m_RectAttributes.m_FillColor);
	pUV->m_Static_LineColor.SetColor(pA->m_RectAttributes.m_LineColor);
	pUV->ShowHide(m_Drawmode);
	pUV->SetUpText(m_Drawmode);
	theApp.GetMainFrame()->UpdateStatusBar("Rectangle:Place First Corner");
}

void CFrontCadView::OnToolbarBitmap()
{
	///----------------------------------------
	/// OnToolbarBitmap
	///		This function is called when the
	/// Draw Bitmap button is pressed and puts
	/// the program into the Draw Bitmap mode.
	///----------------------------------------
	// TODO: Add your command handler code here
	if(this->m_pTempCadBitmap) delete m_pTempCadBitmap;
	m_Drawmode = DRAWMODE_BITMAP;
	CFileDialog dlg(TRUE);
	theApp.GetMainFrame()->UpdateStatusBar("Bitmap:Choose Bitmap File");

	dlg.DoModal();
	CString path = dlg.GetPathName();
	m_pTempCadBitmap = new CCadBitmap;
	m_pTempCadBitmap->LoadImage((char *)LPCTSTR (path));
	theApp.GetMainFrame()->UpdateStatusBar("Bitmap:Place Upper Left Corner");
}

void CFrontCadView::OnButtonArrowobj()
{
	// ------------------------------------------------------
	// OnButtonArrowobj
	// This function is called when we click on the Arrow
	// object in the tool bar.  We will put FrontCAD into
	// the mode fro drawing an arrow
	//-------------------------------------------------------
	m_Drawmode = DRAWMODE_ARROW;
	m_DrawState = DRAWSTATE_WAITMOUSE_DOWN;
	CUtilView *pUV = GetUtilityView();
	CFrontCadApp *pA = (CFrontCadApp *)AfxGetApp();
	pUV->m_Edit_X3.SetValue(pA->m_ArrowAttrib.m_Len);
	pUV->m_Edit_Y3.SetValue(pA->m_ArrowAttrib.m_LineWidth);
	pUV->m_Static_FillColor.SetColor(pA->m_ArrowAttrib.m_Color);
	pUV->ShowHide(m_Drawmode);
	pUV->SetUpText(m_Drawmode);
	theApp.GetMainFrame()->UpdateStatusBar("Arrow:Place Location of Point");
}

void CFrontCadView::OnToolbarLine()
{
	// TODO: Add your command handler code here
	CFrontCadApp *pA = (CFrontCadApp *)AfxGetApp();
	m_Drawmode = DRAWMODE_LINE;
	m_DrawState = DRAWSTATE_WAITMOUSE_DOWN;
	CUtilView *pUV = GetUtilityView();
	pUV->m_Edit_LineThickness.SetValue(pA->m_LineAttrib.m_Width);
	pUV->m_Static_LineColor.SetColor(pA->m_LineAttrib.m_Color );
	pUV->ShowHide(m_Drawmode);
	pUV->SetUpText(m_Drawmode);
	theApp.GetMainFrame()->UpdateStatusBar("Line:Place First Point");
}

void CFrontCadView::OnToolbarEllipse()
{
	// TODO: Add your command handler code here
	CFrontCadApp *pA = (CFrontCadApp *)AfxGetApp();
	m_Drawmode = DRAWMODE_ELIPSE;
	m_DrawState = DRAWSTATE_WAITMOUSE_DOWN;
	CUtilView *pUV = GetUtilityView();
	pUV->m_Edit_LineThickness.SetValue(pA->m_EllipseAttributes.m_Width);
	pUV->m_Static_FillColor.SetColor(pA->m_EllipseAttributes.m_FillColor);
	pUV->m_Static_LineColor.SetColor(pA->m_EllipseAttributes.m_LineColor);
	pUV->ShowHide(m_Drawmode);
	pUV->SetUpText(m_Drawmode);
	theApp.GetMainFrame()->UpdateStatusBar("Ellipse:Place First Corner");
}

void CFrontCadView::OnToolbarRndrect()
{
	// TODO: Add your command handler code here
	CFrontCadApp *pA = (CFrontCadApp *)AfxGetApp();
	CUtilView *pUV = GetUtilityView();
	pUV->m_Edit_X3.SetValue(pA->m_RndRectAttributes.m_X3);
	pUV->m_Edit_Y3.SetValue(pA->m_RndRectAttributes.m_Y3);
	pUV->m_Edit_LineThickness.SetValue(pA->m_RndRectAttributes.m_OutLineWidth);
	pUV->m_Static_FillColor.SetColor(pA->m_RndRectAttributes.m_FillColor);
	pUV->m_Static_LineColor.SetColor(pA->m_RndRectAttributes.m_LineColor);
	m_Drawmode = DRAWMODE_RNDRECT;
	m_DrawState = DRAWSTATE_WAITMOUSE_DOWN;
	pUV->ShowHide(m_Drawmode);
	pUV->SetUpText(m_Drawmode);
	theApp.GetMainFrame()->UpdateStatusBar("Round Rect:Place First Corner");
}

#define ID_CM_FORWARD				3100
#define ID_CM_BACKWARD				3101
#define ID_CM_DELETE				3102
#define ID_CM_MOVE					3103
#define ID_CM_RESTOREASPECTRATIO	3104
#define ID_CM_ALIGHN_DIM_TO_HOLE	3105
#define ID_CM_REPEAT_SELECTED		3106
#define ID_CM_DESELECT				3107
#define ID_CM_DESELECT_ALL			3108

void CFrontCadView::OnContextMenu(CWnd* pWnd, CPoint point)
{
	// TODO: Add your message handler code here
	CFrontCadDoc *pDoc = GetDocument();
	CMenu ConTexMenu;
	int id;
	m_MousePos = point;
	ScreenToClient(&m_MousePos);
	m_MousePos = CorrectMousePosition(m_MousePos);
	CCadObject *pObj = GetTopSelection();;
	ConTexMenu.CreatePopupMenu();
	ConTexMenu.AppendMenu(MF_STRING,ID_CM_FORWARD,"Send Obj Forward");
	ConTexMenu.AppendMenu(MF_STRING,ID_CM_BACKWARD,"Send Obj Back");
	ConTexMenu.AppendMenu(MF_STRING,ID_CM_DELETE,"Delete");
	ConTexMenu.AppendMenu(MF_SEPARATOR);
	ConTexMenu.AppendMenu(MF_STRING,ID_CM_MOVE,"Move");
	if(pObj)	//is anything slected?
	{
		ConTexMenu.EnableMenuItem(ID_CM_FORWARD,MF_ENABLED);
		ConTexMenu.EnableMenuItem(ID_CM_BACKWARD,MF_ENABLED);
		ConTexMenu.EnableMenuItem(ID_CM_DELETE,MF_ENABLED);
		ConTexMenu.EnableMenuItem(ID_CM_MOVE,MF_ENABLED);
		if(pObj->GetType() == OBJECT_TYPE_BITMAP)
		{
			ConTexMenu.AppendMenu(MF_STRING,ID_CM_RESTOREASPECTRATIO,"Restore Aspect Ratio");
			ConTexMenu.EnableMenuItem(ID_CM_RESTOREASPECTRATIO,MF_ENABLED);
		}
		if (pObj->GetType() == OBJECT_TYPE_DIMENSION) 
		{
			ConTexMenu.AppendMenuA(MF_STRING, ID_CM_ALIGHN_DIM_TO_HOLE, "Align Dim To Hole or Part");
			ConTexMenu.EnableMenuItem(ID_CM_ALIGHN_DIM_TO_HOLE, MF_ENABLED);
		}
		ConTexMenu.AppendMenuA(MF_STRING, ID_CM_REPEAT_SELECTED, "Place Array");
		ConTexMenu.EnableMenuItem(ID_CM_REPEAT_SELECTED, MF_ENABLED);
	}
	else
	{
		ConTexMenu.EnableMenuItem(ID_CM_FORWARD,MF_GRAYED);
		ConTexMenu.EnableMenuItem(ID_CM_BACKWARD,MF_GRAYED);
		ConTexMenu.EnableMenuItem(ID_CM_DELETE,MF_GRAYED);
		ConTexMenu.EnableMenuItem(ID_CM_MOVE,MF_GRAYED);
	}
	if (pObj)
	{
		ConTexMenu.AppendMenu(MF_SEPARATOR);
		if (pDoc->CheckSelected(m_SnapPos,0,0, CADDRAWUBG_CHECKSEL_FLAG_SEL) > 0 )
		{
			ConTexMenu.AppendMenu(MF_STRING, ID_CM_DESELECT, "Deselect Object");
			ConTexMenu.EnableMenuItem(ID_CM_DESELECT, MF_ENABLED);
		}
		ConTexMenu.AppendMenu(MF_STRING, ID_CM_DESELECT_ALL, "Deselect All");
		ConTexMenu.EnableMenuItem(ID_CM_DESELECT_ALL, MF_ENABLED);
	}

	id = ConTexMenu.TrackPopupMenu(TPM_RETURNCMD | TPM_LEFTALIGN | TPM_LEFTBUTTON | TPM_RIGHTBUTTON,point.x,point.y, this);
	ConTexMenu.DestroyMenu();
	switch(id)
	{
		case ID_CM_FORWARD:
			pDoc->RemoveObject(pObj);
			pDoc->AddObject(pObj);
			Invalidate();
			break;
		case ID_CM_BACKWARD:
			pDoc->RemoveObject(pObj);
			pDoc->InsertObject(pObj);
			Invalidate();
			break;
		case ID_CM_DELETE:
			RemoveObject(pObj);
			pDoc->RemoveObject(pObj);
			delete pObj;
			Invalidate();
			break;
		case ID_CM_MOVE:
			m_Drawmode = DRAWMODE_MOVE;
			m_DrawState = DRAWSTATE_GETREFEREMCE;
			break;
		case ID_CM_RESTOREASPECTRATIO:
			((CCadBitmap *)pObj)->RestoreAspectRatio();
			break;
		case ID_CM_ALIGHN_DIM_TO_HOLE:
			m_Drawmode = DRAWMODE_ALIGN_DIM_TO_HOLE;
			m_DrawState = DRAWSTATE_WAITMOUSE_DOWN;
			theApp.GetMainFrame()->UpdateStatusBar("Align Dimension:Select Object To Align to");
			break;
		case ID_CM_REPEAT_SELECTED:
			{
				CRepeatDlg dlg;
				int dlgID;
				dlg.m_Xinc = 0;
				dlg.m_Yinc = 0;
				dlg.m_RepeatCountX = 0;
				dlg.m_RepeatCountY = 0;
				dlgID = dlg.DoModal();
				if (IDOK == dlgID)
				{
					int i, j;
					int flag = 1;
					CCadObject *pNewObj;
					for (j = 0; j < dlg.m_RepeatCountY; ++j)
					{
						for (i = 0; i < dlg.m_RepeatCountX; ++i)
						{
							if (flag)	//we need to skip creating the first object
							{
								flag = 0;
								i++;
							}
							pNewObj = pObj->CopyObject();
							pNewObj->m_P1 = pObj->m_P1 + CSize(dlg.m_Xinc*i, dlg.m_Yinc*j);
							pDoc->AddObject(pNewObj);
						}
					}
				}
			}
			break;
		case ID_CM_DESELECT:
			{
				int c;
				CCadObject **ppObList = new CCadObject*[8];
				c = pDoc->CheckSelected(m_SnapPos, ppObList, 8, CADDRAWUBG_CHECKSEL_FLAG_SEL);
				if (c > 1)
				{
					CMenu SelMenu;
					SelMenu.CreatePopupMenu();
					int i;
					char *s = new char[256];
					for (i = 0; i < c; ++i)
					{
						sprintf_s(s, 256, "%s:(%d,%d)", ppObList[i]->GetTypeString(), ppObList[i]->m_P1.x, ppObList[i]->m_P1.y);
						SelMenu.AppendMenu(MF_STRING, ID_OBJECTSEL + i, s);
						SelMenu.EnableMenuItem(ID_OBJECTSEL + i, MF_ENABLED);
					}
					int id = SelMenu.TrackPopupMenu(TPM_RETURNCMD | TPM_LEFTALIGN | TPM_LEFTBUTTON | TPM_RIGHTBUTTON, point.x, point.y, this);
					if (id > 0)
					{
						CCadObject *pObj = ppObList[id - ID_OBJECTSEL];
						pObj->SetSelected(0);
						RemoveObject(pObj);
					}
					delete[] s;
				}
				else if(c == 1)
				{
					ppObList[0]->SetSelected(0);
					RemoveObject(ppObList[0]);
				}
				Invalidate();
			}
			break;
		case ID_CM_DESELECT_ALL:
			ClearSelList();
			break;
	}
}

void CFrontCadView::UpdateRulerInfo(int nMessage, CPoint ScrollPos, CPoint Pos)
{
    if (!m_pParent)
        return;

 //   if (m_fZoomFactor < ZOOM_NORMAL) {
 //       m_bShowRulers = FALSE;
 //       m_pParent->ShowRulers(FALSE);
 //   }

    stRULER_INFO pRulerInfo;
    pRulerInfo.uMessage    = nMessage;
    pRulerInfo.ScrollPos   = ScrollPos;
    pRulerInfo.Pos         = Pos;
    pRulerInfo.DocSize     = m_DocSize;
    pRulerInfo.fZoomFactor = m_fZoomFactor;

    m_pParent->UpdateRulersInfo(pRulerInfo);

}

void CFrontCadView::OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint)
{
	// TODO: Add your specialized code here and/or call the base class

//	SetScrollSizes(MM_TEXT,
//                   CSize(int(m_DocSize.cx*m_fZoomFactor),
//                         int(m_DocSize.cy*m_fZoomFactor)));
    UpdateRulerInfo(RW_POSITION, CPoint(m_HScrollPos,m_VScrollPos));
    Invalidate();

}

CUtilView * CFrontCadView::GetUtilityView()
{
	return this->m_pParent->GetUtilityView();
}

void CFrontCadView::OnToolbarGridonoff()
{
	// TODO: Add your command handler code here
	m_GridOn ^= 1;
	Invalidate();
}

void CFrontCadView::OnToolbarGridsettings()
{
	// TODO: Add your command handler code here
	CDialogGridSettings dlg;
	dlg.m_Grid = this->m_Grid;
	dlg.m_Snap = this->m_SnapGrid;
	dlg.m_Color = this->m_GridColor;
	if(dlg.DoModal() == IDOK)
	{
		m_Grid = dlg.m_Grid;
		m_SnapGrid = dlg.m_Snap;
		m_GridColor = dlg.m_Color;
	}
}

void CFrontCadView::OnToolbarLibpart()
{
	// TODO: Add your command handler code here
	m_Drawmode = DRAWMODE_LIBPART;
	theApp.GetMainFrame()->UpdateStatusBar("Library Part:Place");
}

void CFrontCadView::OnToolbarPolygon()
{
	// TODO: Add your command handler code here
	CFrontCadApp *pA = (CFrontCadApp *)AfxGetApp();
	this->m_Drawmode = DRAWMODE_POLYGON;
	m_DrawState = DRAWSTATE_WAITMOUSE_DOWN;
	CUtilView *pUV = GetUtilityView();
	pUV->m_Edit_LineThickness.SetValue(pA->m_PolyAttributes.m_Width);
	pUV->m_Static_LineColor.SetColor(pA->m_PolyAttributes.m_LineColor);
	pUV->ShowHide(m_Drawmode);
	pUV->SetUpText(m_Drawmode);
	theApp.GetMainFrame()->UpdateStatusBar("Polygon:Place First Point");
}

void CFrontCadView::OnToolbarScalewiz()
{
	// TODO: Add your command handler code here
	CFrontCadApp *pA = (CFrontCadApp *)AfxGetApp();
	CScaleWizDialog dlg;
	int Id;

	dlg.m_atrb.m_ArcEnd = pA->m_SCALEWIZattributes.m_ArcEnd;
	dlg.m_atrb.m_ArcLineWidth = pA->m_SCALEWIZattributes.m_ArcLineWidth;
	dlg.m_atrb.m_ArcRadius = pA->m_SCALEWIZattributes.m_ArcRadius;
	dlg.m_atrb.m_ArcStart = pA->m_SCALEWIZattributes.m_ArcStart;
	dlg.m_atrb.m_ArcColor = pA->m_SCALEWIZattributes.m_ArcColor;
	dlg.m_atrb.m_DivisionLineLegnth = pA->m_SCALEWIZattributes.m_DivisionLineLegnth;
	dlg.m_atrb.m_DivisionLineWidth = pA->m_SCALEWIZattributes.m_DivisionLineWidth;
	dlg.m_atrb.m_Divisions = pA->m_SCALEWIZattributes.m_Divisions;
	dlg.m_atrb.m_DivLineColor = pA->m_SCALEWIZattributes.m_DivLineColor;
	dlg.m_atrb.m_SubdivisionLengnth = pA->m_SCALEWIZattributes.m_SubdivisionLengnth;
	dlg.m_atrb.m_SubDivisionLineWidth = pA->m_SCALEWIZattributes.m_SubDivisionLineWidth;
	dlg.m_atrb.m_SubDivisions = pA->m_SCALEWIZattributes.m_SubDivisions;
	dlg.m_atrb.m_SubDivColor = pA->m_SCALEWIZattributes.m_SubDivColor;
	dlg.m_atrb.m_FlatDist = pA->m_SCALEWIZattributes.m_FlatDist;
	dlg.m_atrb.m_HoleSize = pA->m_SCALEWIZattributes.m_HoleSize;
	dlg.m_atrb.m_HoleType = pA->m_SCALEWIZattributes.m_HoleType;
	dlg.m_atrb.m_FontSize = pA->m_SCALEWIZattributes.m_FontSize;
	dlg.m_atrb.m_FontWeight = pA->m_SCALEWIZattributes.m_FontWeight;
	dlg.m_atrb.m_DistToTick = pA->m_SCALEWIZattributes.m_DistToTick;
	for(int i=0;i< pA->m_SCALEWIZattributes.m_Divisions+1;++i)
		strcpy_s(dlg.m_atrb.m_pLabels[i], SCALE_LABELS_STRING_SIZE,pA->m_SCALEWIZattributes.m_pLabels[i]);
	strcpy_s(dlg.m_atrb.m_pFont, LF_FACESIZE,pA->m_SCALEWIZattributes.m_pFont);
	dlg.m_atrb.m_BkColor = pA->m_SCALEWIZattributes.m_BkColor;
	dlg.m_atrb.m_TextColor = pA->m_SCALEWIZattributes.m_TextColor;
	Id = dlg.DoModal();
	if(IDOK == Id)
	{
		pA->m_SCALEWIZattributes.m_ArcEnd = dlg.m_atrb.m_ArcEnd;
		pA->m_SCALEWIZattributes.m_ArcLineWidth = dlg.m_atrb.m_ArcLineWidth;
		pA->m_SCALEWIZattributes.m_ArcRadius = dlg.m_atrb.m_ArcRadius;
		pA->m_SCALEWIZattributes.m_ArcStart = dlg.m_atrb.m_ArcStart;
		pA->m_SCALEWIZattributes.m_ArcColor = dlg.m_atrb.m_ArcColor;
		pA->m_SCALEWIZattributes.m_DivisionLineLegnth = dlg.m_atrb.m_DivisionLineLegnth;
		pA->m_SCALEWIZattributes.m_DivisionLineWidth = dlg.m_atrb.m_DivisionLineWidth;
		pA->m_SCALEWIZattributes.m_Divisions = dlg.m_atrb.m_Divisions;
		pA->m_SCALEWIZattributes.m_DivLineColor = dlg.m_atrb.m_DivLineColor;
		pA->m_SCALEWIZattributes.m_SubdivisionLengnth = dlg.m_atrb.m_SubdivisionLengnth;
		pA->m_SCALEWIZattributes.m_SubDivisionLineWidth = dlg.m_atrb.m_SubDivisionLineWidth;
		pA->m_SCALEWIZattributes.m_SubDivisions = dlg.m_atrb.m_SubDivisions;
		pA->m_SCALEWIZattributes.m_SubDivColor = dlg.m_atrb.m_SubDivColor;
		pA->m_SCALEWIZattributes.m_FlatDist = dlg.m_atrb.m_FlatDist;
		pA->m_SCALEWIZattributes.m_HoleSize = dlg.m_atrb.m_HoleSize;
		pA->m_SCALEWIZattributes.m_HoleType = dlg.m_atrb.m_HoleType;
		pA->m_SCALEWIZattributes.m_DistToTick = dlg.m_atrb.m_DistToTick;
		for(int i=0;i< pA->m_SCALEWIZattributes.m_Divisions+1;++i)
			strcpy_s(pA->m_SCALEWIZattributes.m_pLabels[i], SCALE_LABELS_SIZE,dlg.m_atrb.m_pLabels[i]);
		pA->m_SCALEWIZattributes.m_FontSize = dlg.m_atrb.m_FontSize;
		pA->m_SCALEWIZattributes.m_BkColor = dlg.m_atrb.m_BkColor;
		pA->m_SCALEWIZattributes.m_TextColor = dlg.m_atrb.m_TextColor;
	}
}

void CFrontCadView::DrawCrosshairs(CDC *pDC,CRect *pRect,CPoint pos)
{
	CPen pen,*pOldPen;
	pen.CreatePen(PS_SOLID,1,RGB(0,0,0));
	pOldPen = pDC->SelectObject(&pen);
	int w,h;
	pos -= CPoint(m_HScrollPos,m_VScrollPos);
	w = pRect->Width();
	h = pRect->Height();
	pDC->MoveTo(0,pos.y);
	pDC->LineTo(w,pos.y);
	pDC->MoveTo(pos.x,0);
	pDC->LineTo(pos.x,h);
	pDC->SelectObject(pOldPen);
}

void CFrontCadView::OnToolbarPolywithfill()
{
	// TODO: Add your command handler code here
	CFrontCadApp *pA = (CFrontCadApp *)AfxGetApp();
	m_Drawmode = DRAWMODE_POLYFILL;
	m_DrawState = DRAWSTATE_WAITMOUSE_DOWN;
	CUtilView *pUV = GetUtilityView();
	pUV->m_Edit_LineThickness.SetValue(pA->m_PolyFillAttributes.m_Width);
	pUV->m_Static_LineColor.SetColor(pA->m_PolyFillAttributes.m_LineColor);
	pUV->m_Static_FillColor.SetColor(pA->m_PolyFillAttributes.m_FillColor);
	pUV->ShowHide(m_Drawmode);
	pUV->SetUpText(m_Drawmode);
	theApp.GetMainFrame()->UpdateStatusBar("Filled Polygon:Place First Point");

}


void CFrontCadView::OnToolbarArcCenter()
{
	// TODO: Add your command handler code here
	CFrontCadApp *pA = (CFrontCadApp *)AfxGetApp();
	m_Drawmode = DRAWMODE_ARC_CENTER;
	m_DrawState = DRAWSTATE_WAITMOUSE_DOWN;
	CUtilView *pUV = GetUtilityView();
	pUV->m_Edit_LineThickness.SetValue(pA->m_ArcAttributes.m_Width);
	pUV->m_Static_LineColor.SetColor(pA->m_ArcAttributes.m_LineColor);
	pUV->ShowHide(m_Drawmode);
	pUV->SetUpText(m_Drawmode);
	theApp.GetMainFrame()->UpdateStatusBar("Arc:Place Center Point");

}

void CFrontCadView::OnToolbarArc()
{
	// TODO: Add your command handler code here
	CFrontCadApp *pA = (CFrontCadApp *)AfxGetApp();
	m_Drawmode = DRAWMODE_ARC;
	m_DrawState = DRAWSTATE_WAITMOUSE_DOWN;
	CUtilView *pUV = GetUtilityView();
	pUV->m_Edit_LineThickness.SetValue(pA->m_ArcAttributes.m_Width);
	pUV->m_Static_LineColor.SetColor(pA->m_ArcAttributes.m_LineColor);
	pUV->ShowHide(m_Drawmode);
	pUV->SetUpText(m_Drawmode);
	theApp.GetMainFrame()->UpdateStatusBar("Arc:Place First Corner");
}

void CFrontCadView::OnToolbarHolernd()
{
	// TODO: Add your command handler code here
	CFrontCadApp *pA = (CFrontCadApp *)AfxGetApp();
	CUtilView *pUV = GetUtilityView();
	m_Drawmode = DRAWMODE_HOLE_ROUND;
	m_DrawState = DRAWSTATE_WAITMOUSE_DOWN;
	pUV->m_Static_LineColor.SetColor(pA->m_HoleRoundAttributes.m_LineColor);
	pUV->m_Edit_LineThickness.SetValue(pA->m_HoleRoundAttributes.m_Width);
	pUV->m_Edit_HoleRadius.SetValue(pA->m_HoleRoundAttributes.m_Radius);
	pUV->ShowHide(m_Drawmode);
	pUV->SetUpText(m_Drawmode);
	theApp.GetMainFrame()->UpdateStatusBar("Round Hole:Place Center Point");
}

void CFrontCadView::OnToolbarRecthole()
{
	// TODO: Add your command handler code here
	CFrontCadApp *pA = (CFrontCadApp *)AfxGetApp();
	CUtilView *pUV = GetUtilityView();
	m_Drawmode = DRAWMODE_HOLE_RECT;
	m_DrawState = DRAWSTATE_WAITMOUSE_DOWN;
	pUV->m_Edit_X2.SetValue(pA->m_RectHoleAttributes.m_H);
	pUV->m_Edit_Y2.SetValue(pA->m_RectHoleAttributes.m_W);
	pUV->m_Edit_LineThickness.SetValue(pA->m_RectHoleAttributes.m_Width);
	pUV->m_Static_LineColor.SetColor(pA->m_RectHoleAttributes.m_LineColor);
	pUV->ShowHide(m_Drawmode);
	pUV->SetUpText(m_Drawmode);
	theApp.GetMainFrame()->UpdateStatusBar("Rectangular Hole:Place Center Point");
}

void CFrontCadView::OnToolbarRnd1flat()
{
	// TODO: Add your command handler code here

	CFrontCadApp *pA = (CFrontCadApp *)AfxGetApp();
	CUtilView *pUV = GetUtilityView();
	m_Drawmode = DRAWMODE_HOLE_RND1F;
	m_DrawState = DRAWSTATE_WAITMOUSE_DOWN;
	pUV->m_Static_LineColor.SetColor(pA->m_HoleRnd1FlatAttributes.m_LineColor);
	pUV->m_Edit_LineThickness.SetValue(pA->m_HoleRnd1FlatAttributes.m_Width);
	pUV->m_Edit_HoleRadius.SetValue(pA->m_HoleRnd1FlatAttributes.m_Radius);
	pUV->m_Edit_FlatToCenterDist.SetValue(pA->m_HoleRnd1FlatAttributes.m_FlatDist);
	pUV->ShowHide(m_Drawmode);
	pUV->SetUpText(m_Drawmode);
	theApp.GetMainFrame()->UpdateStatusBar("Round Hole one Flat Side:Place Center Point");
}

void CFrontCadView::OnToolbarTwoflats()
{
	// TODO: Add your command handler code here
	CFrontCadApp *pA = (CFrontCadApp *)AfxGetApp();
	CUtilView *pUV = GetUtilityView();
	m_Drawmode = DRAWMODE_HOLE_RND2F;
	m_DrawState = DRAWSTATE_WAITMOUSE_DOWN;
	pUV->m_Static_LineColor.SetColor(pA->m_HoleRnd2FlatAttributes.m_LineColor);
	pUV->m_Edit_LineThickness.SetValue(pA->m_HoleRnd2FlatAttributes.m_Width);
	pUV->m_Edit_HoleRadius.SetValue(pA->m_HoleRnd2FlatAttributes.m_Radius);
	pUV->m_Edit_FlatToCenterDist.SetValue(pA->m_HoleRnd2FlatAttributes.m_FlatDist);
	pUV->ShowHide(m_Drawmode);
	pUV->SetUpText(m_Drawmode);
	theApp.GetMainFrame()->UpdateStatusBar("Round Hole two flat sides:Place Center Point");
}

void CFrontCadView::OnToolbarDrawtext()
{
	// TODO: Add your command handler code here
	CFrontCadApp *pA = (CFrontCadApp *)AfxGetApp();
	CUtilView *pUV = GetUtilityView();
	m_Drawmode = DRAWMODE_TEXT;
	pUV->m_Edit_TextAngle.SetValue(pA->m_TextAttributes.m_Angle);
	pUV->m_Edit_FontHeight.SetValue(pA->m_TextAttributes.m_FontHeight);
	pUV->m_Edit_FontWidth.SetValue(pA->m_TextAttributes.m_FontWidth);
	pUV->m_Check_TransparentFont.SetCheck(pA->m_TextAttributes.m_Transparent,0);
	pUV->m_Static_TextColor.SetColor(pA->m_TextAttributes.m_Color);
	pUV->m_Static_BkGrndColor.SetColor(pA->m_TextAttributes.m_BkColor);
	pUV->m_Button_Font.SetWindowText(pA->m_TextAttributes.m_pFontName);
	pUV->m_Combo_FontWeight.SetFontWeight(pA->m_TextAttributes.m_Weight);
	pUV->ShowHide(m_Drawmode);
	pUV->SetUpText(m_Drawmode);
	theApp.GetMainFrame()->UpdateStatusBar("Text:Place Corner Point");
}

LRESULT CFrontCadView::OnUtilMessage(WPARAM wP, LPARAM lP)
{
	///----------------------------------------------
	/// OnUtilMessage
	///		This function recieves a message from the
	///	controls in the Utility pane when the user updates
	//	one of the controls.  This is used to edit
	/// an object in the drawing when that object
	/// is selected
	///
	///	parameters:
	///		wP.....holds the sub message
	///				this identifies the object
	///				that the message came from
	///		lP.....resaerved
	///----------------------------------------------
	CFrontCadDoc *pDoc;

	switch(wP)
	{
		case UTIL_DEFAULT:
			MessageBox("Unknown Message","Debug");
			break;
		case UTIL_X1:
		case UTIL_Y1:
		case UTIL_X2:
		case UTIL_Y2:
		case UTIL_X3:
		case UTIL_Y3:
		case UTIL_TEXT:
		case UTIL_FLATDIST:
		case UTIL_HOLERADIUS:
			if (GetTopSelection())
			{
				GetTopSelection()->MakeDirty();
				ChangeObject(this->GetUtilityView(), GetTopSelection());
				Invalidate();
			}
			break;
		case UTIL_LINEWIDTH:
		case UTIL_LINECOLOR:
		case UTIL_FILLCOLOR:
		case UTIL_FONTHEIGHT:
		case UTIL_FONTWIDTH:
		case UTIL_TEXTANGLE:
		case UTIL_TEXTCOLOR:
		case UTIL_FONT:
		case UTIL_TEXTBKCOLOR:
		case UTIL_FONTWEIGHT:
		case UTIL_PRINTRECT:
			if(GetTopSelection())
			{
				GetTopSelection()->MakeDirty();
				ChangeObject(GetUtilityView(),GetTopSelection());
				Invalidate();
			}
			break;
		case UTIL_BACKGROUND:
			pDoc = GetDocument();
			pDoc->SetBkColor(lP);
			Invalidate();
			break;
		case UTIL_TEXTTRANSPARENT:
			if(GetTopSelection())
			{
				this->GetTopSelection()->MakeDirty();
				ChangeObject(GetUtilityView(),GetTopSelection());
				Invalidate();
			}
			break;
	}
	((CMainFrame *)GetParent())->FocusOnMainView();
	return 0;
}

void CFrontCadView::FillInUtilView(CUtilView *pUV, CCadObject *pSel)
{
	switch(pSel->GetType())
	{
		case OBJECT_TYPE_NONE:
			break;
		case OBJECT_TYPE_LINE:
			pUV->m_Edit_X1.SetValue(((CCadLine *)pSel)->m_P1.x);
			pUV->m_Edit_Y1.SetValue(((CCadLine *)pSel)->m_P1.y);
			pUV->m_Edit_X2.SetValue(((CCadLine *)pSel)->m_P2.x);
			pUV->m_Edit_X2.SetValue(((CCadLine *)pSel)->m_P2.x);
			pUV->m_Edit_Y2.SetValue(((CCadLine *)pSel)->m_P2.y);
			pUV->m_Edit_LineThickness.SetValue(((CCadLine *)pSel)->GetOutLineWidth());
			pUV->m_Static_LineColor.SetColor( ((CCadLine *)pSel)->GetLineColor());
			break;
		case OBJECT_TYPE_RECT:
			pUV->m_Edit_X1.SetValue(((CCadRect *)pSel)->m_P1.x);
			pUV->m_Edit_Y1.SetValue(((CCadRect *)pSel)->m_P1.y);
			pUV->m_Edit_X2.SetValue(((CCadRect *)pSel)->m_P2.x);
			pUV->m_Edit_X2.SetValue(((CCadRect *)pSel)->m_P2.x);
			pUV->m_Edit_Y2.SetValue(((CCadRect *)pSel)->m_P2.y);
			pUV->m_Edit_LineThickness.SetValue(((CCadRect *)pSel)->GetOutLineWidth());
			pUV->m_Static_LineColor.SetColor( ((CCadRect *)pSel)->GetLineColor() );
			pUV->m_Static_FillColor.SetColor( ((CCadRect *)pSel)->GetFillColor() );
			break;
		case OBJECT_TYPE_ELLIPSE:
			pUV->m_Edit_X1.SetValue(((CCadElipse *)pSel)->m_P1.x);
			pUV->m_Edit_Y1.SetValue(((CCadElipse *)pSel)->m_P1.y);
			pUV->m_Edit_X2.SetValue(((CCadElipse *)pSel)->m_P2.x);
			pUV->m_Edit_X2.SetValue(((CCadElipse *)pSel)->m_P2.x);
			pUV->m_Edit_Y2.SetValue(((CCadElipse *)pSel)->m_P2.y);
			pUV->m_Edit_LineThickness.SetValue(((CCadElipse *)pSel)->GetOutLineWidth());
			pUV->m_Static_LineColor.SetColor( ((CCadElipse *)pSel)->GetLineColor() );
			pUV->m_Static_FillColor.SetColor( ((CCadElipse *)pSel)->GetFillColor() );
			break;
		case OBJECT_TYPE_RNDRECT:
			pUV->m_Edit_X1.SetValue(((CCadRoundRect *)pSel)->m_P1.x);
			pUV->m_Edit_Y1.SetValue(((CCadRoundRect *)pSel)->m_P1.y);
			pUV->m_Edit_X2.SetValue(((CCadRoundRect *)pSel)->m_P2.x);
			pUV->m_Edit_Y2.SetValue(((CCadRoundRect *)pSel)->m_P2.y);
			pUV->m_Edit_X3.SetValue(((CCadRoundRect *)pSel)->GetArc().x);
			pUV->m_Edit_Y3.SetValue(((CCadRoundRect *)pSel)->GetArc().y);
			pUV->m_Edit_LineThickness.SetValue(((CCadRoundRect *)pSel)->GetOutLineWidth());
			pUV->m_Static_LineColor.SetColor( ((CCadRoundRect *)pSel)->GetLineColor() );
			pUV->m_Static_FillColor.SetColor( ((CCadRoundRect *)pSel)->GetFillColor() );
			break;
		case OBJECT_TYPE_POLY:
			pUV->m_Edit_LineThickness.SetValue(((CCadPolygon *)pSel)->GetWidth());
			pUV->m_Static_LineColor.SetColor( ((CCadPolygon *)pSel)->GetLineColor() );
			break;
		case OBJECT_TYPE_POLYFILL:
			pUV->m_Edit_LineThickness.SetValue(((CCadPolygonFill *)pSel)->GetWidth());
			pUV->m_Static_LineColor.SetColor( ((CCadPolygonFill *)pSel)->GetLineColor() );
			pUV->m_Static_FillColor.SetColor( ((CCadPolygonFill *)pSel)->GetFillColor() );
			break;
		case OBJECT_TYPE_LIBCOMP:
			break;
		case OBJECT_TYPE_HOLEROUND:
			pUV->m_Edit_X1.SetValue(((CCadHoleRound *)pSel)->m_P1.x);
			pUV->m_Edit_Y1.SetValue(((CCadHoleRound *)pSel)->m_P1.y);
			pUV->m_Edit_LineThickness.SetValue(((CCadHoleRound *)pSel)->GetWidth());
			pUV->m_Static_LineColor.SetColor( ((CCadHoleRound *)pSel)->GetLineColor() );
			pUV->m_Edit_HoleRadius.SetValue(((CCadHoleRound *)pSel)->GetRadius());
			break;
		case OBJECT_TYPE_HOLERECT:
			pUV->m_Edit_X1.SetValue(((CCadRectHole *)pSel)->m_P1.x);
			pUV->m_Edit_Y1.SetValue(((CCadRectHole *)pSel)->m_P1.y);
			pUV->m_Edit_X2.SetValue(((CCadRectHole *)pSel)->GetHieght());
			pUV->m_Edit_Y2.SetValue(((CCadRectHole *)pSel)->GetWidth());
			pUV->m_Edit_LineThickness.SetValue(((CCadRectHole *)pSel)->GetOutLineWidth());
			pUV->m_Static_LineColor.SetColor( ((CCadRectHole *)pSel)->GetLineColor() );
			break;
		case OBJECT_TYPE_HOLERND1FLAT:
			pUV->m_Edit_X1.SetValue(((CCadHoleRnd1Flat *)pSel)->m_P1.x);
			pUV->m_Edit_Y1.SetValue(((CCadHoleRnd1Flat *)pSel)->m_P1.y);
			pUV->m_Edit_LineThickness.SetValue(((CCadHoleRnd1Flat *)pSel)->GetWidth());
			pUV->m_Static_LineColor.SetColor( ((CCadHoleRnd1Flat *)pSel)->GetLineColor() );
			pUV->m_Edit_HoleRadius.SetValue(((CCadHoleRnd1Flat *)pSel)->GetRadius());
			pUV->m_Edit_FlatToCenterDist.SetValue(((CCadHoleRnd1Flat *)pSel)->GetFlatDist());
			break;
		case OBJECT_TYPE_HOLERND2FLAT:
			pUV->m_Edit_X1.SetValue(((CCadHoleRnd2Flat *)pSel)->m_P1.x);
			pUV->m_Edit_Y1.SetValue(((CCadHoleRnd2Flat *)pSel)->m_P1.y);
			pUV->m_Edit_LineThickness.SetValue(((CCadHoleRnd2Flat *)pSel)->GetWidth());
			pUV->m_Static_LineColor.SetColor( ((CCadHoleRnd2Flat *)pSel)->GetLineColor() );
			pUV->m_Edit_HoleRadius.SetValue(((CCadHoleRnd2Flat *)pSel)->GetRadius());
			pUV->m_Edit_FlatToCenterDist.SetValue(((CCadHoleRnd2Flat *)pSel)->GetFlatDist());
			break;
		case OBJECT_TYPE_TEXT:
			pUV->m_Edit_X1.SetValue(((CCadText *)pSel)->m_P1.x);
			pUV->m_Edit_Y1.SetValue(((CCadText *)pSel)->m_P1.y);
			pUV->m_Edit_Text.SetWindowText(((CCadText *)pSel)->GetText());
			pUV->m_Button_Font.SetWindowText(((CCadText *)pSel)->GetFontName() );
			pUV->m_Static_BkGrndColor.SetColor(((CCadText *)pSel)->GetBkColor());
			pUV->m_Static_TextColor.SetColor(((CCadText *)pSel)->GetColor());
			pUV->m_Edit_FontHeight.SetValue(((CCadText *)pSel)->GetFontHeight());
			pUV->m_Edit_FontWidth.SetValue(((CCadText *)pSel)->GetFontWidth());
			pUV->m_Edit_TextAngle.SetValue(((CCadText *)pSel)->GetAngle());
			pUV->m_Check_TransparentFont.SetCheck(((CCadText *)pSel)->GetTransparent(),0);
			pUV->m_Combo_FontWeight.SetFontWeight(((CCadText *)pSel)->GetWeight());
			break;
		case OBJECT_TYPE_ARC:
			pUV->m_Edit_X1.SetValue(((CCadArc *)pSel)->m_P1.x);
			pUV->m_Edit_X2.SetValue(((CCadArc *)pSel)->m_P2.x);
			pUV->m_Edit_Y2.SetValue(((CCadArc *)pSel)->m_P2.y);
			pUV->m_Edit_Y1.SetValue(((CCadArc *)pSel)->m_P1.y);
			pUV->m_Edit_X3.SetValue(((CCadArc *)pSel)->GetStartAngle());
			pUV->m_Edit_Y3.SetValue(((CCadArc *)pSel)->GetEndAngle());
			pUV->m_Edit_LineThickness.SetValue(((CCadArc *)pSel)->GetWidth());
			pUV->m_Static_LineColor.SetColor(((CCadArc *)pSel)->GetLineColor());
			break;
		case OBJECT_TYPE_ARCCENTERED:
			pUV->m_Edit_X1.SetValue(((CCadArcCentered *)pSel)->m_P1.x);
			pUV->m_Edit_X2.SetValue(((CCadArcCentered *)pSel)->m_P2.x);
			pUV->m_Edit_Y2.SetValue(((CCadArcCentered *)pSel)->m_P2.y);
			pUV->m_Edit_Y1.SetValue(((CCadArcCentered *)pSel)->m_P1.y);
			pUV->m_Edit_X3.SetValue(((CCadArcCentered *)pSel)->GetStartAngle());
			pUV->m_Edit_Y3.SetValue(((CCadArcCentered *)pSel)->GetEndAngle());
			pUV->m_Edit_LineThickness.SetValue(((CCadArcCentered *)pSel)->GetWidth());
			pUV->m_Static_LineColor.SetColor(((CCadArcCentered *)pSel)->GetLineColor());
			break;
		case OBJECT_TYPE_ARROW:
			pUV->m_Edit_X1.SetValue(((CCadArrow *)pSel)->m_P1.x);
			pUV->m_Edit_X2.SetValue(((CCadArrow *)pSel)->m_P2.x);
			pUV->m_Edit_Y2.SetValue(((CCadArrow *)pSel)->m_P2.y);
			pUV->m_Edit_Y1.SetValue(((CCadArrow *)pSel)->m_P1.y);
			pUV->m_Edit_X3.SetValue(((CCadArrow *)pSel)->GetLegnth());
			pUV->m_Edit_Y3.SetValue(((CCadArrow *)pSel)->GetWidth());
			pUV->m_Static_FillColor.SetColor(((CCadArrow *)pSel)->GetColor() );
			break;
		case OBJECT_TYPE_ORIGIN:
			break;
		case OBJECT_TYPE_DIMENSION:
			pUV->m_Edit_LineThickness.SetValue(((CCadDimension *)pSel)->GetWidth());
			pUV->m_Static_LineColor.SetColor(((CCadDimension *)pSel)->GetColor());

			pUV->m_Button_Font.SetWindowText( ( (CCadDimension *)pSel)->GetText()->GetFontName() );
			pUV->m_Static_BkGrndColor.SetColor(((CCadDimension *)pSel)->GetText()->GetBkColor());
			pUV->m_Static_TextColor.SetColor(((CCadDimension *)pSel)->GetText()->GetColor());
			pUV->m_Edit_FontHeight.SetValue(((CCadDimension *)pSel)->GetText()->GetFontHeight());
			pUV->m_Edit_FontWidth.SetValue(((CCadDimension *)pSel)->GetText()->GetFontWidth());
			pUV->m_Combo_FontWeight.SetFontWeight(((CCadDimension *)pSel)->GetText()->GetWeight());
			break;
	}
}


void CFrontCadView::ClearSelList()
{
	CCadObject *pO = GetTopSelection();
	CCadObject *next;
	while(pO)
	{
		next = pO->m_pSelNext;
		pO->SetSelected(0);
		RemoveObject(pO);
		pO = next;
	}
	m_SelectionCount = 0;

}

CCadObject *CFrontCadView::PopSelList(void)
{
	CCadObject *rObj;

	rObj = m_pSelObjList;
	if (rObj)
	{
		m_SelectionCount--;
		m_pSelObjList = rObj->m_pSelNext;
		rObj->m_pSelNext = 0;
	}
	if (m_pSelObjList == NULL) m_pSelObjEnd = 0;
	return rObj;
}

void CFrontCadView::AddToSelList(CCadObject *pO)
{
	///----------------------------------------
	///	AddToSelList
	///		This function adds an object to the
	/// selection list.
	///
	///	parameter:
	///		pO.....Object to add to list
	///---------------------------------------
	if(this->m_pSelObjList)
	{
		pO->m_pSelNext = m_pSelObjList;
		m_pSelObjList = pO;
	}
	else
	{
		m_pSelObjList = pO;
		m_pSelObjEnd = pO;
	}
	m_SelectionCount++;

}

CCadObject * CFrontCadView::GetTopSelection()
{
	///-------------------------------------------
	///	GetTopSelection
	///		Returns the object that is at the
	/// top of the Selected Objects List
	///-------------------------------------------
	return m_pSelObjList;
}

void CFrontCadView::RemoveUnselected()
{
	///-------------------------------------
	/// RemoveUnselected
	///remove all unselected objects
	///--------------------------------------
	CCadObject *pO,*pOr,*pPrev=0;

	pO = m_pSelObjList;
	while(pO)
	{
		if(!pO->m_Selected)
		{
			m_SelectionCount--;

			pOr = pO;
			if(!pPrev)
			{
				m_pSelObjList = pO->m_pSelNext;
				pO = m_pSelObjList;
				pOr->m_pSelNext = 0;
			}
			else
			{
				//pPrev will stay the same
				pO = pO->m_pSelNext;
				pPrev->m_pSelNext = pOr->m_pSelNext;
				pOr->m_pSelNext = 0;
			}
		}
		else
		{
			pPrev = pO;
			pO = pO->m_pSelNext;
		}

	}
	if(m_pSelObjList == 0) m_pSelObjEnd = 0;
}

void CFrontCadView::RemoveObject(CCadObject *pO)
{
	///----------------------------------------------
	///	RemoveObject
	/// This fuction removes the specified object
	/// from the selected list
	///		parameters:
	///			pO.....pointer to object to remove
	///----------------------------------------------
	CCadObject *pL,*pPrev=0;
	int loop=1;

	pL = m_pSelObjList;
	while(pL && loop)
	{
		if(pL == pO)
		{
			m_SelectionCount--;
			loop = 0;
			if(pPrev == 0)
			{
				m_pSelObjList = pL->m_pSelNext;
				pL->m_pSelNext = 0;
			}
			else
			{
				pPrev->m_pSelNext = pL->m_pSelNext;
				pO->m_pSelNext = 0;
			}
		}
		else
		{
			pPrev = pL;
			pL = pL->m_pSelNext;
		}

	}
	if(m_pSelObjList == 0) m_pSelObjEnd = 0;
}

void CFrontCadView::ChangeObject(CUtilView *pUV,CCadObject *pO)
{
	if(pO == NULL) return;
	CFrontCadDoc *pDoc = GetDocument();
	char *s = new char[256];
	if((m_Drawmode == DRAWMODE_SELECT) && pO)
	{
		pDoc->SetModifiedFlag(true);
		switch(pO->GetType())
		{
			case OBJECT_TYPE_NONE:
				break;
			case OBJECT_TYPE_LINE:
				((CCadLine *)pO)->m_P1.x = pUV->m_Edit_X1.GetValue();
				((CCadLine *)pO)->m_P1.y = pUV->m_Edit_Y1.GetValue();
				((CCadLine *)pO)->m_P2.x = pUV->m_Edit_X2.GetValue();
				((CCadLine *)pO)->m_P2.y = pUV->m_Edit_Y2.GetValue();
				((CCadLine *)pO)->SetLineColor(pUV->m_Static_LineColor.GetColor());
				((CCadLine *)pO)->SetOutLineWidth(pUV->m_Edit_LineThickness.GetValue());
				break;
			case OBJECT_TYPE_RECT:
				((CCadRect *)pO)->m_P1.x = pUV->m_Edit_X1.GetValue();
				((CCadRect *)pO)->m_P1.y = pUV->m_Edit_Y1.GetValue();
				((CCadRect *)pO)->m_P2.x = pUV->m_Edit_X2.GetValue();
				((CCadRect *)pO)->m_P2.y = pUV->m_Edit_Y2.GetValue();
				((CCadRect *)pO)->SetLineColor(pUV->m_Static_LineColor.GetColor());
				((CCadRect *)pO)->SetFillColor(pUV->m_Static_FillColor.GetColor());
				((CCadRect *)pO)->SetOutLineWidth(pUV->m_Edit_LineThickness.GetValue());
				break;
			case OBJECT_TYPE_ELLIPSE:
				((CCadElipse *)pO)->m_P1.x = pUV->m_Edit_X1.GetValue();
				((CCadElipse *)pO)->m_P1.y = pUV->m_Edit_Y1.GetValue();
				((CCadElipse *)pO)->m_P2.x = pUV->m_Edit_X2.GetValue();
				((CCadElipse *)pO)->m_P2.y = pUV->m_Edit_Y2.GetValue();
				((CCadElipse *)pO)->SetLineColor(pUV->m_Static_LineColor.GetColor());
				((CCadElipse *)pO)->SetFillColor(pUV->m_Static_FillColor.GetColor());
				((CCadElipse *)pO)->SetOutLineWidth(pUV->m_Edit_LineThickness.GetValue());
				break;
			case OBJECT_TYPE_RNDRECT:
				((CCadRoundRect *)pO)->m_P1.x = pUV->m_Edit_X1.GetValue();
				((CCadRoundRect *)pO)->m_P1.y = pUV->m_Edit_Y1.GetValue();
				((CCadRoundRect *)pO)->m_P2.x = pUV->m_Edit_X2.GetValue();
				((CCadRoundRect *)pO)->m_P2.y = pUV->m_Edit_Y2.GetValue();
				((CCadRoundRect *)pO)->SetLineColor(pUV->m_Static_LineColor.GetColor());
				((CCadRoundRect *)pO)->SetFillColor(pUV->m_Static_FillColor.GetColor());
				((CCadRoundRect *)pO)->SetLineWidth(pUV->m_Edit_LineThickness.GetValue());
				((CCadRoundRect *)pO)->SetArc(CPoint(pUV->m_Edit_X3.GetValue(),pUV->m_Edit_Y3.GetValue()));
				break;
			case OBJECT_TYPE_POLY:
				((CCadPolygon *)pO)->SetLineColor(pUV->m_Static_LineColor.GetColor());
				((CCadPolygon *)pO)->SetWidth(pUV->m_Edit_LineThickness.GetValue());
				break;
			case OBJECT_TYPE_POLYFILL:
				((CCadPolygonFill *)pO)->SetLineColor(pUV->m_Static_LineColor.GetColor());
				((CCadPolygonFill *)pO)->SetWidth(pUV->m_Edit_LineThickness.GetValue());
				((CCadPolygonFill *)pO)->SetFillColor(pUV->m_Static_FillColor.GetColor());
				break;
			case OBJECT_TYPE_LIBCOMP:
				break;
			case OBJECT_TYPE_HOLEROUND:
				((CCadHoleRound *)pO)->m_P1.x = pUV->m_Edit_X1.GetValue();
				((CCadHoleRound *)pO)->m_P1.y = pUV->m_Edit_Y1.GetValue();
				((CCadHoleRound *)pO)->SetRadius(pUV->m_Edit_HoleRadius.GetValue());
				((CCadHoleRound *)pO)->SetWidth(pUV->m_Edit_LineThickness.GetValue());
				((CCadHoleRound *)pO)->SetLineColor(pUV->m_Static_LineColor.GetColor());
				break;
			case OBJECT_TYPE_HOLERECT:
				((CCadRectHole *)pO)->m_P1.x = pUV->m_Edit_X1.GetValue();
				((CCadRectHole *)pO)->m_P1.y = pUV->m_Edit_Y1.GetValue();
				((CCadRectHole *)pO)->SetHieghtWidth(pUV->m_Edit_X2.GetValue(),pUV->m_Edit_Y2.GetValue());
				((CCadRectHole *)pO)->SetOutLineWidth(pUV->m_Edit_LineThickness.GetValue());
				((CCadRectHole *)pO)->SetLineColor(pUV->m_Static_LineColor.GetColor());
				break;
			case OBJECT_TYPE_HOLERND1FLAT:
				((CCadHoleRnd1Flat *)pO)->m_P1.x = pUV->m_Edit_X1.GetValue();
				((CCadHoleRnd1Flat *)pO)->m_P1.y = pUV->m_Edit_Y1.GetValue();
				((CCadHoleRnd1Flat *)pO)->SetRadius(pUV->m_Edit_HoleRadius.GetValue());
				((CCadHoleRnd1Flat *)pO)->SetFlatDist(pUV->m_Edit_FlatToCenterDist.GetValue());
				((CCadHoleRnd1Flat *)pO)->SetWidth(pUV->m_Edit_LineThickness.GetValue());
				((CCadHoleRnd1Flat *)pO)->SetLineColor(pUV->m_Static_LineColor.GetColor());
				break;
			case OBJECT_TYPE_HOLERND2FLAT:
				((CCadHoleRnd2Flat *)pO)->m_P1.x = pUV->m_Edit_X1.GetValue();
				((CCadHoleRnd2Flat *)pO)->m_P1.y = pUV->m_Edit_Y1.GetValue();
				((CCadHoleRnd2Flat *)pO)->SetRadius(pUV->m_Edit_HoleRadius.GetValue());
				((CCadHoleRnd2Flat *)pO)->SetFlatDist(pUV->m_Edit_FlatToCenterDist.GetValue());
				((CCadHoleRnd2Flat *)pO)->SetWidth(pUV->m_Edit_LineThickness.GetValue());
				((CCadHoleRnd1Flat *)pO)->SetLineColor(pUV->m_Static_LineColor.GetColor());
				break;
			case OBJECT_TYPE_TEXT:
				pUV->m_Button_Font.GetWindowText(s,255);
				((CCadText *)pO)->SetFontName(s);
				((CCadText *)pO)->SetColor(pUV->m_Static_TextColor.GetColor());
				((CCadText *)pO)->SetBkColor(pUV->m_Static_BkGrndColor.GetColor());
				((CCadText *)pO)->SetWeight(pUV->m_Combo_FontWeight.GetFontWeight());
				((CCadText *)pO)->m_P1.x = pUV->m_Edit_X1.GetValue();
				((CCadText *)pO)->m_P1.y = pUV->m_Edit_Y1.GetValue();
				((CCadText *)pO)->SetFontHeight(pUV->m_Edit_FontHeight.GetValue());
				((CCadText *)pO)->SetFontWidth(pUV->m_Edit_FontWidth.GetValue());
				((CCadText *)pO)->SetTransparent(pUV->m_Check_TransparentFont.GetCheck());
				pUV->m_Edit_Text.GetWindowText(s,255);
				((CCadText *)pO)->SetText(s);
				((CCadText *)pO)->SetAngle(pUV->m_Edit_TextAngle.GetValue());
				break;
			case OBJECT_TYPE_ARC:
				break;
			case OBJECT_TYPE_ARCCENTERED:
				break;
			case OBJECT_TYPE_ARROW:
				((CCadArrow *)pO)->m_P1.x = pUV->m_Edit_X1.GetValue();
				((CCadArrow *)pO) ->m_P1.y = pUV->m_Edit_Y1.GetValue();
				((CCadArrow *)pO)->m_P2.x = pUV->m_Edit_X2.GetValue();
				((CCadArrow *)pO)->m_P2.y = pUV->m_Edit_Y2.GetValue();
				((CCadArrow *)pO)->SetColor(pUV->m_Static_FillColor.GetColor());
				((CCadArrow *)pO)->SetLegnth(pUV->m_Edit_X3.GetValue());
				((CCadArrow *)pO)->SetWidth(pUV->m_Edit_Y3.GetValue());
				break;
			case OBJECT_TYPE_ORIGIN:
				((CCadOrigin *)pO)->SetColor(pUV->m_Static_LineColor.GetColor());
				((CCadOrigin *)pO)->SetWidth(pUV->m_Edit_LineThickness.GetValue());
				break;
			case OBJECT_TYPE_DIMENSION:
				((CCadDimension *)pO)->SetColor(pUV->m_Static_LineColor.GetColor());
				((CCadDimension *)pO)->SetWidth(pUV->m_Edit_LineThickness.GetValue());
				((CCadDimension *)pO)->GetText()->SetFontHeight(pUV->m_Edit_FontHeight.GetValue());
				pUV->m_Button_Font.GetWindowTextA(s, 255);
				((CCadDimension *)pO)->GetText()->SetFontName(s);
				((CCadDimension *)pO)->GetText()->SetFontWidth(pUV->m_Edit_FontWidth.GetValue());
				((CCadDimension *)pO)->GetText()->SetWeight(pUV->m_Combo_FontWeight.GetFontWeight());
				((CCadDimension *)pO)->UpdateText(GetOrigin());
				break;
			case OBJECT_TYPE_PRINTRECT:
				break;
		}	//end of switch
	}//end of if
	delete[] s;
}	//change object

CLibFormView *CFrontCadView::GetLibFormView()
{
	return this->m_pParent->GetLibFormView();
}

void CFrontCadView::OnKillFocus(CWnd* pNewWnd)
{
	CView::OnKillFocus(pNewWnd);

	// TODO: Add your message handler code here
//	MessageBox("Lost Focus","DEBUG");
}
/*
CPoint CFrontCadView::ScalePosition(CPoint p)
{
	p.x *= ZoomScaleDiv[m_ZoomLevel];
	p.y *= ZoomScaleDiv[m_ZoomLevel];
	return p;
}
*/
CPoint CFrontCadView::GetScrollPosition()
{
	return CPoint(0,0);
}


BOOL CFrontCadView::Create(LPCTSTR lpszClassName, LPCTSTR lpszWindowName, DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID, CCreateContext* pContext)
{
	// TODO: Add your specialized code here and/or call the base class
	dwStyle |=  WS_HSCROLL | WS_VSCROLL;
	return CWnd::Create(lpszClassName, lpszWindowName, dwStyle, rect, pParentWnd, nID, pContext);
}

LRESULT CFrontCadView::OnLibViewRequestReferencePoint(WPARAM wP, LPARAM lP)
{
	m_Drawmode = DRAWMODE_GETREF;
	theApp.GetMainFrame()->UpdateStatusBar("Define Reference Point");
	return 0;
}

void CFrontCadView::DrawGrid(CDC *pDC, int mode,CRect rect)
{
	int i;
	if(this->m_GridOn)
	{
		double gd = m_Grid * ZF[m_ZoomLevel];
		CPen pen,*oldpen;
		pen.CreatePen(PS_SOLID,1,m_GridColor);
		oldpen = pDC->SelectObject(&pen);
		int H,W;
		W = rect.Width();
		H = rect.Height();
		int N = int(W/gd)+1;
		int offset = m_HScrollPos % int(gd);
		for(int i=0;i<N;++i)
		{
			pDC->MoveTo(CPoint(int((i+1)*gd)- offset,0));
			pDC->LineTo(CPoint(int((i+1)*gd)- offset,H-1));
		}
		N = int(H/gd)+1;
		offset = m_VScrollPos % int(gd);

		for(i=0;i<N;++i)
		{
			pDC->MoveTo(CPoint(0,int((i+1) * gd) - offset));
			pDC->LineTo(CPoint(W-1,int((i+1) * gd) - offset));
		}
		pDC->SelectObject(oldpen);
	}
}


void CFrontCadView::NextSnap(int mode, int SnapUnits)
{
	CSize rV;

	switch(mode)
	{
		case VK_LEFT:
			m_SnapPos.x -= SnapUnits;
			break;
		case VK_RIGHT:
			m_SnapPos.x += SnapUnits;
			break;
		case VK_UP:
			m_SnapPos.y -= SnapUnits;
			break;
		case VK_DOWN:
			m_SnapPos.y += SnapUnits;
			break;
	}
	m_MousePos = SnapToScreen(m_SnapPos);
}

CPoint CFrontCadView::Snap(CPoint nP, int SnapUnits)
{
	//-----------------------------------------------------
	// Snap
	// sets the position to nearest snap grid position
	//
	// Parameters:
	//	nP........Current corrected cursor position
	//	SnapUnits.Number of units for the snap grid
	//
	// Returns:
	//	Cursor position in real units for the display
	//-----------------------------------------------------
	int r;
	int x, y;
	int HalfSnap;
	int off;

	x = int(nP.x * ZoomScaleDiv[m_ZoomLevel]);
	y = int(nP.y * ZoomScaleDiv[m_ZoomLevel]);
	if(!m_SnapOff)
	{
		HalfSnap = SnapUnits / 2;
		off = x % SnapUnits;
		if (off > HalfSnap)
			x += SnapUnits - off;
		else
			x -= off;

		off = y % SnapUnits;
		if (off > HalfSnap)
			y += SnapUnits - off;
		else
			y -= off;
	}
	return CPoint(x,y);
}

CPoint CFrontCadView::CorrectMousePosition(CPoint point)
{
	point += CPoint(this->m_HScrollPos,this->m_VScrollPos);
	return point;
}

void CFrontCadView::UpdateScrollbarInfo()
{
	CRect clientrect;
	GetClientRect(&clientrect);
	SCROLLINFO si;
	int cx = clientrect.Width();
	int cy = clientrect.Height();
	int HScrollMax = 0;
	m_HScrollPos = 0;
	m_HPageSize = 0;
   if (cx < (m_DocSize.cx / ZoomScaleDiv[m_ZoomLevel])) {
        HScrollMax = (m_DocSize.cx / ZoomScaleDiv[m_ZoomLevel])- 1;
        m_HPageSize = cx;
        m_HScrollPos = min (m_HScrollPos, (m_DocSize.cx/ ZoomScaleDiv[m_ZoomLevel]) - m_HPageSize - 1);
    }

	si.fMask = SIF_PAGE | SIF_RANGE | SIF_POS;
    si.nMin = 0;
    si.nMax = HScrollMax;
    si.nPos = m_HScrollPos;
    si.nPage = m_HPageSize;
    SetScrollInfo (SB_HORZ, &si, TRUE);
	int VScrollMax=0;
	m_VScrollPos = 0;
	m_VPageSize = 0;

    if (cy < (m_DocSize.cy/ ZoomScaleDiv[m_ZoomLevel]) ) {
        VScrollMax = (m_DocSize.cy/ ZoomScaleDiv[m_ZoomLevel]) - 1;
        m_VPageSize = cy;
        m_VScrollPos = min (m_VScrollPos, (m_DocSize.cy/ ZoomScaleDiv[m_ZoomLevel]) - m_VPageSize - 1);
    }
    si.fMask = SIF_PAGE | SIF_RANGE | SIF_POS;
    si.nMin = 0;
    si.nMax = VScrollMax;
    si.nPos = m_VScrollPos;
    si.nPage = m_VPageSize;

    SetScrollInfo (SB_VERT, &si, TRUE);
}

void CFrontCadView::OnEditCopy()
{
	// TODO: Add your command handler code here
	m_Drawmode = DRAWMODE_COPY;
	m_DrawState = DRAWSTATE_GETREFEREMCE;
	theApp.GetMainFrame()->UpdateStatusBar("Define Reference Point");
}

void CFrontCadView::OnUpdateEditCopy(CCmdUI* pCmdUI)
{
	// TODO: Add your command update UI handler code here

}

void CFrontCadView::OnEditCut()
{
	// TODO: Add your command handler code here
	m_Drawmode = DRAWMODE_CUT;
	m_DrawState = DRAWSTATE_GETREFEREMCE;
	theApp.GetMainFrame()->UpdateStatusBar("Define Reference Point");
}

void CFrontCadView::OnUpdateEditCut(CCmdUI* pCmdUI)
{
	// TODO: Add your command update UI handler code here

}

void CFrontCadView::OnEditPaste()
{
	// TODO: Add your command handler code here
	m_Drawmode = DRAWMODE_PASTE;
	m_DrawState = DRAWSTATE_PLACE;
	theApp.GetMainFrame()->UpdateStatusBar("Select Paste Location");
}

void CFrontCadView::OnUpdateEditPaste(CCmdUI* pCmdUI)
{
	// TODO: Add your command update UI handler code here

}

void CFrontCadView::OnToolbarSelectregion()
{
	// TODO: Add your command handler code here
	m_Drawmode = DRAWMODE_SELECTREGION;
	theApp.GetMainFrame()->UpdateStatusBar("Select Region:Select First Corner");
}

CPoint CFrontCadView::GetCursorPos()
{
	return m_MousePos;
}

BOOL CFrontCadView::SetCursorPos(CPoint pos)
{
	m_MousePos = pos;
	pos -= CPoint(this->m_HScrollPos,this->m_VScrollPos);
	ClientToScreen(&pos);
	return ::SetCursorPos(pos.x,pos.y);
}

BOOL CFrontCadView::DestroyWindow()
{
	// TODO: Add your specialized code here and/or call the base class
	return CView::DestroyWindow();
}


void CFrontCadView::OnFilePrinttoclipboard()
{
	CBitmap 	bitmap;
	CClientDC	dc(this);
	CDC 		memDC;
	CRect		rect;
	CPoint P1, P2;

	memDC.CreateCompatibleDC(&dc);

	if (m_nSelRegionLock == 0)
	{
		CCadPrintRect *pPRect;
		CFrontCadDoc *pDoc = GetDocument();
		pPRect = pDoc->GetPrintRect();
		if (pPRect)
		{
			rect = pPRect->GetRect();
		}
		else
		{
			rect.SetRect(CPoint(0, 0), CPoint(8500, 11000));
		}
		P1 = rect.TopLeft();
		P1 = CScale(ZF[m_ZoomLevel], ZF[m_ZoomLevel]) * P1;
		P2 = rect.BottomRight();
		P2 = CScale(ZF[m_ZoomLevel], ZF[m_ZoomLevel]) * P2;
	}
	else
	{
		P1 = CScale(ZF[m_ZoomLevel], ZF[m_ZoomLevel]) * m_SelRegion_P1;
		P2 = CScale(ZF[m_ZoomLevel], ZF[m_ZoomLevel]) * m_SelRegion_P2;
	}
	rect.SetRect((P1), (P2));
	rect.NormalizeRect();
	bitmap.CreateCompatibleBitmap(&dc, rect.Width(), rect.Height());

	CBitmap* pOldBitmap = memDC.SelectObject(&bitmap);

	PrintToDC(&memDC);
	this->OpenClipboard();
	EmptyClipboard();
	SetClipboardData(CF_BITMAP, bitmap.GetSafeHandle());
	CloseClipboard();

	memDC.SelectObject(pOldBitmap);
	bitmap.Detach();
}

// ///helper function for printing to clipboard
void CFrontCadView::PrintToDC(CDC * pDC)
{
	CFrontCadDoc* pDoc = GetDocument();
	CBrush br;
	CRect rect;
	CPoint P1, P2;
	CPoint Offset;

	if (m_nSelRegionLock == 0)
	{
		CCadPrintRect *pPRect;
		CFrontCadDoc *pDoc = GetDocument();
		pPRect = pDoc->GetPrintRect();
		if (pPRect)
		{
			rect = pPRect->GetRect();
		}
		else
		{
			rect.SetRect(CPoint(0, 0), CPoint(8500, 11000));
		}
		P1 = rect.TopLeft();
		P1 = CScale(ZF[m_ZoomLevel], ZF[m_ZoomLevel]) * P1;
		P2 = rect.BottomRight();
		P2 = CScale(ZF[m_ZoomLevel], ZF[m_ZoomLevel]) * P2;
	}
	else
	{
		P1 = CScale(ZF[m_ZoomLevel],ZF[m_ZoomLevel]) * m_SelRegion_P1;
		P2 = CScale(ZF[m_ZoomLevel],ZF[m_ZoomLevel]) * m_SelRegion_P2;
	}
	Offset = -P1;
	rect.SetRect(0, 0, P2.x - P1.x, P2.y - P1.y);
	br.CreateSolidBrush(pDoc->GetBkColor());
	pDC->FillRect(&rect, &br);
	//----------------------------------
	// Make all of the objects dirty
	///---------------------------------
	pDoc->MakeDirty();
	//----------------------------------
	// Print to the clipboard
	//----------------------------------
	pDoc->Print(pDC, OBJECT_MODE_FINAL, Offset, CScale(ZF[m_ZoomLevel], ZF[m_ZoomLevel]));
	//make dirty all over again.
	pDoc->MakeDirty();

}

CPoint CFrontCadView::GetOrigin(void)
{
	CPoint rP = CPoint(0,0);
	CFrontCadDoc *pDoc = GetDocument();
	CCadObject *pOb;
	int loop = 1;

	pOb = pDoc->GetHead();
	while (pOb && loop)
	{
		if (pOb->GetType() == OBJECT_TYPE_ORIGIN)
		{
			loop = 0;
			rP = pOb->m_P1;
		}
		else
			pOb = pOb->m_pNext;
	}
	return rP;
}

afx_msg LRESULT CFrontCadView::OnUpdatedimensions(WPARAM wParam, LPARAM lParam)
{
	CPoint p = GetOrigin();
	CFrontCadDoc *pDoc = GetDocument();
	CCadObject *pOb = pDoc->GetHead();
	CCadDimension *pDim;

	while (pOb)
	{
		if (pOb->GetType() == OBJECT_TYPE_DIMENSION)
		{
			pDim = (CCadDimension*)pOb;
			pDim->UpdateText(p);
		}
		pOb = pOb->m_pNext;
	}
	return 0;
}


void CFrontCadView::OnViewRenderenable()
{
	// TODO: Add your command handler code here
	CDialogRenderEnable dlg;

	dlg.DoModal();
}


CCadObject * CFrontCadView::FindHole(CCadObject * pHead,CPoint p)
{
	CCadObject *pRO = 0;

	while (pHead && !pRO)
	{
		if (pHead->CheckSelected(p) && IsHole(pHead))
		{
			pRO = pHead;
		}
		else
			pHead = pHead->m_pNext;
	}
	return pRO;
}


int CFrontCadView::IsHole(CCadObject * pObj)
{
	int rV = 0;
	if ((pObj->GetType() == OBJECT_TYPE_HOLEROUND) ||
		(pObj->GetType() == OBJECT_TYPE_HOLERND1FLAT) ||
		(pObj->GetType() == OBJECT_TYPE_HOLERND2FLAT) ||
		(pObj->GetType() == OBJECT_TYPE_HOLERECT) )
		rV = 1;
	else if (pObj->GetType() == OBJECT_TYPE_LIBCOMP)
		rV = 2;
	return rV;
}


void CFrontCadView::OnAlignCentersvertical()
{
	// TODO: Add your command handler code here
	CCadObject *pObj,*pObjRef;
	CPoint Ref,NewRef;
	pObjRef = PopSelList();	//this is the reference object
	pObj = PopSelList();
	Ref = pObjRef->GetCenter();
	Ref.x = 0;
	pObjRef->SetSelected(0);
	while (pObj)
	{
		NewRef = pObj->GetCenter();
		NewRef.x = 0;
		CSize v = NewRef - Ref;
		pObj->ChangeCenter(v);
		pObj->SetSelected(0);
		pObj = PopSelList();
	}
	Invalidate();
}


void CFrontCadView::OnAlignCentershorizontal()
{
	char *s = new char[256];
	CCadObject *pObj, *pObjRef;
	CPoint Ref, NewRef;
	pObjRef = PopSelList();	//this is the reference object
	pObj = PopSelList();
	Ref = pObjRef->GetCenter();
	Ref.y = 0;

	sprintf(s, "X=%d  Y=%d", Ref.x, Ref.y);
	((CFrontCadApp *)AfxGetApp())->WriteToLogFile(s);

	pObjRef->SetSelected(0);
	while (pObj)
	{
		NewRef = pObj->GetCenter();
		NewRef.y = 0;

		sprintf(s, "X=%d  Y=%d", NewRef.x, NewRef.y);
		((CFrontCadApp *)AfxGetApp())->WriteToLogFile(s);

		CSize v = NewRef - Ref;

		sprintf(s, "X=%d  Y=%d", v.cx, v.cy);
		((CFrontCadApp *)AfxGetApp())->WriteToLogFile(s);

		pObj->ChangeCenter(v);
		pObj->SetSelected(0);
		pObj = PopSelList();
	}
	Invalidate();
	delete[] s;
}


void CFrontCadView::OnAlignCenters()
{
	CCadObject *pObj, *pObjRef;
	CPoint Ref, NewRef;
	pObjRef = PopSelList();	//this is the reference object
	pObj = PopSelList();
	Ref = pObjRef->GetCenter();
	pObjRef->SetSelected(0);
	while (pObj)
	{
		NewRef = pObj->GetCenter();
		CSize v = NewRef - Ref;
		pObj->ChangeCenter(v);
		pObj->SetSelected(0);
		pObj = PopSelList();
	}
	Invalidate();
}


void CFrontCadView::OnUpdateAlignCentersUI(CCmdUI *pCmdUI)
{
	// TODO: Add your command update UI handler code here
	pCmdUI->Enable((1 < m_SelectionCount)?1:0);
}


void CFrontCadView::OnUpdateAlignCentershorizontal(CCmdUI *pCmdUI)
{
	// TODO: Add your command update UI handler code here
	pCmdUI->Enable((1 < m_SelectionCount) ? 1 : 0);
}


void CFrontCadView::OnUpdateAlignCentersvertical(CCmdUI *pCmdUI)
{
	// TODO: Add your command update UI handler code here
	pCmdUI->Enable((1 < m_SelectionCount) ? 1 : 0);
}


void CFrontCadView::OnAlignBottom()
{
	// TODO: Add your command handler code here
	CCadObject *pObj, *pRefObj;
	CPoint Ref, NewRef;

	pRefObj = PopSelList();
	Ref = pRefObj->GetReference() + pRefObj->GetSize();
	pRefObj->SetSelected(0);
	pObj = PopSelList();
	while (pObj)
	{
		NewRef = pObj->GetReference() + pObj->GetSize() - Ref;
		NewRef.x = 0;
		pObj->AdjustRefernce(NewRef);
		pObj->SetSelected(0);
		pObj = PopSelList();

	}
	Invalidate();
}


void CFrontCadView::OnAlignLeft()
{
	// TODO: Add your command handler code here
	CCadObject *pObj, *pRefObj;
	CPoint Ref, NewRef;

	pRefObj = PopSelList();
	Ref = pRefObj->GetReference() + pRefObj->GetSize();
	pRefObj->SetSelected(0);
	pObj = PopSelList();
	while (pObj)
	{
		NewRef = pObj->GetReference()+ pObj->GetSize() - Ref;
		NewRef.y = 0;
		pObj->AdjustRefernce(NewRef);
		pObj->SetSelected(0);
		pObj = PopSelList();

	}
	Invalidate();
}


void CFrontCadView::OnAlignRight()
{
	// TODO: Add your command handler code here
	CCadObject *pObj, *pRefObj;
	CPoint Ref, NewRef;

	pRefObj = PopSelList();
	Ref = pRefObj->GetReference();
	pRefObj->SetSelected(0);
	pObj = PopSelList();
	while (pObj)
	{
		NewRef = pObj->GetReference() - Ref;
		NewRef.y = 0;
		pObj->AdjustRefernce(NewRef);
		pObj->SetSelected(0);
		pObj = PopSelList();

	}
	Invalidate();
}


void CFrontCadView::OnAlignSizeboth()
{
	// TODO: Add your command handler code here
	CCadObject *pObj, *pRefObj;
	CSize RefSize, NewSize;

	pRefObj = PopSelList();
	RefSize = pRefObj->GetSize();
	pRefObj->SetSelected(0);
	pObj = PopSelList();
	while (pObj)
	{
		NewSize = RefSize - pObj->GetSize();
		pObj->ChangeSize(NewSize);
		pObj->SetSelected(0);
		pObj = PopSelList();

	}
	Invalidate();
}


void CFrontCadView::OnAlignSizex()
{
	CCadObject *pObj, *pRefObj;
	CSize RefSize, NewSize;

	pRefObj = PopSelList();
	RefSize = pRefObj->GetSize();
	pRefObj->SetSelected(0);
	pObj = PopSelList();
	while (pObj)
	{
		NewSize = RefSize - pObj->GetSize();
		NewSize.cy = 0;
		pObj->ChangeSize(NewSize);
		pObj->SetSelected(0);
		pObj = PopSelList();

	}
	Invalidate();
}


void CFrontCadView::OnAlignSizey()
{
	// TODO: Add your command handler code here
	CCadObject *pObj, *pRefObj;
	CSize RefSize, NewSize;

	pRefObj = PopSelList();
	RefSize = pRefObj->GetSize();
	pRefObj->SetSelected(0);
	pObj = PopSelList();
	while (pObj)
	{
		NewSize = RefSize - pObj->GetSize();
		NewSize.cx = 0;
		pObj->ChangeSize(NewSize);
		pObj->SetSelected(0);
		pObj = PopSelList();

	}
	Invalidate();
}


void CFrontCadView::OnAlignTop()
{
	// TODO: Add your command handler code here
	CCadObject *pObj, *pRefObj;
	CPoint Ref, NewRef;

	pRefObj = PopSelList();
	Ref = pRefObj->GetReference();
	pRefObj->SetSelected(0);
	pObj = PopSelList();
	while (pObj)
	{
		NewRef = pObj->GetReference() - Ref;
		NewRef.x = 0;
		pObj->AdjustRefernce(NewRef);
		pObj->SetSelected(0);
		pObj = PopSelList();

	}
	Invalidate();
}


void CFrontCadView::OnUpdateAlignBottom(CCmdUI *pCmdUI)
{
	// TODO: Add your command update UI handler code here
	pCmdUI->Enable((1 < m_SelectionCount) ? 1 : 0);
}


void CFrontCadView::OnUpdateAlignLeft(CCmdUI *pCmdUI)
{
	// TODO: Add your command update UI handler code here
	pCmdUI->Enable((1 < m_SelectionCount) ? 1 : 0);
}


void CFrontCadView::OnUpdateAlignRight(CCmdUI *pCmdUI)
{
	// TODO: Add your command update UI handler code here
	pCmdUI->Enable((1 < m_SelectionCount) ? 1 : 0);
}


void CFrontCadView::OnUpdateAlignSizeboth(CCmdUI *pCmdUI)
{
	// TODO: Add your command update UI handler code here
	pCmdUI->Enable((1 < m_SelectionCount) ? 1 : 0);
}


void CFrontCadView::OnUpdateAlignSizex(CCmdUI *pCmdUI)
{
	// TODO: Add your command update UI handler code here
	pCmdUI->Enable((1 < m_SelectionCount) ? 1 : 0);
}


void CFrontCadView::OnUpdateAlignSizey(CCmdUI *pCmdUI)
{
	// TODO: Add your command update UI handler code here
	pCmdUI->Enable((1 < m_SelectionCount) ? 1 : 0);
}


void CFrontCadView::OnUpdateAlignTop(CCmdUI *pCmdUI)
{
	// TODO: Add your command update UI handler code here
	pCmdUI->Enable((1 < m_SelectionCount) ? 1 : 0);
}


void CFrontCadView::OnButtonPrintRectangle()
{
	//-------------------------------------------
	// Place a Print Rectangle on the Drawing
	//-------------------------------------------
	m_Drawmode = DRAWMODE_PRINTRECT;
	m_DrawState = DRAWSTATE_WAITMOUSE_DOWN;
	theApp.GetMainFrame()->UpdateStatusBar("Print Rectangle:Place First Corner");
}


CPoint CFrontCadView::SnapToScreen(CPoint npSnap)
{
	//------------------------------------------------------
	// SnapToScreen
	//	This function is used to turn the actual coordinates
	// of a point into points on the screen.
	//
	// parameters:
	//	nSnap.......point that was calculated by the function Snap
	//
	//	Returns
	//		A scalled point
	//---------------------------------------------------------
	int x, y;

	x = int(npSnap.x * ZF[m_ZoomLevel]);
	y = int(npSnap.y * ZF[m_ZoomLevel]);
	return CPoint(x,y);
}


CPoint CFrontCadView::Scale(CPoint nP)
{
	int x, y;

	x = int(nP.x * ZoomScaleDiv[m_ZoomLevel]);
	y = int(nP.y * ZoomScaleDiv[m_ZoomLevel]);
	return CPoint(x,y);
}
