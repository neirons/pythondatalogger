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
#include <IO.h>

#include <stdio.h>
#include <math.h>
#include <iostream>
#include "ExitDialog.h"
#include "ClearDialog.h"
#include ".\PDFLib\PDFLib.hpp"
#include "ResetDialog.h"
#include "LogFile.h"
#include "Temperature_Const.h"
#include "WarningDialog.h"
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
	virtual void OnOK();
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
	DDX_Control(pDX, IDC_BATTERY, m_BatteryGraph);
	DDX_Control(pDX, IDC_STATIC_GRAPH, m_DataGraph);
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
	

	CString strFilePath;
	char strBuff[256];
	strFilePath=GetCurrentDirectory(256,strBuff); //Get current path
	strFilePath.Format("%s\\config.ini",strBuff);
	int Days = 10;
	double sum = 0;

	if (_access ((LPCSTR)strFilePath, 0) != 0) 
	{
			// File exists! (-1 if not)
		m_Days = 10;
	}
	else
	{
		GetPrivateProfileString("DataLogger","Day",NULL,strBuff,80,strFilePath);
		m_Days = atoi(strBuff);
	}

	switch(m_Days)
	{
	case 1:
//		m_TotalPoint = 1440;
        m_MaxPoint = 1440;
		break;
	case 3:
		m_MaxPoint = 4320;
		break;
	case 7:
		m_MaxPoint = 10080;
		break;
	case 10:
	case 20:
	case 40:
	case 90:
		m_MaxPoint = 14400;
		break;
	case 75:
		m_MaxPoint = 15428;
		break;
	}



