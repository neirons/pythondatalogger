#if !defined(AFX_SAVEDIALOG_H__1B5BE068_D183_45D6_94A8_E7DE4D3AAE17__INCLUDED_)
#define AFX_SAVEDIALOG_H__1B5BE068_D183_45D6_94A8_E7DE4D3AAE17__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// SaveDialog.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CSaveDialog dialog

class CSaveDialog : public CDialog
{
// Construction
public:
	CSaveDialog(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CSaveDialog)
	enum { IDD = IDD_SAVE_DIALOG };
	int		m_iFileType;
	CString	m_csFileName;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSaveDialog)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CSaveDialog)
	virtual void OnOK();
	virtual void OnCancel();
	afx_msg void OnButtonBrowse();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SAVEDIALOG_H__1B5BE068_D183_45D6_94A8_E7DE4D3AAE17__INCLUDED_)
