// LibFormView.cpp : implementation file
//

#include "stdafx.h"
#include "FrontCad.h"
#include "LibFormView.h"
#include <stdio.h>
#include "FileParser.h"
#include "NewLibDialog.h"
#include "FrontCadDefines.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CLibFormView

IMPLEMENT_DYNCREATE(CLibFormView, CFormView)

CLibFormView::CLibFormView()
	: CFormView(CLibFormView::IDD)
{
	//{{AFX_DATA_INIT(CLibFormView)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_pHead = 0;
	m_pTail = 0;
	((CFrontCadApp *)AfxGetApp())->m_pLibView = this;
}

CLibFormView::~CLibFormView()
{
}

void CLibFormView::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CLibFormView)
	DDX_Control(pDX, IDC_COMBO_LIBVIEW_COMPONENT, m_Combo_SelPart);
	DDX_Control(pDX, IDC_COMBO_LIBVIEW_LIBRARY, m_Combo_SelLIb);
	DDX_Control(pDX, IDC_STATIC_LIBVIEW_PREVIEW, m_Static_Preview);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CLibFormView, CFormView)
	//{{AFX_MSG_MAP(CLibFormView)
	ON_BN_CLICKED(IDC_BUTTON_LIBVIEW_CREATECOMP, OnButtonLibviewCreatecomp)
	ON_BN_CLICKED(IDC_BUTTON_LIBVIEW_PLACE, OnButtonLibviewPlace)
	ON_BN_CLICKED(IDC_BUTTON_LIBVIEW_NEWLIB, OnButtonLibviewNewlib)
	ON_BN_CLICKED(IDC_BUTTON_ADDLIBRARY, OnButtonAddlibrary)
	ON_BN_CLICKED(IDC_BUTTON_LIBVIEW_REMOVE, OnButtonLibviewRemove)
	ON_BN_CLICKED(IDC_BUTTON_LIBVIEW_DELETEPART, OnButtonLibviewDeletepart)
	ON_CBN_SELCHANGE(IDC_COMBO_LIBVIEW_COMPONENT, OnSelchangeComboLibviewComponent)
	ON_CBN_SELCHANGE(IDC_COMBO_LIBVIEW_LIBRARY, OnSelchangeComboLibviewLibrary)
	ON_CBN_DBLCLK(IDC_COMBO_LIBVIEW_COMPONENT, OnDblclkComboLibviewComponent)
	ON_BN_CLICKED(IDC_BUTTON_LIBVIEW_SAVELIBRARY, OnButtonLibviewSavelibrary)
	//}}AFX_MSG_MAP
	ON_MESSAGE(WM_MAINVIEW_GOTREF,OnMainViewGotReference)
	ON_COMMAND(ID_VIEW_RENDERENABLE, &CLibFormView::OnViewRenderenable)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CLibFormView diagnostics

#ifdef _DEBUG
void CLibFormView::AssertValid() const
{
	CFormView::AssertValid();
}

void CLibFormView::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CLibFormView message handlers

void CLibFormView::OnButtonLibviewCreatecomp() 
{
	// TODO: Add your control notification handler code here
	MessageBox("Select Reference Point","Hey Dude!");
	CFrontCadView *pFCV = ((CFrontCadApp *)AfxGetApp())->m_pMainView;
	pFCV->SendMessage(WM_LIBVIEW_GETREF);
}

void CLibFormView::OnButtonLibviewPlace() 
{
	// TODO: Add your control notification handler code here
	
}

void CLibFormView::AddLibrary(CCadLibrary *pL)
{
	if(m_pHead == 0)
	{
		m_pHead = pL;
		m_pTail = pL;
	}
	else
	{
		m_pTail->SetNext(pL);
		pL->SetPrev(m_pTail);
		m_pTail = pL;
	}
	this->m_Combo_SelLIb.SetLibChain(this->m_pHead);
	this->m_Combo_SelLIb.SetSelectionByName(pL->GetName());
	this->m_Combo_SelPart.SetLibraryFile(pL);
	this->m_Combo_SelPart.SetCurSel(0);
	CCadLibObject *pPart = GetCurrentPart();
	if(pPart)
	{
		m_Static_Preview.m_pLibObj = pPart;
		m_Static_Preview.Invalidate();
	}
}

void CLibFormView::DeleteLibrary(CCadLibrary *pL)
{
	if(pL == m_pHead)
	{
		m_pHead = pL->GetNext();
		m_pHead->SetPrev(0);
	}
	else if(pL == m_pTail)
	{
		m_pTail = m_pTail->GetPrev();
		m_pTail->SetNext(0);
	}
	else
	{
		pL->GetPrev()->SetNext(pL->GetNext());
		pL->GetNext()->SetPrev(pL->GetPrev());
	}
	delete pL;
}

void CLibFormView::OnButtonLibviewNewlib() 
{
	// TODO: Add your control notification handler code here
	
}

