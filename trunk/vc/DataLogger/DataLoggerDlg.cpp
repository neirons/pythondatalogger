// DataLoggerDlg.cpp : implementation file
//

#include "stdafx.h"
#include "DataLogger.h"
#include "DataLoggerDlg.h"
#include <fstream.h>

#include <iostream.h>

#include <conio.h>
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
	
	CString sFName;
	
	CFileDialog fd( FALSE,        // true for open, false for save  
		
		_T("BMP"),       // default extention  
		
		NULL,        // initial filename in box);  
		
		OFN_HIDEREADONLY & // flags for detailed behaviour  
		
		OFN_OVERWRITEPROMPT,  
		
		NULL,        // file-filter pairs  
		
		NULL);       // pointer to parent window  
	
	fd.m_ofn.lpstrTitle    = _T("Enter file name...");  
	
	fd.m_ofn.lpstrInitialDir = NULL;  
	
	fd.m_ofn.lpstrFilter   = _T("BMP files (*.bmp)\000*.BMP\000");  
	
	HANDLE hDIB = 0;
	hDIB = GetWindowDIB((CWnd*)&m_Graph);
	
	if(fd.DoModal() == IDOK)  
		
		sFName = fd.GetPathName();  
	
	else  
		
		return;  
	
	 WriteDIB((LPSTR)(LPCSTR)sFName,hDIB);

     GlobalFree(hDIB);

}
HANDLE CDataLoggerDlg::GetWindowDIB(CWnd *pWnd)
{

	   CBitmap bitmap;

       CWindowDC dc(pWnd);

       CDC memDC;

       CRect rect;

       memDC.CreateCompatibleDC(&dc);

       pWnd->GetWindowRect(rect);

       bitmap.CreateCompatibleBitmap(&dc,rect.Width(),rect.Height());

       CBitmap* pOldBitmap=memDC.SelectObject(&bitmap);

       memDC.BitBlt(0,0,rect.Width(),rect.Height(),&dc,0,0,SRCCOPY);

       CPalette pal;

       if(dc.GetDeviceCaps(RASTERCAPS)&RC_PALETTE)

       {

              UINT nSize=sizeof(LOGPALETTE)+(sizeof(PALETTEENTRY)*256);

              LOGPALETTE* pLP=(LOGPALETTE*)new BYTE[nSize];

              pLP->palVersion=0x300;

              pLP->palNumEntries=GetSystemPaletteEntries(dc,0,255,pLP->palPalEntry);

              pal.CreatePalette(pLP);

              delete[] pLP;

 

       }

       memDC.SelectObject(pOldBitmap);

       HANDLE hDIB=DDBToDIB(bitmap,BI_RGB,&pal);

       if(hDIB==NULL)

              return 0;
		return hDIB;


}
BOOL CDataLoggerDlg::WriteWindowToDIB(LPTSTR szFile, CWnd *pWnd)
{

return FALSE;
 

}

 

HANDLE CDataLoggerDlg::DDBToDIB(CBitmap &bitmap, DWORD dwCompression, CPalette *pPal)

