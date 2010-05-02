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
	m_iFileType = 0;
	m_csFileName = _T("");
	//}}AFX_DATA_INIT
}


void CSaveDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CSaveDialog)
	DDX_CBIndex(pDX, IDC_COMBO_TYPE, m_iFileType);
	DDX_Text(pDX, IDC_EDIT_FILENAME, m_csFileName);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CSaveDialog, CDialog)
	//{{AFX_MSG_MAP(CSaveDialog)
	ON_BN_CLICKED(IDC_BUTTON_BROWSE, OnButtonBrowse)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSaveDialog message handlers


void CSaveDialog::OnOK() 
{
	// TODO: Add extra validation here
	
	CDialog::OnOK();
}

void CSaveDialog::OnCancel() 
{
	// TODO: Add extra cleanup here
	
	CDialog::OnCancel();
}

void CSaveDialog::OnButtonBrowse() 
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);
	CString csFilter;
	CString csDefaultExt;
	CString csDeafultFileName;
	csDeafultFileName.LoadString(DEFAULT_FILE_NAME);
	switch(m_iFileType)
	{
	case 0:
		csFilter.LoadString(IDS_WINDOWS_BITMAP_FILES);
		csDefaultExt.LoadString(DEFAULT_BITMAP_FILE_EXTENSION);
		//BMP File
		break;
	case 1:
		csFilter.LoadString(IDS_WINDOWS_JPEG_FILES);
		csDefaultExt.LoadString(DEFAULT_JPEG_FILE_EXTENSION);
		//JPG File
		break;
	case 2:
		//PDF File
		csFilter.LoadString(IDS_WINDOWS_PDF_FILES);
		csDefaultExt.LoadString(DEFAULT_PDF_FILE_EXTENSION);
		break;
	case 3:
		csFilter.LoadString(IDS_WINDOWS_TXT_FILES);
		csDefaultExt.LoadString(DEFAULT_TXT_FILE_EXTENSION);
		//Txt File
		break;
	}

	CFileDialog dlg(FALSE, csDefaultExt, csDeafultFileName, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, csFilter, NULL);


	if(dlg.DoModal() == IDOK)  
		
		m_csFileName = dlg.GetPathName();  
	
	UpdateData(FALSE);
}
