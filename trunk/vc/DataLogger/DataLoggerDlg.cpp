// DataLoggerDlg.cpp : implementation file
//

#include "stdafx.h"
#include "DataLogger.h"
#include "DataLoggerDlg.h"
#include <fstream.h>
#include <iostream.h>
#include <conio.h>
#include "SaveDialog.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CAboutDlg dialog used for App About

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// Dialog Data
	//{{AFX_DATA(CAboutDlg)
	enum { IDD = IDD_ABOUTBOX };
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAboutDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	//{{AFX_MSG(CAboutDlg)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
	//{{AFX_DATA_INIT(CAboutDlg)
	//}}AFX_DATA_INIT
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAboutDlg)
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
	//{{AFX_MSG_MAP(CAboutDlg)
		// No message handlers
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDataLoggerDlg dialog

CDataLoggerDlg::CDataLoggerDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CDataLoggerDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDataLoggerDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	// Note that LoadIcon does not require a subsequent DestroyIcon in Win32
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CDataLoggerDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDataLoggerDlg)
	DDX_Control(pDX, IDC_BATTERY, m_Battery);
	DDX_Control(pDX, IDC_STATIC_GRAPH, m_Graph);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CDataLoggerDlg, CDialog)
	//{{AFX_MSG_MAP(CDataLoggerDlg)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BUTTON_SAVE, OnButtonSave)
	ON_BN_CLICKED(IDC_BUTTON_PRINT, OnButtonPrint)
	ON_BN_CLICKED(IDC_BUTTON_CLEAR, OnButtonClear)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDataLoggerDlg message handlers

BOOL CDataLoggerDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// Add "About..." menu item to system menu.

	// IDM_ABOUTBOX must be in the system command range.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		CString strAboutMenu;
		strAboutMenu.LoadString(IDS_ABOUTBOX);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon
	
	m_Battery.SetBatteryLevel(16,1);
	m_Graph.SetDays(20);

//	m_Battery.MoveWindow(rect.left,rect.top,rect.Width(),rect.Height());
	// TODO: Add extra initialization here
	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CDataLoggerDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialog::OnSysCommand(nID, lParam);
	}
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CDataLoggerDlg::OnPaint() 
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, (WPARAM) dc.GetSafeHdc(), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

// The system calls this to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CDataLoggerDlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}

void CDataLoggerDlg::OnButtonSave() 
{
	// TODO: Add your control notification handler code here
	
	CSaveDialog dlg;
	int iFileType;
	CString csFileName;

	CBitmap bitmapGraph;
	SaveWindowToBitmap((CWnd*)this, bitmapGraph);

	if(IDOK == dlg.DoModal())
	{
		iFileType = dlg.m_iFileType;
		csFileName = dlg.m_csFileName;
		switch(iFileType)
		{
		case 0:
		case 1:
			SaveBitmapToFile(bitmapGraph,iFileType,csFileName);
			break;
		case 2:
			//PDF file
			break;
		case 3:
			//Txt file
			break;
		default:
			TRACE("unknow error");
		}

	}
}


 



 



void CDataLoggerDlg::OnButtonPrint() 
{
	// TODO: Add your control notification handler code here
}


int   CDataLoggerDlg::GetEncoderClsid(const   WCHAR*   format,   CLSID*   pClsid)  
  {  
        UINT     num   =   0;                     //   number   of   image   encoders  
        UINT     size   =   0;                   //   size   of   the   image   encoder   array   in   bytes  
   
        ImageCodecInfo*   pImageCodecInfo   =   NULL;  
   
        GetImageEncodersSize(&num,   &size);  
        if(size   ==   0)  
              return   -1;     //   Failure  
   
        pImageCodecInfo   =   (ImageCodecInfo*)(malloc(size));  
        if(pImageCodecInfo   ==   NULL)  
              return   -1;     //   Failure  
   
        GetImageEncoders(num,   size,   pImageCodecInfo);  
   
        for(UINT   j   =   0;   j   <   num;   ++j)  
        {  
              if(   wcscmp(pImageCodecInfo[j].MimeType,   format)   ==   0   )  
              {  
                    *pClsid   =   pImageCodecInfo[j].Clsid;  
                    free(pImageCodecInfo);  
                    return   j;     //   Success  
              }          
        }  
   
        free(pImageCodecInfo);  
        return   -1;     //   Failure  
  }   

void CDataLoggerDlg::OnButtonClear() 
{
}


void CDataLoggerDlg::SaveWindowToBitmap(CWnd *pWnd, CBitmap& bitmapGraph)
{

    CClientDC dc(pWnd);

    CDC  m_dcGraph;

    m_dcGraph.CreateCompatibleDC(&dc);
	CRect rect;
	pWnd->GetClientRect(&rect);
    bitmapGraph.CreateCompatibleBitmap(&dc,rect.Width(),rect.Height());
    m_dcGraph.SelectObject(&bitmapGraph);
    m_dcGraph.BitBlt(0,0,rect.Width(),rect.Height(),&dc,0,0,SRCCOPY);	
}

void CDataLoggerDlg::SaveBitmapToFile(CBitmap& bitmapGraph,int ifiletype,CString csFileName)
{
	HBITMAP   hBitmap;
    hBitmap=(HBITMAP)bitmapGraph.GetSafeHandle();
	
	CLSID encoderClsid;
	Bitmap bitmap(hBitmap,NULL);
	
	switch(ifiletype)
	{
	case 0:
	     GetEncoderClsid(L"image/bmp", &encoderClsid);
		break;
	case 1:
		GetEncoderClsid(L"image/jpeg", &encoderClsid);
		break;


	}

	EncoderParameters encoderPara;
	encoderPara.Count=1;
	encoderPara.Parameter[0].Guid=EncoderQuality;
	encoderPara.Parameter[0].Type=EncoderParameterValueTypeLong;
	encoderPara.Parameter[0].NumberOfValues=1;
	ULONG quality=100;
	encoderPara.Parameter[0].Value=&quality;
	
	bitmap.Save(csFileName.AllocSysString(),&encoderClsid,&encoderPara);
	
}