// DataLoggerGraph.cpp : implementation file
//

#include "stdafx.h"
#include "DataLogger.h"
#include "DataLoggerGraph.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDataLoggerGraph

char XAXIS[][21][20]=
{
	{"","2","4","6","8","10","12","14","16","18","20","22",""},
	{"","6","12","18","24","30","36","42","48","54","60","66",""},
	{"","1","2","3","4","5","6",""},
	{"","1","2","3","4","5","6","7","8","9",""},
	{"","1","","3","","5","","7","","9","","11","","13","","15","","17","","19",""},
	{"","2","","6","","10","","14","","18","","22","","26","","30","","34","","38",""},
	{"","5","","15","","25","","35","","45","","55","","65","",""},
	{"","10","20","30","40","50","60","70","80",""}
};

CDataLoggerGraph::CDataLoggerGraph()
{

	m_LeftOffset = m_RightOffset = m_TopOffset = m_BottomOffset  = 50;

	m_XGrid = 12;
	m_YGrid = 6;

}

CDataLoggerGraph::~CDataLoggerGraph()
{
}


BEGIN_MESSAGE_MAP(CDataLoggerGraph, CStatic)
	//{{AFX_MSG_MAP(CDataLoggerGraph)
	ON_WM_PAINT()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDataLoggerGraph message handlers

void CDataLoggerGraph::OnPaint() 
{
	CPaintDC dc(this); // device context for painting


	this->DrawGridAndText(dc);
	
	this->DrawData(dc);
#if 1
	CRect rect;
	CDC memdc;
	CBitmap   bmp;
	CBitmap *pOldBMP;

	this->GetClientRect(&rect);
	bmp.CreateCompatibleBitmap(&dc, rect.Width(),rect.Height());   	

	//Create compatibledc
	if(!memdc.CreateCompatibleDC(&dc))  
	{  
		TRACE("Can't create compatibaleDC\n");
		::PostQuitMessage(0);  
	}   
	pOldBMP = memdc.SelectObject(&bmp);   
	memdc.BitBlt(0,   0,  rect.Width(),   rect.Height(),   &dc,   0,   0,   SRCCOPY);  	

	memdc.MoveTo(0,0);
	memdc.LineTo(100,100);

	dc.BitBlt(0,   0,  rect.Width(),   rect.Height(),   &memdc,   0,   0,   SRCCOPY);  	
	memdc.SelectObject(pOldBMP);
	memdc.DeleteDC();
#endif

	// Do not call CStatic::OnPaint() for painting messages
}


int CDataLoggerGraph::GetXAxisCharsIndex()
{

	int ret = 0; 
	switch(m_Days)	
	{
	case 1:
		ret = 0;
		break;
	case 3:
		ret = 1;
		break;
	case 7:
		ret = 2;
		break;
	case 10:
		ret = 3;

		break;
	case 20:
		ret = 4;
		break;
	case 40:
		ret = 5;
		break;
	case 75:
		ret = 6;
		break;
	case 90:
		ret = 7;
		break;
	default:
		ASSERT(1==0);
		break;
	}
	return ret;
	
}
void CDataLoggerGraph::SetDays(int days)
{
	m_Days = days;
	switch(m_Days)
	{
	case 1:
	case 3:
		m_XGrid = 12;
		break;
	case 7:
		m_XGrid = 7;
		break;
	case 10:
		m_XGrid = 10;
		break;
	case 20:
	case 40:
		m_XGrid = 20;
		break;
	case 75:
		m_XGrid = 15;
		break;
	case 90:
		m_XGrid = 9;
		break;
	default:
		ASSERT(1==0);
		break;
	}

}

void CDataLoggerGraph::DrawGridAndText(CPaintDC& dc)
{
	CDC memdc;
	//Double buffer bitmap
	CBitmap      bmp;   
	CPen GridPen,*pOldPen;
	CRect rect;
	this->GetClientRect(&rect);
	bmp.CreateCompatibleBitmap(&dc, rect.Width(),rect.Height());   	

	//Create compatibledc
	if(!memdc.CreateCompatibleDC(&dc))  
	{  
		TRACE("Can't create compatibaleDC\n");
		::PostQuitMessage(0);  
	}   

  
	CBitmap   *pOldBMP   =   memdc.SelectObject(&bmp);   
	
	memdc.FillSolidRect(&rect,  RGB(255,255,255));   

	TRACE(" DataLoggerGraph l=%d,t=%d,r=%d,b=%d\n",rect.left,rect.top,rect.right,rect.bottom);

	//adjust the offset
	m_RightOffset +=  (rect.Width() - m_LeftOffset- m_RightOffset)%m_XGrid;
	m_BottomOffset += (rect.Height()-m_TopOffset - m_BottomOffset)%m_YGrid;

	m_xpixel = rect.Width()-m_LeftOffset- m_RightOffset;
	TRACE("There are %d pixel x size\n",m_xpixel);

	m_ypixel = rect.Height()-m_TopOffset - m_BottomOffset;
	TRACE("There are %d pixel y size\n",m_ypixel);


	GridPen.CreatePen(PS_DASHDOTDOT, 1, RGB(255, 25, 5));
	pOldPen=memdc.SelectObject(&GridPen);



//Draw the horizontal line
	int x_start = m_LeftOffset;
	int y_start = m_TopOffset;
	int x_end = rect.Width() - m_RightOffset;
	int y_end = y_start;

	CString str1;
	CRect rect1;

	char pFDegree[][20] = {"60.0","55.0","50.0","45.0","40.0","35.0","30.0"};
	char pCDegree[][20] = {"15.5","12.7","10.0","7.2","4.4","1.7","-1.0"};
	for( int i = 0;i<m_YGrid + 1;i++)
	{

		rect1.left = x_start - 30;
		rect1.top = y_start - 10;
		rect1.right = x_start;
		rect1.bottom = y_start + 30;
		memdc.DrawText(pFDegree[i],&rect1,DT_LEFT);
		memdc.MoveTo(x_start,y_start);
		memdc.LineTo(x_end,y_end);

		rect1.left = x_end + 5;
		rect1.top = y_end  - 10;
		rect1.right = rect1.left + 30;
		rect1.bottom = rect1.top + 30;
		memdc.DrawText(pCDegree[i],&rect1,DT_LEFT);
		y_start += (rect.Height() - m_TopOffset - m_BottomOffset)/m_YGrid;
		y_end = y_start;


	}
//Draw the vertical line

	x_start = m_LeftOffset;
	y_start = m_TopOffset;
	x_end = x_start;
	y_end = rect.Height() - m_BottomOffset;

	int xaxis_index;

	for(i = 0;i<m_XGrid + 1;i++)
	{
		memdc.MoveTo(x_start,y_start);
		memdc.LineTo(x_end,y_end);
		rect1.left = x_end - 5;
		rect1.top = y_end ;
		rect1.right = rect1.left + 30;
		rect1.bottom = rect1.top + 30;
		xaxis_index = GetXAxisCharsIndex();
		memdc.DrawText(XAXIS[xaxis_index][i],&rect1,DT_LEFT);
		x_start +=(rect.Width() - m_LeftOffset - m_RightOffset)/m_XGrid; 


		x_end = x_start;
	}

//	//Biblt the bitmap to orignal dc

	dc.BitBlt(0,   0,  rect.Width(),   rect.Height(),   &memdc,   0,   0,   SRCCOPY);  	
	memdc.SelectObject(pOldPen);

	memdc.SelectObject(pOldBMP);  

	memdc.DeleteDC();

}

void CDataLoggerGraph::DrawData(CPaintDC& dc)
{
}
