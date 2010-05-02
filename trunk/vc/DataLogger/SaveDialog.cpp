// SaveDialog.cpp : implementation file
//

#include "stdafx.h"
#include "datalogger.h"
#include "SaveDialog.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CSaveDialog dialog


CSaveDialog::CSaveDialog(CWnd* pParent /*=NULL*/)
	: CDialog(CSaveDialog::IDD, pParent)
{
	//{{AFX_DATA_INIT(CSaveDialog)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CSaveDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CSaveDialog)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CSaveDialog, CDialog)
	//{{AFX_MSG_MAP(CSaveDialog)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSaveDialog message handlers

