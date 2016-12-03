// FrontCad.cpp : Defines the class behaviors for the application.
//

#include "stdafx.h"
#include "FrontCad.h"

#include "MainFrm.h"
#include "FrontCadDoc.h"
#include "FrontCadView.h"
#include "FrontCadDefines.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CFrontCadApp

BEGIN_MESSAGE_MAP(CFrontCadApp, CWinApp)
	//{{AFX_MSG_MAP(CFrontCadApp)
	ON_COMMAND(ID_APP_ABOUT, OnAppAbout)
		// NOTE - the ClassWizard will add and remove mapping macros here.
		//    DO NOT EDIT what you see in these blocks of generated code!
	//}}AFX_MSG_MAP
	// Standard file based document commands
	ON_COMMAND(ID_FILE_NEW, CWinApp::OnFileNew)
	ON_COMMAND(ID_FILE_OPEN, CWinApp::OnFileOpen)
	// Standard print setup command
	ON_COMMAND(ID_FILE_PRINT_SETUP, CWinApp::OnFilePrintSetup)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CFrontCadApp construction

CFrontCadApp::CFrontCadApp()
{
	// TODO: add construction code here,
	// Place all significant initialization in InitInstance
	this->m_pLibView = 0;
	this->m_pMainView = 0;
	fname = "FrontCad.ini";
	m_pLogFile = 0;
	fopen_s(&m_pLogFile,"Log.txt", "w");
 
}

/////////////////////////////////////////////////////////////////////////////
// The one and only CFrontCadApp object

CFrontCadApp theApp;

/////////////////////////////////////////////////////////////////////////////
// CFrontCadApp initialization

BOOL CFrontCadApp::InitInstance()
{
	// Standard initialization
	// If you are not using these features and wish to reduce the size
	//  of your final executable, you should remove from the following
	//  the specific initialization routines you do not need.

	// Change the registry key under which our settings are stored.
	// TODO: You should modify this string to be something appropriate
	// such as the name of your company or organization.
	SetRegistryKey(_T("Local AppWizard-Generated Applications"));

	LoadStdProfileSettings();  // Load standard INI file options (including MRU)

	// Register the application's document templates.  Document templates
	//  serve as the connection between documents, frame windows and views.

	CSingleDocTemplate* pDocTemplate;
	pDocTemplate = new CSingleDocTemplate(
		IDR_MAINFRAME,
		RUNTIME_CLASS(CFrontCadDoc),
		RUNTIME_CLASS(CMainFrame),       // main SDI frame window
		RUNTIME_CLASS(CFrontCadView));
	AddDocTemplate(pDocTemplate);

	// Enable DDE Execute open
	EnableShellOpen();
	RegisterShellFileTypes(TRUE);

	// Parse command line for standard shell commands, DDE, file open
	CCommandLineInfo cmdInfo;
	ParseCommandLine(cmdInfo);

	// Dispatch commands specified on the command line
	if (!ProcessShellCommand(cmdInfo))
		return FALSE;

	// The one and only window has been initialized, so show and update it.
	m_pMainWnd->ShowWindow(SW_SHOW);
	m_pMainWnd->UpdateWindow();

	// Enable drag/drop open
	m_pMainWnd->DragAcceptFiles();
	LoadSettings();
	return TRUE;
}


/////////////////////////////////////////////////////////////////////////////
// CAboutDlg dialog used for App About

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// Dialog Data
	//{{AFX_DATA(CAboutDlg)
	enum { IDD = IDD_ABOUTBOX };
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAboutDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	//{{AFX_MSG(CAboutDlg)
		// No message handlers
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
public:
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
	//{{AFX_DATA_INIT(CAboutDlg)
	//}}AFX_DATA_INIT
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAboutDlg)
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
	//{{AFX_MSG_MAP(CAboutDlg)
		// No message handlers
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

// App command to run the dialog
void CFrontCadApp::OnAppAbout()
{
	CAboutDlg aboutDlg;
	aboutDlg.DoModal();
}

