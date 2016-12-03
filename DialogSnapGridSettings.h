#if !defined(AFX_DIALOGSNAPGRIDSETTINGS_H__96B31817_8990_40F6_B78C_A4E5C8301FD3__INCLUDED_)
#define AFX_DIALOGSNAPGRIDSETTINGS_H__96B31817_8990_40F6_B78C_A4E5C8301FD3__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DialogSnapGridSettings.h : header file
//
#include "EditDecimal.h"

/////////////////////////////////////////////////////////////////////////////
// CDialogSnapGridSettings dialog

class CDialogSnapGridSettings : public CDialog
{
// Construction
public:
	CDialogSnapGridSettings(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CDialogSnapGridSettings)
	enum { IDD = IDD_DIALOG_SNAPGRIDSETTINGS };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDialogSnapGridSettings)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDialogSnapGridSettings)
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DIALOGSNAPGRIDSETTINGS_H__96B31817_8990_40F6_B78C_A4E5C8301FD3__INCLUDED_)
