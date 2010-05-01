// DataLoggerDlg.h : header file
//

#if !defined(AFX_DATALOGGERDLG_H__22CD8E80_5880_42C7_9132_320F97ECCF44__INCLUDED_)
#define AFX_DATALOGGERDLG_H__22CD8E80_5880_42C7_9132_320F97ECCF44__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

/////////////////////////////////////////////////////////////////////////////
// CDataLoggerDlg dialog
#include "DataLoggerGraph.h"
#include "BatteryLabel.h"
#define DEFAULT_BITMAP_FILE_EXTENSION						_T("bmp")
#define DEFAULT_BITMAP_FILE_NAME							_T("My Drawing")

class CDataLoggerDlg : public CDialog
{
// Construction
public:
	CDataLoggerDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	//{{AFX_DATA(CDataLoggerDlg)
	enum { IDD = IDD_DATALOGGER_DIALOG };
	CBatteryLabel	m_Battery;
	CDataLoggerGraph	m_Graph;
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDataLoggerDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	//{{AFX_MSG(CDataLoggerDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnButtonSave();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
private:
	BOOL WriteWindowToDIB(LPTSTR szFile, CWnd *pWnd);

	HANDLE DDBToDIB(CBitmap &bitmap, DWORD dwCompression, CPalette *pPal);

	HANDLE CDataLoggerDlg::GetWindowDIB(CWnd *pWnd);
	BOOL WriteDIB( LPTSTR szFile, HANDLE hDIB)  ;

};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DATALOGGERDLG_H__22CD8E80_5880_42C7_9132_320F97ECCF44__INCLUDED_)
