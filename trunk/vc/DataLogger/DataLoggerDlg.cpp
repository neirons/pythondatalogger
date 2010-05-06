// DataLoggerDlg.cpp : implementation file
//

#include "stdafx.h"
#include "DataLogger.h"
#include "DataLoggerDlg.h"
#include <fstream.h>
#include <iostream.h>
#include <conio.h>
#include "SaveDialog.h"

#include "PrintFrame.h"
#include "PrintView.h"


#include <stdio.h>

#include <iostream>

#include ".\PDFLib\PDFLib.hpp"


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
	ON_BN_CLICKED(IDC_BUTTON_CLEAR, OnButtonClear)
	ON_BN_CLICKED(IDC_PRINT, OnPrint)
	ON_BN_CLICKED(IDC_PRINT_PREVIEW, OnPrintPreview)
	//}}AFX_MSG_MAP
	ON_MESSAGE(WM_BEGIN_PRINTING,OnBeginPrinting)
	ON_MESSAGE(WM_END_PRINTING,OnEndPrinting)
	ON_MESSAGE(WM_MY_PRINT,OnMyPrint)

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
	
	for(int i =0;i<2000;i++)
		m_Data[i] = 33.5+i*0.0123;

	m_Battery.SetBatteryLevel(16,1);
	int Days = 20;
	m_Graph.SetData(20,m_Data);

	CString csHour;
	csHour.Format("%d",Days*24);
	GetDlgItem(IDC_HOURS)->SetWindowText(csHour);

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
	CString csTempFileName;
	CRect rect_report;

	//Get the report position
	GetDlgItem(IDC_STATIC_REPORT)->GetWindowRect(&rect_report);  
	ScreenToClient(&rect_report);

	SaveWindowToBitmap((CWnd*)this, 0,rect_report.top - 5,m_SaveGraph);
	
	CBitmap PdfGraph;
	SaveWindowToBitmap((CWnd*)&m_Graph, 0,0,PdfGraph);

	if(IDOK == dlg.DoModal())
	{
		iFileType = dlg.m_iFileType;
		csFileName = dlg.m_csFileName;
		switch(iFileType)
		{
		case 0:
		case 1:
			SaveBitmapToFile(m_SaveGraph,iFileType,csFileName);
			break;
		case 2:
//First save the  temp jpge file
			csTempFileName = csFileName + ".jpg";
			SaveBitmapToFile(PdfGraph,1,csTempFileName);
			SaveToPDFFile(csFileName,csTempFileName);
			//Remove the tempfile??
			DeleteFile(csTempFileName);
			//PDF file
			break;
		case 3:
			//Txt file
			break;
		default:
			TRACE("unknow error");
		}

	}

	m_SaveGraph.Detach();  
	m_SaveGraph.DeleteObject(); 

	PdfGraph.Detach();  
	PdfGraph.DeleteObject(); 

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


