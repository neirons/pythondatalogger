// DataLogger.h : main header file for the DATALOGGER application
//

#if !defined(AFX_DATALOGGER_H__D112641D_D5B8_40E2_B7ED_DD5EB6031642__INCLUDED_)
#define AFX_DATALOGGER_H__D112641D_D5B8_40E2_B7ED_DD5EB6031642__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols

/////////////////////////////////////////////////////////////////////////////
// CDataLoggerApp:
// See DataLogger.cpp for the implementation of this class
//

class CDataLoggerApp : public CWinApp
{
public:
	CDataLoggerApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDataLoggerApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// Implementation

	//{{AFX_MSG(CDataLoggerApp)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DATALOGGER_H__D112641D_D5B8_40E2_B7ED_DD5EB6031642__INCLUDED_)