/////////////////////////////////////////////////////////////////////////////
// CFrontCadApp message handlers


int CFrontCadApp::ExitInstance() 
{
	// TODO: Add your specialized code here and/or call the base class
	SaveSettings();
	if (m_pLogFile) fclose(m_pLogFile);
	return CWinApp::ExitInstance();
}


void CFrontCadApp::LoadSettings()
{
	m_OriginAttrib.m_Color = GetProfileInt("Origin", "Color", (int)RGB(0, 0, 0));
	m_OriginAttrib.m_Width = GetProfileInt("Origin", "Width", 10);

	m_DimAttrib.m_Color = GetProfileInt("Dimension", "Color", (int)RGB(0, 0, 0));
	m_DimAttrib.m_Width = GetProfileInt("Dimension", "Width", 10);
	m_DimAttrib.m_Text.m_Color = GetProfileInt("Dimension", "Color", (int)RGB(0, 0, 0));
	m_DimAttrib.m_Text.m_BkColor = GetProfileInt("Dimension", "BackColor", (int)RGB(255, 255, 255));
	m_DimAttrib.m_Text.m_Angle = GetProfileInt("Dimension", "Angle", 0);
	m_DimAttrib.m_Text.m_FontHeight = GetProfileInt("Dimension", "FontHeight", 200);
	m_DimAttrib.m_Text.m_FontWidth = GetProfileInt("Dimension", "Width", 0);
	m_DimAttrib.m_Text.m_Format = GetProfileInt("Dimension", "Format", DT_BOTTOM | DT_SINGLELINE);
	m_DimAttrib.m_Text.m_Transparent = GetProfileInt("Dimension", "Transparent", 1);
	m_DimAttrib.m_Text.m_Weight = GetProfileInt("Dimension", "Weight", FW_DEMIBOLD);
	m_DimAttrib.m_Text.m_pFontName = new char[LF_FACESIZE];
	strcpy_s(m_DimAttrib.m_Text.m_pFontName, LF_FACESIZE, LPCTSTR(GetProfileString("Dimension", "Font", "Arial")));
	m_DimAttrib.m_Text.m_pText = 0;

	m_ArrowAttrib.m_Color = GetProfileInt("Arrow", "Color", (int)RGB(0, 0, 0));
	m_ArrowAttrib.m_Len = GetProfileInt("Arrow", "Len", 300);
	m_ArrowAttrib.m_LineWidth = GetProfileInt("Arrow", "Width", 200);

	m_RndRectAttributes.m_FillColor = GetProfileInt("RndRect","FillColor",(int)RGB(0,128,255));
	m_RndRectAttributes.m_LineColor = GetProfileInt("RndRect","LineColor",(int)RGB(0,255,0));
	m_RndRectAttributes.m_OutLineWidth = GetProfileInt("RndRect","LineWidth", 100);
	m_RndRectAttributes.m_X3 = GetProfileInt("RndRect","X3",250);
	m_RndRectAttributes.m_Y3 =  GetProfileInt("RndRect","Y3",250);

	m_LineAttrib.m_Width = GetProfileInt("Line","LineWidth",125);
	m_LineAttrib.m_Color = GetProfileInt("Line","LineColor",(int)RGB(0,0,0));

	m_RectAttributes.m_FillColor = GetProfileInt("Rect","FillColor",(int)RGB(0,0,255) );
	m_RectAttributes.m_LineColor = GetProfileInt("Rect","LineColor",(int)RGB(0,0,0) );
	m_RectAttributes.m_Width = GetProfileInt("Rect","LineWidth",100);

	m_EllipseAttributes.m_FillColor = GetProfileInt("Ellipse","FillColor",(int)RGB(0,0,255) );
	m_EllipseAttributes.m_LineColor = GetProfileInt("Ellipse","LineColor",(int)RGB(0,0,0) );
	m_EllipseAttributes.m_Width = GetProfileInt("Ellipse","LineWidth",100);

	m_PolyAttributes.m_LineColor = GetProfileInt("Poly","LineColor",(int)RGB(0,0,0) );
	m_PolyAttributes.m_Width = GetProfileInt("Poly","LineWidth",50);

	m_PolyFillAttributes.m_Width = GetProfileInt("PolyFill","LineWidth",50);
	m_PolyFillAttributes.m_FillColor = GetProfileInt("PolyFill","FillColor",(int)RGB(0,255,0) );
	m_PolyFillAttributes.m_LineColor = GetProfileInt("PolyFill","LineColor",(int)RGB(255,0,0) );

	m_ArcAttributes.m_LineColor = GetProfileInt("Arc","LineColor",(int)RGB(0,0,0) );
	m_ArcAttributes.m_Width = GetProfileInt("Arc","LineWidth",50);

	m_HoleRoundAttributes.m_Radius = GetProfileInt("HoleRound","Radius",188);
	m_HoleRoundAttributes.m_LineColor = GetProfileInt("HoleRound","LineColor",(int)RGB(0,0,0) );
	m_HoleRoundAttributes.m_Width = GetProfileInt("HoleRound","LineWidth",20);

	m_HoleRnd2FlatAttributes.m_Width = GetProfileInt("HoleRound2Flat","LineWidth",20);
	m_HoleRnd2FlatAttributes.m_FlatDist = GetProfileInt("HoleRound2Flat","FlatDist",150);
	m_HoleRnd2FlatAttributes.m_LineColor = GetProfileInt("HoleRound2Flat","LineColor",(int)RGB(0,0,0) );
	m_HoleRnd2FlatAttributes.m_Radius = GetProfileInt("HoleRound2Flat","Radius",188);

	m_HoleRnd1FlatAttributes.m_FlatDist = GetProfileInt("HoleRound1Flat","FlatDist",150);
	m_HoleRnd1FlatAttributes.m_LineColor = GetProfileInt("HoleRound1Flat","LineColor",(int)RGB(0,0,0) );
	m_HoleRnd1FlatAttributes.m_Radius = GetProfileInt("HoleRound1Flat","Radius",188);
	m_HoleRnd1FlatAttributes.m_Width = GetProfileInt("HoleRound1Flat","LineWidth",20);;

	m_RectHoleAttributes.m_H = GetProfileInt("HoleRect","Height",250);
	m_RectHoleAttributes.m_W = GetProfileInt("HoleRect","Width",250);
	m_RectHoleAttributes.m_Width = GetProfileInt("HoleRect","LineWidth",20);
	m_RectHoleAttributes.m_LineColor = GetProfileInt("HoleRect","LineColor",(int)RGB(0,0,0) );

	m_TextAttributes.m_Color = GetProfileInt("Text","Color",(int)RGB(0,0,0) );
	m_TextAttributes.m_BkColor = GetProfileInt("Text","BackColor",(int)RGB(255,255,255) );
	m_TextAttributes.m_Angle = GetProfileInt("Text","Angle",0);
	m_TextAttributes.m_FontHeight = GetProfileInt("Text","FontHeight",200);
	m_TextAttributes.m_FontWidth = GetProfileInt("Text","Width",0);
	m_TextAttributes.m_Format = GetProfileInt("Text","Format",DT_BOTTOM | DT_SINGLELINE);
	m_TextAttributes.m_Transparent = GetProfileInt("Text","Transparent",1);
	m_TextAttributes.m_Weight = GetProfileInt("Text","Weight",FW_DEMIBOLD);
	m_TextAttributes.m_pFontName = new char[LF_FACESIZE];
	strcpy_s(m_TextAttributes.m_pFontName, LF_FACESIZE,LPCTSTR(GetProfileString("Text","Font","Arial")));
	m_TextAttributes.m_pText = 0;

	m_SCALEWIZattributes.m_ArcEnd = GetProfileInt("SWiz","ArcEnd",2250);
	m_SCALEWIZattributes.m_ArcLineWidth = GetProfileInt("SWiz","ArcLineWidth",40);
	m_SCALEWIZattributes.m_ArcRadius = GetProfileInt("SWiz","ArcRadius",375);
	m_SCALEWIZattributes.m_ArcStart = GetProfileInt("SWiz","ArcStart",3150);
	m_SCALEWIZattributes.m_ArcColor = GetProfileInt("SWiz","ArcColor",(int)RGB(0,0,0) );
	m_SCALEWIZattributes.m_DivisionLineLegnth = GetProfileInt("SWiz","DivLinLen",100);
	m_SCALEWIZattributes.m_DivisionLineWidth = GetProfileInt("SWiz","DivLinW",20);
	m_SCALEWIZattributes.m_Divisions = GetProfileInt("SWiz","Divisions",10);
	m_SCALEWIZattributes.m_DivLineColor = GetProfileInt("SWiz","DivColor",(int)RGB(0,0,0) );
	m_SCALEWIZattributes.m_SubdivisionLengnth = GetProfileInt("SWiz","SubLinLen",85);
	m_SCALEWIZattributes.m_SubDivisionLineWidth = GetProfileInt("SWiz","SubLinW",15);
	m_SCALEWIZattributes.m_SubDivisions = GetProfileInt("SWiz","SubDiv",3);
	m_SCALEWIZattributes.m_SubDivColor = GetProfileInt("SWiz","SubDivColor",(int)RGB(0,0,0) );
	m_SCALEWIZattributes.m_FlatDist = GetProfileInt("SWiz","FlatDist",0);
	m_SCALEWIZattributes.m_HoleSize = GetProfileInt("SWiz","HoleRad",188);
	m_SCALEWIZattributes.m_HoleType = GetProfileInt("SWiz","HoleType",OBJECT_TYPE_HOLEROUND);
	char *s = new char[32];
	for(int i=0;i< SCALE_LABELS_SIZE;++i)
	{
		m_SCALEWIZattributes.m_pLabels[i] = new char[SCALE_LABELS_STRING_SIZE];
		sprintf_s(s, SCALE_LABELS_STRING_SIZE,"%d",i);
		strcpy_s(m_SCALEWIZattributes.m_pLabels[i], SCALE_LABELS_STRING_SIZE,s);
	}
	delete[] s;
	m_SCALEWIZattributes.m_FontSize = GetProfileInt("SWiz","FontSize",150);
	m_SCALEWIZattributes.m_FontWeight = GetProfileInt("SWiz","Weight",FW_DEMIBOLD);
	m_SCALEWIZattributes.m_DistToTick = GetProfileInt("SWiz","DistToTick",150);
	m_SCALEWIZattributes.m_TextColor = GetProfileInt("SWiz","TextColor",(int)RGB(0,0,0) );
	m_SCALEWIZattributes.m_BkColor = GetProfileInt("SWiz","TextBkColor",(int)RGB(255,255,255) );
	m_SCALEWIZattributes.m_pFont = new char[LF_FACESIZE];
	strcpy_s(m_SCALEWIZattributes.m_pFont, LF_FACESIZE,LPCTSTR(GetProfileString("SWiz","Font","Arial")) );
}

