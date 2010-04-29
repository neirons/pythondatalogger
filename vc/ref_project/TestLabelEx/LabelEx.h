#if !defined(AFX_LABELEX_H__C9BE230C_B8E9_48DE_8022_F3D4F5260B65__INCLUDED_)
#define AFX_LABELEX_H__C9BE230C_B8E9_48DE_8022_F3D4F5260B65__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// LabelEx.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CLabelEx window

class CLabelEx : public CStatic
{
// Construction
public:
	CLabelEx();

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CLabelEx)
	protected:
	virtual void PreSubclassWindow();
	//}}AFX_VIRTUAL

// Implementation
public:
	void EnableHighLight(BOOL bHighLight);
	void SetClickedLabelBitmap(UINT ID);
	void SetMouseOverLabelBitmap(UINT ID);
	void SetClickedBGBitmap(UINT ID);
	void SetLabelBitmap(UINT ID);
	void SetMouseOverBGBitmap(UINT ID);
	void SetBGBitmap(UINT ID);
	void RedrawWindow();
	void EnableAutoUnderLine(BOOL bAutoUnderLine);
	void SetBkColor(COLORREF crBkColor);
	void SetBorder(BOOL bBorder,COLORREF crBorder);
	void SetUnderLine(BOOL bUnderLine,COLORREF crUnderLine);
	void SetTextColor(COLORREF crText);
	virtual ~CLabelEx();

	// Generated message map functions
protected:
	HCURSOR m_hHandCur;
	CFont m_Font;
	BOOL m_bBorder;
	BOOL m_bClicked;
	BOOL m_bOver;
	BOOL m_bUnderLine,m_bBackUnderLine;
	BOOL m_bAutoUnderLine;
	BOOL m_bHighLight;
	CBitmap m_BGBm,m_MouseOverBGBm,m_ClickedBGBm;
	CBitmap m_LabelBm,m_MouseOverLabelBm,m_ClickedLabelBm;
	COLORREF m_crText,m_crBorder,m_crBG,m_crUnderLine;
	COLORREF m_crBackText;
	//{{AFX_MSG(CLabelEx)
	afx_msg void OnPaint();
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_LABELEX_H__C9BE230C_B8E9_48DE_8022_F3D4F5260B65__INCLUDED_)
