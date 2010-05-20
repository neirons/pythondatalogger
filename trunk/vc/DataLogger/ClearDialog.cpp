// ClearDialog.cpp : implementation file
//

#include "stdafx.h"
#include "datalogger.h"
#include "ClearDialog.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CClearDialog dialog


CClearDialog::CClearDialog(CWnd* pParent /*=NULL*/)
	: CDialog(CClearDialog::IDD, pParent)
{
	//{{AFX_DATA_INIT(CClearDialog)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CClearDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CClearDialog)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CClearDialog, CDialog)
	//{{AFX_MSG_MAP(CClearDialog)
		// NOTE: the ClassWizard will add message map macros here
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CClearDialog message handlers
