#if !defined(AFX_DATALOGGERGRAPH_H__2D7B84CA_DCBE_4AC9_BB03_5CC0D66300AD__INCLUDED_)
#define AFX_DATALOGGERGRAPH_H__2D7B84CA_DCBE_4AC9_BB03_5CC0D66300AD__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DataLoggerGraph.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CGraphStatic window

class CGraphStatic : public CStatic
{
// Construction
public:
	CGraphStatic();

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CGraphStatic)
	//}}AFX_VIRTUAL

// Implementation
public:
	void SetMaxMin(double max,double min);

	void SetData(int days,double* pData,int maxpoint,int totalpoint,double average);
	virtual ~CGraphStatic();

	// Generated message map functions
protected:
	//{{AFX_MSG(CGraphStatic)
	afx_msg void OnPaint();
	//}}AFX_MSG
private:
	void GetPoint(double &x,double &y);
	int round(double   f);
	void DrawGridAndText(CPaintDC& dc);
	void DrawData(CPaintDC& dc);
	void DrawPixcel(CDC& memdc,double x, double y);
	
	int GetXAxisCharsIndex();

	int m_LeftOffset;
	int m_RightOffset;
	int m_TopOffset;
	int m_BottomOffset;

	int m_XGrid ;
	int m_YGrid;

	int m_xpixel;
	int m_ypixel;

	int m_Days;

	double* m_pData;
	int m_TotalPoint;
    int m_MaxPoint;
	double m_Average;
    double m_Max;
    double m_Min;

	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DATALOGGERGRAPH_H__2D7B84CA_DCBE_4AC9_BB03_5CC0D66300AD__INCLUDED_)
