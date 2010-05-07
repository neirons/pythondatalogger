#if !defined(AFX_EXITDIALOG_H__2DBCA340_FF5A_4412_9B79_803121A6F962__INCLUDED_)
#define AFX_EXITDIALOG_H__2DBCA340_FF5A_4412_9B79_803121A6F962__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ExitDialog.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CExitDialog dialog

class CExitDialog : public CDialog
{
// Construction
public:
	CExitDialog(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CExitDialog)
	enum { IDD = IDD_EXIT_DIALOG };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CExitDialog)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CExitDialog)
		// NOTE: the ClassWizard will add member functions here
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_EXITDIALOG_H__2DBCA340_FF5A_4412_9B79_803121A6F962__INCLUDED_)
