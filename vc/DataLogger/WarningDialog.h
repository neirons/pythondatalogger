#if !defined(AFX_WARNINGDIALOG_H__C7D0B573_00DB_4958_B6EE_B7C3FEF75B72__INCLUDED_)
#define AFX_WARNINGDIALOG_H__C7D0B573_00DB_4958_B6EE_B7C3FEF75B72__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// WarningDialog.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CWarningDialog dialog

class CWarningDialog : public CDialog
{
// Construction
public:
	CWarningDialog(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CWarningDialog)
	enum { IDD = IDD_WARNING_DIALOG };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CWarningDialog)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CWarningDialog)
		// NOTE: the ClassWizard will add member functions here
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_WARNINGDIALOG_H__C7D0B573_00DB_4958_B6EE_B7C3FEF75B72__INCLUDED_)
