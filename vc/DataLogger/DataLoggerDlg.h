// DataLoggerDlg.h : header file
//

#if !defined(AFX_DATALOGGERDLG_H__22CD8E80_5880_42C7_9132_320F97ECCF44__INCLUDED_)
#define AFX_DATALOGGERDLG_H__22CD8E80_5880_42C7_9132_320F97ECCF44__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

/////////////////////////////////////////////////////////////////////////////
// CDataLoggerDlg dialog
#include "GraphStatic.h"
#include "BatteryStatic.h"
#define MAX_POINT 20000

#pragma comment(lib, ".\\PDFLib\\PDFLib.lib")

class CDataLoggerDlg : public CDialog
{
// Construction
public:
	CDataLoggerDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	//{{AFX_DATA(CDataLoggerDlg)
	enum { IDD = IDD_DATALOGGER_DIALOG };
	CBatteryStatic	m_Battery;
	CGraphStatic	m_Graph;
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDataLoggerDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL

	UINT m_cxWidth;
	UINT m_cxOffset;
	UINT m_nLinesPerPage;
	UINT m_cyPrinter;
	CFont		m_ListFont;
	CFont		m_fontPrinter;
	double m_Data[MAX_POINT];
	int m_TotalPoint;
	double m_Average;

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
	afx_msg void OnButtonClear();
	afx_msg void OnPrint();
	afx_msg void OnPrintPreview();
	virtual void OnCancel();
	//}}AFX_MSG
	LRESULT OnBeginPrinting(WPARAM wParam,LPARAM lParam);
	LRESULT OnEndPrinting(WPARAM wParam,LPARAM lParam);
	LRESULT OnMyPrint(WPARAM wParam,LPARAM lParam);

	DECLARE_MESSAGE_MAP()
private:
	void SaveToTxtFile(CString txtfilename);
	int  SaveToPDFFile(CString pdfillename,CString cstempjpgfile);



	int  GetEncoderClsid(const   WCHAR*   format,   CLSID*   pClsid)  ;
	void SaveBitmapToFile(CBitmap& bitmapGraph,int ifiletype,CString csFileName);
	void SaveWindowToBitmap(CWnd *pWnd, int left, int top,CBitmap& bitmapGraph);
	CBitmap m_SaveGraph;


};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DATALOGGERDLG_H__22CD8E80_5880_42C7_9132_320F97ECCF44__INCLUDED_)
