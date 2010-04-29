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

CDataLoggerGraph::CDataLoggerGraph()
{
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
	CDC MemDC;
	CPen BorderPen,*pOldPen,UnderLinePen;
	CBrush BGBrush,*pOldBrush;

	
	MemDC.CreateCompatibleDC(&dc);
	MemDC.SetMapMode(dc.GetMapMode());


	// TODO: Add your message handler code here
	CRect rect;
	this->GetClientRect(&rect);


	BorderPen.CreatePen(PS_DASHDOTDOT, 1, RGB(255, 25, 5));

	BGBrush.CreateSolidBrush(RGB(255,255,255));

	pOldPen=dc.SelectObject(&BorderPen);
	pOldBrush=dc.SelectObject(&BGBrush);

	dc.Rectangle(&rect);

	dc.SelectObject(pOldPen);
	dc.SelectObject(pOldBrush);

	rect.DeflateRect(1,1);



	// Do not call CStatic::OnPaint() for painting messages
}