void CLibFormView::OnButtonAddlibrary() 
{
	// TODO: Add your control notification handler code here
	CFileDialog dlg(TRUE);
	if(dlg.DoModal() == IDOK)
	{
		CString fname = dlg.GetFileName();
		FILE *pIN = fopen(LPCTSTR(fname),"r");
		CFileParser libfile;
		libfile.Parse(pIN);
		AddLibrary(libfile.GetLib());
	}
}

void CLibFormView::OnButtonLibviewRemove() 
{
	// TODO: Add your control notification handler code here
	CCadLibrary *pL = this->GetCurrentLibrary();
	DeleteLibrary(pL);
	this->m_Combo_SelLIb.SetLibChain(m_pHead);
}

void CLibFormView::OnButtonLibviewDeletepart() 
{
	// TODO: Add your control notification handler code here
	CCadLibObject *pLO = this->GetCurrentPart();
	CCadLibrary *pLib = this->GetCurrentLibrary();
	if(pLO && pLib) pLib->DeletePart(pLO);
	this->m_Combo_SelPart.SetLibraryFile(pLib);
}

void CLibFormView::OnInitialUpdate() 
{
	CFormView::OnInitialUpdate();
	
	// TODO: Add your specialized code here and/or call the base class
	
}

void CLibFormView::OnSelchangeComboLibviewComponent() 
{
	// TODO: Add your control notification handler code here
	CCadLibObject *pPart = GetCurrentPart();
	this->m_Static_Preview.m_pLibObj = pPart;
	m_Static_Preview.Invalidate();
}

void CLibFormView::OnDblclkComboLibviewComponent() 
{
	// TODO: Add your control notification handler code here
	
}

void CLibFormView::OnSelchangeComboLibviewLibrary() 
{
	// TODO: Add your control notification handler code here
	CCadLibrary *pL = GetCurrentLibrary();
	this->m_Combo_SelPart.SetLibraryFile(pL);
	this->m_Combo_SelPart.SetCurSel(0);
	CCadLibObject *pPart = GetCurrentPart();
	if(pPart)
	{
		this->m_Static_Preview.m_pLibObj = pPart;
		m_Static_Preview.Invalidate();
	}
}

void CLibFormView::AddPartToCurLib(CCadLibObject *pObj)
{
	CCadLibrary *pLib = GetCurrentLibrary();
	pLib->AddPart(pObj);
	m_Combo_SelPart.SetLibraryFile(pLib);
	this->m_Combo_SelPart.SetCurSel(0);
	CCadLibObject *pPart = GetCurrentPart();
	if(pPart)
	{
		this->m_Static_Preview.m_pLibObj = pPart;
		m_Static_Preview.Invalidate();
	}
}

CCadLibrary * CLibFormView::GetCurrentLibrary()
{
	int Sel = this->m_Combo_SelLIb.GetCurSel();
	char *LibName = (char *)m_Combo_SelLIb.GetItemDataPtr(Sel);
	CCadLibrary *pL = this->m_pHead;
	int loop = 1;
	while(pL && loop)
	{
		if(strcmp(LibName,pL->GetName()) == 0)
			loop = 0;
		else
			pL = pL->GetNext();
	}
	return pL;
}

CCadLibObject * CLibFormView::GetCurrentPart()
{
	int loop = 1;
	CCadLibObject *pPart = NULL;

	int Sel = this->m_Combo_SelPart.GetCurSel();
	if(Sel >= 0)
	{
	char *pPartName = (char *)m_Combo_SelPart.GetItemDataPtr(Sel);
	CCadLibrary *pLib = GetCurrentLibrary();
	pPart = pLib->GetHeadObj();
	while(pPart && loop)
	{
		if(strcmp(pPartName,pPart->m_pName) == 0) loop = 0;
		else pPart = (CCadLibObject *)pPart->m_pNext;
	}
	}
	return pPart;
}

void CLibFormView::OnButtonLibviewSavelibrary() 
{
	// TODO: Add your control notification handler code here
	CCadLibrary *pLib = GetCurrentLibrary();
	CFileDialog dlg(FALSE);

	dlg.DoModal();

	CString FileName = dlg.GetFileName();
	FILE *pO = fopen(LPCTSTR(FileName),"w");
	pLib->Save(pO);
}



LRESULT CLibFormView::OnMainViewGotReference(WPARAM wP, LPARAM lP)
{
	//----------------------------------
	// Ref.x is in wP, Ref.y is in lP
	//----------------------------------
	CPoint Ref;
	Ref.x = wP;
	Ref.y = lP;
	CNewLibDialog dlg;
	dlg.m_pCaption = "New Part Name";
	dlg.DoModal();

	CFrontCadDoc *pDoc = ((CFrontCadApp *)AfxGetApp())->m_pDoc;
	CCadLibObject *pPart = pDoc->m_pDrawing->CreatePartFromSelected(dlg.m_LibName);
	CCadObject *pObj = pPart->GetHead();
	while(pObj)
	{
		pObj->AdjustRefernce(Ref);
		pObj= pObj->m_pNext;
	}
	this->AddPartToCurLib(pPart);
	return 0;
}


void CLibFormView::OnViewRenderenable()
{
	// TODO: Add your command handler code here
}
