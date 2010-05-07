// ExitDialog.cpp : implementation file
//

#include "stdafx.h"
#include "datalogger.h"
#include "ExitDialog.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CExitDialog dialog


CExitDialog::CExitDialog(CWnd* pParent /*=NULL*/)
	: CDialog(CExitDialog::IDD, pParent)
{
	//{{AFX_DATA_INIT(CExitDialog)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CExitDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CExitDialog)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CExitDialog, CDialog)
	//{{AFX_MSG_MAP(CExitDialog)
		// NOTE: the ClassWizard will add message map macros here
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CExitDialog message handlers