void CDataLoggerDlg::SaveWindowToBitmap(CWnd *pWnd, int left, int top,CBitmap& bitmapGraph)
{

    CClientDC dc(pWnd);

    CDC  m_dcGraph;

    m_dcGraph.CreateCompatibleDC(&dc);
	CRect rect;
	pWnd->GetClientRect(&rect);
    bitmapGraph.CreateCompatibleBitmap(&dc,rect.Width() - left,rect.Height() - top);
    m_dcGraph.SelectObject(&bitmapGraph);
    m_dcGraph.BitBlt(0,0,rect.Width() - left,rect.Height() - top,&dc,left,top,SRCCOPY);	
	m_dcGraph.DeleteDC();
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

void CDataLoggerDlg::OnPrint() 
{
	// TODO: Add your control notification handler code here
	CRect rect_report;

	//Get the report position
	GetDlgItem(IDC_STATIC_REPORT)->GetWindowRect(&rect_report);  
	ScreenToClient(&rect_report);
	SaveWindowToBitmap((CWnd*)this, 0,rect_report.top - 5,m_SaveGraph);

	CPrintFrame *pFrame = new CPrintFrame;
	pFrame->m_pCallerDlg = this;
	pFrame->Create(NULL,"Curve Print",WS_OVERLAPPEDWINDOW,CRect(0,0,0,0));
	pFrame->m_pView->OnMyPrint();

	m_SaveGraph.Detach();  
	m_SaveGraph.DeleteObject(); 

}

void CDataLoggerDlg::OnPrintPreview() 
{
	// TODO: Add your control notification handler code here
	CRect rect_report;

	//Get the report position
	GetDlgItem(IDC_STATIC_REPORT)->GetWindowRect(&rect_report);  
	ScreenToClient(&rect_report);

	SaveWindowToBitmap((CWnd*)this, 0,rect_report.top - 5,m_SaveGraph);

	CPrintFrame *pFrame = new CPrintFrame;
	pFrame->m_pCallerDlg = this;
	pFrame->Create(NULL,"Curve Print Preview",WS_OVERLAPPEDWINDOW,CRect(0,0,0,0));
	pFrame->m_pView->OnMyPrintPreview();

	m_SaveGraph.Detach();  
	m_SaveGraph.DeleteObject(); 

}



LRESULT CDataLoggerDlg::OnBeginPrinting(WPARAM wParam,LPARAM lParam)
{

	CDC* pDC			= (CDC*)wParam;
	CPrintInfo* pInfo	= (CPrintInfo *)lParam;
	if(m_fontPrinter.m_hObject==NULL)
		m_fontPrinter.CreatePointFont(120,"FixedSys",pDC);
    TEXTMETRIC tm;
    CFont* pOldFont = pDC->SelectObject (&m_fontPrinter);
    pDC->GetTextMetrics (&tm);
    m_cyPrinter = tm.tmHeight + tm.tmExternalLeading;
    CSize size = pDC->GetTextExtent (_T ("---------1---------2---------" \
        "3---------4---------5---------6---------7---------8---"), 81);
    pDC->SelectObject (pOldFont);
    m_cxWidth = size.cx;

    m_nLinesPerPage = (pDC->GetDeviceCaps (VERTRES) -
        (m_cyPrinter * (3 + (2 * PRINTMARGIN)))) / m_cyPrinter;
	pInfo->SetMaxPage (1);

    m_cxOffset = (pDC->GetDeviceCaps (HORZRES) - size.cx) / 2;
	return TRUE;
}
LRESULT CDataLoggerDlg::OnEndPrinting(WPARAM wParam,LPARAM lParam)
{
	if(m_fontPrinter.m_hObject!=NULL)
		m_fontPrinter.DeleteObject ();
	return TRUE;
}
LRESULT CDataLoggerDlg::OnMyPrint(WPARAM wParam,LPARAM lParam)
{

	UINT gl_uNumOfPoints = 52;
	CDC* pDC			= (CDC*)wParam;
	CPrintInfo* pInfo	= (CPrintInfo *)lParam;
	int nPageNumber = pInfo->m_nCurPage;


	if(nPageNumber==1)
	{

		CRect rect_report;
		//Get the report position
		GetDlgItem(IDC_STATIC_REPORT)->GetWindowRect(&rect_report);  
		ScreenToClient(&rect_report);
		CRect rect;
		GetClientRect(&rect);


		int pix_x = rect.Width();
		int pix_y = (rect.Height() - rect_report.top);


		double dScale=(double)m_cxWidth/pix_x;
		int nScaledWidth=m_cxWidth;
		int nScaledHeight=(int)(pix_y*dScale);


		
		CDC memdc;
		memdc.CreateCompatibleDC(pDC);
		CBitmap   *pOldBMP   =  memdc.SelectObject(&m_SaveGraph);   

		
		int nVertCenterPos = pDC->GetDeviceCaps (VERTRES) / 2;
		pDC->StretchBlt(m_cxOffset,(4 + PRINTMARGIN)*m_cyPrinter,nScaledWidth,nScaledHeight,&memdc,0,0,pix_x,pix_y,SRCCOPY);
		
		memdc.SelectObject(pOldBMP);


		memdc.DeleteDC();

	}
	return TRUE;
}

int  CDataLoggerDlg::SaveToPDFFile(CString pdfillename,CString cstempjpgfile)
{

	try
	{
		PDFlib pdf;
		int left_offset = 20;
		int text_offset_y = 100;
		int text_line_height = 25;
		//Set the  parameter 
		pdf.set_parameter("compatibility", "1.4");	

		// Open the file 
		if(pdf.open((LPCSTR)pdfillename) == -1)
			throw("Open pdf file name error");

		// setup the document info
		pdf.set_info("Creator", "Dickson");
		pdf.set_info("Author", "Dickson");
		pdf.set_info("Title", "Dickson Datalogger");
		pdf.set_info("Subject", "DataLogger");
		pdf.set_info("Keywords", "datalogger,dickson");

		// using the A4 page
		double iwidth = a4_height;
		double iheight = a4_width;
		pdf.begin_page(iwidth, iheight);

		// Set the font 
		int font_song = pdf.findfont("Arial", "winansi", 1);
		pdf.setfont(font_song, 18);

		// Set the start point 
//		pdf.set_text_pos(18, iheight - 40);

		// Set the font color to blue
		pdf.setcolor("fill", "rgb", 0, 0, 1, 0);

		// out put the text
		CString cs1,cs2;

		GetDlgItemText(IDC_STATIC_DATA_REPORT,cs1);
		GetDlgItemText(IDC_DATA_REPORT,cs2);
		cs1 =cs1 + " " + cs2;
		pdf.show_xy((LPCSTR)cs1, left_offset + 200, iheight - 40);

		int img = pdf.open_image_file("jpeg", (LPCSTR)cstempjpgfile, "", 0);
		pdf.place_image(img, left_offset, 130, (float)0.93);
		pdf.close_image(img);

		pdf.setfont(font_song, 14);
		pdf.setcolor("fill", "rgb", 0, 0, 0, 0);

		GetDlgItemText(IDC_STATIC_START_TIME,cs1);
		GetDlgItemText(IDC_START_TIME,cs2);
		cs1 =cs1 + " " + cs2;		
		pdf.show_xy((LPCSTR)cs1, left_offset, text_offset_y);
		text_offset_y -= text_line_height;




		GetDlgItemText(IDC_STATIC_HOURS,cs1);
		GetDlgItemText(IDC_HOURS,cs2);
		cs1 =cs1 + " " + cs2;				
		pdf.show_xy((LPCSTR)cs1, left_offset, text_offset_y);

		GetDlgItemText(IDC_STATIC_SERIAL,cs1);
		GetDlgItemText(IDC_SERIAL,cs2);
		cs1 =cs1 + " " + cs2;				
		pdf.show_xy((LPCSTR)cs1, left_offset+400, text_offset_y);
		text_offset_y -= text_line_height;

		GetDlgItemText(IDC_STATIC_PRINTED_BY,cs1);
		GetDlgItemText(IDC_PRINTED_BY,cs2);
		cs1 =cs1 + " " + cs2;		
		pdf.show_xy((LPCSTR)cs1, left_offset, text_offset_y);

		GetDlgItemText(IDC_STATIC_DATE_TIME,cs1);
		GetDlgItemText(IDC_DATE_TIME,cs2);
		cs1 =cs1 + " " + cs2;				
		pdf.show_xy((LPCSTR)cs1, left_offset+400, text_offset_y);
		text_offset_y -= text_line_height;


		GetDlgItemText(IDC_STATIC_SHIP_NOTE,cs1);
		GetDlgItemText(IDC_SHIP_NOTE,cs2);
		cs1 =cs1 + " " + cs2;				
		pdf.show_xy((LPCSTR)cs1, left_offset, text_offset_y);
		


		pdf.end_page();

		pdf.close();
	}

	catch(PDFlib::Exception &ex)
	{
		AfxMessageBox("Please make sure the pdf file is not opened!");
		TRACE("%s",ex.get_message());
		return -1;
	}
	catch(char *pStrErr)
	{
		AfxMessageBox("Save Error! Please make sure the pdf file is not opened by other application!");

		TRACE(pStrErr);
		return -1;
	}
	catch(...)
	{
		TRACE("Unknown error");
		return -1;
	}

	return 0;

}


