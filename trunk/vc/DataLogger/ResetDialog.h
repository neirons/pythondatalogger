#if !defined(AFX_RESETDIALOG_H__C4819707_C8BB_4156_923E_CCBBD503E670__INCLUDED_)
#define AFX_RESETDIALOG_H__C4819707_C8BB_4156_923E_CCBBD503E670__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ResetDialog.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CResetDialog dialog

class CResetDialog : public CDialog
{
// Construction
public:
	CResetDialog(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CResetDialog)
	enum { IDD = IDD_RESET_DIALOG };
	int		m_iUTC;
	int		m_iMinute;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CResetDialog)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CResetDialog)
		// NOTE: the ClassWizard will add member functions here
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_RESETDIALOG_H__C4819707_C8BB_4156_923E_CCBBD503E670__INCLUDED_)
