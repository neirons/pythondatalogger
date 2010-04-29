// TestLabelEx.h : main header file for the TESTLABELEX application
//

#if !defined(AFX_TESTLABELEX_H__D9BBDD5C_D138_4ACE_9ADD_B26E1F1C4FE3__INCLUDED_)
#define AFX_TESTLABELEX_H__D9BBDD5C_D138_4ACE_9ADD_B26E1F1C4FE3__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols

/////////////////////////////////////////////////////////////////////////////
// CTestLabelExApp:
// See TestLabelEx.cpp for the implementation of this class
//

class CTestLabelExApp : public CWinApp
{
public:
	CTestLabelExApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CTestLabelExApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// Implementation

	//{{AFX_MSG(CTestLabelExApp)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_TESTLABELEX_H__D9BBDD5C_D138_4ACE_9ADD_B26E1F1C4FE3__INCLUDED_)
