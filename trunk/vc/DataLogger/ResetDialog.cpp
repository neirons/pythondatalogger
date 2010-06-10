// ResetDialog.cpp : implementation file
//

#include "stdafx.h"
#include "datalogger.h"
#include "ResetDialog.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CResetDialog dialog


CResetDialog::CResetDialog(CWnd* pParent /*=NULL*/)
	: CDialog(CResetDialog::IDD, pParent)
{
	//{{AFX_DATA_INIT(CResetDialog)
	m_iUTC = 24;
	m_iMinute = 5;
	//}}AFX_DATA_INIT
}


void CResetDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CResetDialog)
	DDX_Text(pDX, IDC_EDIT_UTC, m_iUTC);
	DDX_Text(pDX, IDC_EDIT_MINIUTE, m_iMinute);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CResetDialog, CDialog)
	//{{AFX_MSG_MAP(CResetDialog)
		// NOTE: the ClassWizard will add message map macros here
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CResetDialog message handlers