void CFrontCadApp::SaveSettings()
{
	WriteProfileInt("Dimension", "Color", (int)m_DimAttrib.m_Color);
	WriteProfileInt("Dimension", "Width", m_DimAttrib.m_Width);
	WriteProfileInt("Dimension", "Color", (int)m_DimAttrib.m_Text.m_Color);
	WriteProfileInt("Dimension", "BackColor",(int)m_DimAttrib.m_Text.m_BkColor);
	WriteProfileInt("Dimension", "Angle", m_DimAttrib.m_Text.m_Angle);
	WriteProfileInt("Dimension", "FontHeight", m_DimAttrib.m_Text.m_FontHeight);
	WriteProfileInt("Dimension", "Width", m_DimAttrib.m_Text.m_FontWidth);
	WriteProfileInt("Dimension", "Format", m_DimAttrib.m_Text.m_Format);
	WriteProfileInt("Dimension", "Transparent", m_DimAttrib.m_Text.m_Transparent);
	WriteProfileInt("Dimension", "Weight", m_DimAttrib.m_Text.m_Weight);
	WriteProfileStringA("Dimension", "Font", m_DimAttrib.m_Text.m_pFontName);

	WriteProfileInt("Origin", "Color", (int)m_OriginAttrib.m_Color);
	WriteProfileInt("Origin", "Width", m_OriginAttrib.m_Width);

	WriteProfileInt("Arrow", "Color", (int)m_ArrowAttrib.m_Color);
	WriteProfileInt("Arrow", "Len", m_ArrowAttrib.m_Len);
	WriteProfileInt("Arrow", "Width", m_ArrowAttrib.m_LineWidth);

	WriteProfileInt("RndRect","FillColor",(int)m_RndRectAttributes.m_FillColor);
	WriteProfileInt("RndRect","LineColor",(int)m_RndRectAttributes.m_LineColor);
	WriteProfileInt("RndRect","LineWidth", m_RndRectAttributes.m_OutLineWidth);
	WriteProfileInt("RndRect","X3",m_RndRectAttributes.m_X3);
	WriteProfileInt("RndRect","Y3",m_RndRectAttributes.m_Y3);

	WriteProfileInt("Line","LineWidth",m_LineAttrib.m_Width);
	WriteProfileInt("Line","LineColor",(int)m_LineAttrib.m_Color);

	WriteProfileInt("Rect","FillColor",(int)m_RectAttributes.m_FillColor );
	WriteProfileInt("Rect","LineColor",(int)m_RectAttributes.m_LineColor);
	WriteProfileInt("Rect","LineWidth",m_RectAttributes.m_Width);

	WriteProfileInt("Ellipse","FillColor",(int)m_EllipseAttributes.m_FillColor );
	WriteProfileInt("Ellipse","LineColor",(int)m_EllipseAttributes.m_LineColor );
	WriteProfileInt("Ellipse","LineWidth",m_EllipseAttributes.m_Width );

	WriteProfileInt("Poly","LineColor",(int)m_PolyAttributes.m_LineColor );
	WriteProfileInt("Poly","LineWidth",m_PolyAttributes.m_Width);

	WriteProfileInt("PolyFill","LineWidth",m_PolyFillAttributes.m_Width);
	WriteProfileInt("PolyFill","FillColor",(int)m_PolyFillAttributes.m_FillColor );
	WriteProfileInt("PolyFill","LineColor",(int)m_PolyFillAttributes.m_LineColor );

	WriteProfileInt("Arc","LineColor",(int)m_ArcAttributes.m_LineColor );
	WriteProfileInt("Arc","LineWidth",m_ArcAttributes.m_Width);

	WriteProfileInt("HoleRound","Radius",m_HoleRoundAttributes.m_Radius);
	WriteProfileInt("HoleRound","LineColor",(int)m_HoleRoundAttributes.m_LineColor );
	WriteProfileInt("HoleRound","LineWidth",m_HoleRoundAttributes.m_Width);

	WriteProfileInt("HoleRound2Flat","LineWidth",m_HoleRnd2FlatAttributes.m_Width);
	WriteProfileInt("HoleRound2Flat","FlatDist",m_HoleRnd2FlatAttributes.m_FlatDist);
	WriteProfileInt("HoleRound2Flat","LineColor",(int)m_HoleRnd2FlatAttributes.m_LineColor );
	WriteProfileInt("HoleRound2Flat","Radius",m_HoleRnd2FlatAttributes.m_Radius);

	WriteProfileInt("HoleRound1Flat","FlatDist",m_HoleRnd1FlatAttributes.m_FlatDist);
	WriteProfileInt("HoleRound1Flat","LineColor",(int)m_HoleRnd1FlatAttributes.m_LineColor );
	WriteProfileInt("HoleRound1Flat","Radius",m_HoleRnd1FlatAttributes.m_Radius);
	WriteProfileInt("HoleRound1Flat","LineWidth",m_HoleRnd1FlatAttributes.m_Width);;

	WriteProfileInt("HoleRect","Height",m_RectHoleAttributes.m_H);
	WriteProfileInt("HoleRect","Width",m_RectHoleAttributes.m_W);
	WriteProfileInt("HoleRect","LineWidth",m_RectHoleAttributes.m_Width);
	WriteProfileInt("HoleRect","LineColor",(int)m_RectHoleAttributes.m_LineColor );

	WriteProfileInt("Text","Color",(int)m_TextAttributes.m_Color );
	WriteProfileInt("Text","BackColor",(int)m_TextAttributes.m_BkColor );
	WriteProfileInt("Text","Angle",m_TextAttributes.m_Angle);
	WriteProfileInt("Text","FontHeight",m_TextAttributes.m_FontHeight);
	WriteProfileInt("Text","Width",m_TextAttributes.m_FontWidth);
	WriteProfileInt("Text","Format",m_TextAttributes.m_Format);
	WriteProfileInt("Text","Transparent",m_TextAttributes.m_Transparent);
	WriteProfileInt("Text","Weight",m_TextAttributes.m_Weight);
	WriteProfileString("Text","Font",m_TextAttributes.m_pFontName);

	WriteProfileInt("SWiz","ArcEnd",m_SCALEWIZattributes.m_ArcEnd);
	WriteProfileInt("SWiz","ArcLineWidth",m_SCALEWIZattributes.m_ArcLineWidth);
	WriteProfileInt("SWiz","ArcRadius",m_SCALEWIZattributes.m_ArcRadius);
	WriteProfileInt("SWiz","ArcStart",m_SCALEWIZattributes.m_ArcStart);
	WriteProfileInt("SWiz","ArcColor",(int)m_SCALEWIZattributes.m_ArcColor );
	WriteProfileInt("SWiz","DivLinLen",m_SCALEWIZattributes.m_DivisionLineLegnth);
	WriteProfileInt("SWiz","DivLinW",m_SCALEWIZattributes.m_DivisionLineWidth);
	WriteProfileInt("SWiz","Divisions",m_SCALEWIZattributes.m_Divisions);
	WriteProfileInt("SWiz","DivColor",(int)m_SCALEWIZattributes.m_DivLineColor );
	WriteProfileInt("SWiz","SubLinLen",m_SCALEWIZattributes.m_SubdivisionLengnth);
	WriteProfileInt("SWiz","SubLinW",m_SCALEWIZattributes.m_SubDivisionLineWidth);
	WriteProfileInt("SWiz","SubDiv",m_SCALEWIZattributes.m_SubDivisions);
	WriteProfileInt("SWiz","SubDivColor",(int)m_SCALEWIZattributes.m_SubDivColor );
	WriteProfileInt("SWiz","FlatDist",m_SCALEWIZattributes.m_FlatDist);
	WriteProfileInt("SWiz","HoleRad",m_SCALEWIZattributes.m_HoleSize);
	WriteProfileInt("SWiz","HoleType",m_SCALEWIZattributes.m_HoleType);
	WriteProfileInt("SWiz","FontSize",m_SCALEWIZattributes.m_FontSize);
	WriteProfileInt("SWiz","Weight",m_SCALEWIZattributes.m_FontWeight);
	WriteProfileInt("SWiz","DistToTick",m_SCALEWIZattributes.m_DistToTick);
	WriteProfileInt("SWiz","TextColor",(int)m_SCALEWIZattributes.m_TextColor );
	WriteProfileInt("SWiz","TextBkColor",(int)m_SCALEWIZattributes.m_BkColor );
	WriteProfileString("SWiz","Font",m_SCALEWIZattributes.m_pFont);
}



void CFrontCadApp::WriteToLogFile(char * pS)
{
	if (m_pLogFile)
		fprintf(m_pLogFile, "%s\n", pS);
}