#define PI 3.1415926535897931
	if (_access ("datalogger.bin", 0) != 0) 
	{

#if 0
        m_TotalPoint = 144;
		for(int i =0;i<m_TotalPoint;i++)
		{
			m_Data[i] = 42.0+10*cos(2*PI/m_TotalPoint * i);
			sum += m_Data[i];
		}
#else
        m_TotalPoint = 60;
		for(int i =0;i<m_TotalPoint;i++)
		{
			m_Data[i] = 30 + i ;
            if(m_Data[i] > 60)
                m_Data[i] = m_Data[i] - 30;
            if(m_Data[i] > 60)
                m_Data[i] = m_Data[i] - 30;

			sum += m_Data[i];
		}

#endif 


	}
	else
	{
		CFile cfile;
        unsigned short rd_value;
        unsigned short convert_value;

		cfile.Open("datalogger.bin",CFile::modeRead|CFile::typeBinary);

		for(int i =0;i<m_MaxPoint;i++)
		{
            if(cfile.Read(&rd_value,sizeof(unsigned short)) == sizeof(unsigned short) )
            {
            
                rd_value = rd_value + 1;
                if(rd_value == 0)
                    rd_value = 1;
                convert_value = ((4096 - rd_value)* 100 * 1000 ) / rd_value;
                m_Data[i] = GetTemperature(convert_value);
                m_TotalPoint = i ;

            }    
            else
            {
                m_TotalPoint = i ;
                break;

            }
/*

    3300 * rd_value
    --------------------
          4096                            3300
  ---------------------------  =   --------------------
      100*1000                        100 * 1000 + Y




           (4096 - A) * 100 * 1000         
     Y =  --------------------------
               rd_value

*/
			sum += m_Data[i];
		}
        cfile.Close();

	}

	m_AverageTemperature=sum/m_TotalPoint;
	

	CFile cfile;
    unsigned short batter_value;

	cfile.Open("battery.bin",CFile::modeRead|CFile::typeBinary);
    cfile.Read(&batter_value,sizeof(unsigned short));
    cfile.Close();

    //Check the low voltage.
    mBatteryLevel = ((batter_value + 1) * 3000/4096)/100;
    if(mBatteryLevel < 26)
    {

        CWarningDialog dlg;
        if(dlg.DoModal() == IDOK)
        {

            CDialog::OnCancel();
        }

    }   

	m_BatteryGraph.SetBatteryLevel(30,mBatteryLevel);
    

	m_DataGraph.SetData(m_Days,m_Data,m_MaxPoint,m_TotalPoint,m_AverageTemperature);

	CString csHour;
	csHour.Format("%d",Days*24);
	GetDlgItem(IDC_HOURS)->SetWindowText(csHour);

	CLogFile log;
	CString csLog;
	csLog.Format("Day  is %d ,Hour is %s",Days,csHour);

	log.WriteLog(csLog);



    SetUTCTime();

    char   szUser[80];  
    DWORD   cbUser   =   80;  
    GetUserName   (szUser,   &cbUser);

    this->SetDlgItemText(IDC_PRINTED_BY,szUser);
    TRACE("user name is %s",szUser);
    
    

    DisplayCurrentTime();
    DisplayStartTime();

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
	CLogFile logfile;
	CString cslog;

	//Get the report position
	GetDlgItem(IDC_STATIC_REPORT)->GetWindowRect(&rect_report);  
	ScreenToClient(&rect_report);

	SaveWindowToBitmap((CWnd*)this, 0,rect_report.top - 5,m_SaveGraph);
	
	CBitmap PdfGraph;
	SaveWindowToBitmap((CWnd*)&m_DataGraph, 0,0,PdfGraph);

	if(IDOK == dlg.DoModal())
	{
		iFileType = dlg.m_iFileType;
		csFileName = dlg.m_csFileName;
		cslog.Format("save file %s",csFileName);
		logfile.WriteLog(cslog);
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
			SaveToTxtFile(csFileName);
			//Txt file
			break;
        case 4:
			SaveToXMLFile(csFileName);

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
	CClearDialog dlg;
	if(dlg.DoModal() == IDOK)
	{
//		ClearData();

		CResetDialog dlg;
		if(IDOK ==dlg.DoModal())
        {
		    AfxMessageBox("The Report Logger has been successfully cleared.\nUnplug the Logger from the USB, and replace the\ncap. Press the Start button when you are ready to \nbegin logging",MB_OK|MB_ICONINFORMATION);
        }
		CDialog::OnCancel();
	}
	else
	{
		TRACE("Do nothing without exit the dialog");
	}

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



void CDataLoggerDlg::OnCancel() 
{
	// TODO: Add extra cleanup here
	CExitDialog dlg;
	if(dlg.DoModal() == IDOK)
	{
		CDialog::OnCancel();
		AfxMessageBox("OK to unplug logger from PC",MB_OK|MB_ICONINFORMATION);
		TRACE("EXIT..... new reached...");
	}
	else
	{
		TRACE("Do nothing without exit the dialog");
	}

}

void CDataLoggerDlg::SaveToTxtFile(CString txtfilename)
{
	
	CFile cFile(txtfilename,CFile::modeCreate|CFile::modeWrite);
	
	CString cs1,cs2;
	GetDlgItemText(IDC_STATIC_DATA_REPORT,cs1);
	GetDlgItemText(IDC_DATA_REPORT,cs2);
	cs1 =cs1 + " " + cs2 + "\r\n";	
	cFile.Write(cs1,cs1.GetLength());

	cs1.Format("==============================DATA START==============================\r\n");
	cFile.Write(cs1,cs1.GetLength());

	cs1.Empty();

    int j = 0;

	for (int i = 0;i< m_TotalPoint;i++)
	{

        j = j ++;
	    cs2.Format("%.8f,",m_Data[i]);
	    cs1 += cs2;

        if(j % 10 == 0)
        {            
		    cs1 += "\r\n";
		    cFile.Write(cs1,cs1.GetLength());
            j = 0;
            cs1.Empty();
        }

	}
	cs1 += "\r\n";
	cFile.Write(cs1,cs1.GetLength());


	cs1.Format("==============================DATA   END==============================\r\n");
	cs1.Format("Total Point :%8d\r\n",m_TotalPoint);

	cFile.Write(cs1,cs1.GetLength());

	cs1.Format("The average values : %.8f\r\n",m_AverageTemperature);
	cFile.Write(cs1,cs1.GetLength());

	
	GetDlgItemText(IDC_STATIC_START_TIME,cs1);
	GetDlgItemText(IDC_START_TIME,cs2);
	cs1 =cs1 + " " + cs2 + "\r\n";	
	cFile.Write(cs1,cs1.GetLength());

	GetDlgItemText(IDC_STATIC_HOURS,cs1);
	GetDlgItemText(IDC_HOURS,cs2);
	cs1 =cs1 + " " + cs2 + "\r\n";	
	cFile.Write(cs1,cs1.GetLength());

	GetDlgItemText(IDC_STATIC_SERIAL,cs1);
	GetDlgItemText(IDC_SERIAL,cs2);
	cs1 =cs1 + " " + cs2 + "\r\n";	
	cFile.Write(cs1,cs1.GetLength());
	

	GetDlgItemText(IDC_STATIC_PRINTED_BY,cs1);
	GetDlgItemText(IDC_PRINTED_BY,cs2);
	cs1 =cs1 + " " + cs2 + "\r\n";	
	cFile.Write(cs1,cs1.GetLength());

	GetDlgItemText(IDC_STATIC_DATE_TIME,cs1);
	GetDlgItemText(IDC_DATE_TIME,cs2);
	cs1 =cs1 + " " + cs2 + "\r\n";	
	cFile.Write(cs1,cs1.GetLength());
	

	GetDlgItemText(IDC_STATIC_SHIP_NOTE,cs1);
	GetDlgItemText(IDC_SHIP_NOTE,cs2);
	cs1 =cs1 + " " + cs2 + "\r\n";	
	cFile.Write(cs1,cs1.GetLength());

		
	cFile.Close();
	
}

void CDataLoggerDlg::ClearData()
{
	m_TotalPoint = 0;
	CString str;
	str.Empty();
	SetDlgItemText(IDC_DATA_REPORT,str);
	SetDlgItemText(IDC_START_TIME,str);
	SetDlgItemText(IDC_HOURS,str);
	SetDlgItemText(IDC_SERIAL,str);
	SetDlgItemText(IDC_PRINTED_BY,str);
	SetDlgItemText(IDC_DATE_TIME,str);
	SetDlgItemText(IDC_SHIP_NOTE,str);
	m_DataGraph.SetData(m_Days,m_Data,m_MaxPoint,m_TotalPoint,0);

	m_DataGraph.Invalidate();

}

double CDataLoggerDlg::GetTemperature(int value)
{
    int size = sizeof(TEMPERATURE_TABLE)/sizeof(double);
    double value_double = value/1000.0;
    int index;

    if(value_double >= TEMPERATURE_TABLE[0])
        return -50.0;

    if(value_double <= TEMPERATURE_TABLE[size - 1])
        return (size-50.0);

    for(int i=0;i<size;i++)
    {
        if(TEMPERATURE_TABLE[i] > value_double && TEMPERATURE_TABLE[i + 1] < value_double)
        {
            if(( TEMPERATURE_TABLE[i] - value_double ) < (value_double - TEMPERATURE_TABLE[i + 1]))
            {
                index = i;
            }
            else
            {
                index = i + 1;
            }
            break;
        }
    }

    return (index - 50.0);

}

void CDataLoggerDlg::SetUTCTime()
{
    TIME_ZONE_INFORMATION tz;
    GetTimeZoneInformation(&tz);
    CString utc;
    utc.Format("%.2f",-(tz.Bias/60.0));
    SetDlgItemText(IDC_HOURS,utc);

}

void CAboutDlg::OnOK() 
{
	// TODO: Add extra validation here
	
	CDialog::OnOK();
}

void CDataLoggerDlg::SaveToXMLFile(CString xmlfilename)
{
	CFile cFile(xmlfilename,CFile::modeCreate|CFile::modeWrite);
	CString cs1,cs2;
    cs1.Format("<?xml version=\"1.0\" encoding=\"utf-8\"?>\n");
    cFile.Write(cs1,cs1.GetLength());

    cs1.Format("<root>\n");
    cFile.Write(cs1,cs1.GetLength());

    cs1.Format("<TotalPoint>%d</TotalPoint>\n",m_TotalPoint);
    cFile.Write(cs1,cs1.GetLength());
	
	cs1.Format("<Average>%.8f</Average>\n",m_AverageTemperature);
	cFile.Write(cs1,cs1.GetLength());

    
    GetDlgItemText(IDC_PRINTED_BY,cs2);
	cs1.Format("<Printby>%s</Printby>\n",cs2);
	cFile.Write(cs1,cs1.GetLength());

    cs1.Format("<Data>\n");		
    cFile.Write(cs1,cs1.GetLength());

	for (int i = 0;i< m_TotalPoint;i++)
	{
        cs1.Format("<point id=\"%d\">%.8f</point>\n",i,m_Data[i]);
    	cFile.Write(cs1,cs1.GetLength());
	}
    cs1.Format("</Data>\n");		
    cFile.Write(cs1,cs1.GetLength());

    cs1.Format("</root>\n");		
    cFile.Write(cs1,cs1.GetLength());

	cFile.Close();

}

void CDataLoggerDlg::DisplayCurrentTime()
{
    CTime ct = CTime::GetCurrentTime();

    CString cstr_time ;
    cstr_time.Format("%04d-%02d-%02d %02d:%02d:%02d",ct.GetYear(),ct.GetMonth(),ct.GetDay(),
        ct.GetHour(),ct.GetMinute(),ct.GetSecond());

    SetDlgItemText(IDC_DATE_TIME,cstr_time);
    
}

void CDataLoggerDlg::DisplayStartTime()
{

    char buf[] = "[Start-Time]:3010-07-28:09-54";
    CString csStartTime;


    CFile cfile;
    cfile.Open("startime.ini",CFile::modeRead);
    cfile.Read(buf,sizeof(buf));
    buf[23] = ' ';
    buf[26] = ':';
    csStartTime.Format("%s",&buf[13]);
    SetDlgItemText(IDC_START_TIME,csStartTime);
    cfile.Close();

}
