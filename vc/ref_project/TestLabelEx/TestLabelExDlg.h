// TestLabelExDlg.h : header file
//

#if !defined(AFX_TESTLABELEXDLG_H__B4DBF45C_652E_40B4_B2C6_FFF661E9CC67__INCLUDED_)
#define AFX_TESTLABELEXDLG_H__B4DBF45C_652E_40B4_B2C6_FFF661E9CC67__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

/////////////////////////////////////////////////////////////////////////////
// CTestLabelExDlg dialog
#include "LabelEx.h"
class CTestLabelExDlg : public CDialog
{
// Construction
public:
	CTestLabelExDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	//{{AFX_DATA(CTestLabelExDlg)
	enum { IDD = IDD_TESTLABELEX_DIALOG };
	CLabelEx	m_Label2;
	CLabelEx	m_Label1;
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CTestLabelExDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	//{{AFX_MSG(CTestLabelExDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnStatic1();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_TESTLABELEXDLG_H__B4DBF45C_652E_40B4_B2C6_FFF661E9CC67__INCLUDED_)
