// DialogSnapGridSettings.cpp : implementation file
//

#include "stdafx.h"
#include "frontcad.h"
#include "DialogSnapGridSettings.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDialogSnapGridSettings dialog


CDialogSnapGridSettings::CDialogSnapGridSettings(CWnd* pParent /*=NULL*/)
	: CDialog(CDialogSnapGridSettings::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDialogSnapGridSettings)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CDialogSnapGridSettings::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDialogSnapGridSettings)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDialogSnapGridSettings, CDialog)
	//{{AFX_MSG_MAP(CDialogSnapGridSettings)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDialogSnapGridSettings message handlers

BOOL CDialogSnapGridSettings::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CDialogSnapGridSettings::OnOK() 
{
	// TODO: Add extra validation here
	
	CDialog::OnOK();
}