{

       BITMAP bm;

       BITMAPINFOHEADER bi;

       LPBITMAPINFOHEADER lpbi;

       DWORD dwLen;

       HANDLE hDIB;

       HANDLE handle;

       HDC hDC;

       HPALETTE hPal;

       ASSERT(bitmap.GetSafeHandle());

       if(dwCompression==BI_BITFIELDS)

              return NULL;

       hPal=(HPALETTE)pPal->GetSafeHandle();

       if(hPal==NULL)

              hPal=(HPALETTE)GetStockObject(DEFAULT_PALETTE);

       bitmap.GetObject(sizeof(bm),(LPSTR)&bm);

       bi.biSize=sizeof(BITMAPINFOHEADER);

       bi.biWidth=bm.bmWidth;

       bi.biHeight=bm.bmHeight;

       bi.biPlanes=1;

       bi.biBitCount=bm.bmPlanes*bm.bmBitsPixel;

       bi.biCompression=dwCompression;

       bi.biSizeImage=0;

       bi.biXPelsPerMeter=0;

       bi.biYPelsPerMeter=0;

       bi.biClrImportant=0;

       bi.biClrUsed=0;

 

       int nColors=(1<<bi.biBitCount);

       if(nColors>256)

              nColors=0;

       dwLen=bi.biSize+nColors*sizeof(RGBQUAD);

       hDC=::GetDC(NULL);

       hPal=SelectPalette(hDC,hPal,FALSE);

       RealizePalette(hDC);

       hDIB=GlobalAlloc(GMEM_FIXED,dwLen);

       if(!hDIB)

       {

              SelectPalette(hDC,hPal,FALSE);

                     ::ReleaseDC(NULL,hDC);

              return NULL;

 

       }

      

       lpbi=(LPBITMAPINFOHEADER)hDIB;

       *lpbi=bi;

       GetDIBits(hDC,(HBITMAP)bitmap.GetSafeHandle(),0L,(DWORD)bi.biHeight,(LPBYTE)NULL,(LPBITMAPINFO)lpbi,(DWORD)DIB_RGB_COLORS);

       bi=*lpbi;

       if(bi.biSizeImage==0)

       {

              bi.biSizeImage=((((bi.biWidth*bi.biBitCount)+31)& ~31)/8)*bi.biHeight;

              if(dwCompression!=BI_RGB)

                     bi.biSizeImage=(bi.biSizeImage*3)/2;

       }

 

       dwLen+=bi.biSizeImage;

       if(handle=GlobalReAlloc(hDIB,dwLen,GMEM_MOVEABLE))

              hDIB=handle;

       else

       {

              GlobalFree(hDIB);

              SelectPalette(hDC,hPal,FALSE);

              ::ReleaseDC(NULL,hDC);

              return NULL;

       }

       lpbi=(LPBITMAPINFOHEADER)hDIB;

       BOOL bGotBits=GetDIBits(hDC,(HBITMAP)bitmap.GetSafeHandle(),0L,(DWORD)bi.biHeight,(LPBYTE)lpbi+(bi.biSize+nColors*sizeof(RGBQUAD)),

              (LPBITMAPINFO)lpbi,(DWORD)DIB_RGB_COLORS);

       if(!bGotBits)

       {

              GlobalFree(hDIB);

              SelectPalette(hDC,hPal,FALSE);

              ::ReleaseDC(NULL,hDC);

              return NULL;

       }

       SelectPalette(hDC,hPal,FALSE);

       ::ReleaseDC(NULL,hDC);

       return hDIB;

}

 

BOOL CDataLoggerDlg::WriteDIB(LPTSTR szFile, HANDLE hDIB)

{

       BITMAPFILEHEADER hdr;

       LPBITMAPINFOHEADER lpbi;

       if(!hDIB)

              return FALSE;

       CFile file;

       if(!file.Open(szFile,CFile::modeWrite|CFile::modeCreate))

              return FALSE;

       lpbi=(LPBITMAPINFOHEADER)hDIB;

       int nColors=1<<lpbi->biBitCount;

       hdr.bfType=((WORD)('M'<<8)|'B');

       hdr.bfSize=::GlobalSize(hDIB)+sizeof(hdr);

       hdr.bfReserved1=0;

       hdr.bfReserved2=0;

       hdr.bfOffBits=(DWORD)(sizeof(hdr)+lpbi->biSize+nColors*sizeof(RGBQUAD));

       file.Write(&hdr,sizeof(hdr));

       file.Write(lpbi,GlobalSize(hDIB));

       return TRUE;

 

}

void CDataLoggerDlg::OnButtonPrint() 
{
	// TODO: Add your control notification handler code here
}

void CDataLoggerDlg::SaveWindowToJPG(CWnd *pWnd, CString cFileName)
{

    CClientDC dc(pWnd);

    CDC  m_dcGraph;

    m_dcGraph.CreateCompatibleDC(&dc);
	CRect rect;
	pWnd->GetClientRect(&rect);

    CBitmap   bitmapGraph;

    bitmapGraph.CreateCompatibleBitmap(&dc,rect.Width(),rect.Height());
    m_dcGraph.SelectObject(&bitmapGraph);
    m_dcGraph.BitBlt(0,0,rect.Width(),rect.Height(),&dc,0,0,SRCCOPY);


   HBITMAP   hBitmap;
   hBitmap=(HBITMAP)bitmapGraph.GetSafeHandle();


   CLSID encoderClsid;
   Bitmap bitmap(hBitmap,NULL);
   GetEncoderClsid(L"image/jpeg",&encoderClsid);
   EncoderParameters encoderPara;
   encoderPara.Count=1;
   encoderPara.Parameter[0].Guid=EncoderQuality;
   encoderPara.Parameter[0].Type=EncoderParameterValueTypeLong;
   encoderPara.Parameter[0].NumberOfValues=1;
   ULONG quality=50;
   encoderPara.Parameter[0].Value=&quality;

   bitmap.Save(cFileName.AllocSysString(),&encoderClsid,&encoderPara);
	
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
	// TODO: Add your control notification handler code here
	this->SaveWindowToJPG((CWnd*)this,"c:\\2.jpg");
}
