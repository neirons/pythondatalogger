#if !defined(AFX_BATTERYLABEL_H__E85E18D5_DE3B_4C02_83C8_CD1BAAA96E92__INCLUDED_)
#define AFX_BATTERYLABEL_H__E85E18D5_DE3B_4C02_83C8_CD1BAAA96E92__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// BatteryLabel.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CBatteryLabel window

class CBatteryLabel : public CStatic
{
// Construction
public:
	CBatteryLabel();

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CBatteryLabel)
	//}}AFX_VIRTUAL

// Implementation
public:
	void SetBatteryLevel(int maxlevel,int batterylevel);
	virtual ~CBatteryLabel();

	// Generated message map functions
protected:
	//{{AFX_MSG(CBatteryLabel)
	afx_msg void OnPaint();
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
private:
	int m_BatteryLevel;
	int m_MaxLevel;
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_BATTERYLABEL_H__E85E18D5_DE3B_4C02_83C8_CD1BAAA96E92__INCLUDED_)
