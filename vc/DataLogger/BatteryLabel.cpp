// BatteryLabel.cpp : implementation file
//

#include "stdafx.h"
#include "DataLogger.h"
#include "BatteryLabel.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CBatteryLabel

CBatteryLabel::CBatteryLabel()
{
}

CBatteryLabel::~CBatteryLabel()
{
}


BEGIN_MESSAGE_MAP(CBatteryLabel, CStatic)
	//{{AFX_MSG_MAP(CBatteryLabel)
	ON_WM_PAINT()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CBatteryLabel message handlers

void CBatteryLabel::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	
	// TODO: Add your message handler code here
//1.First full fill the rect with white brush
	CRect rect;
	this->GetClientRect(&rect);

	CBrush WhiteBrush,*pOldBrush,RedBrush;
	CPen WhitePen,*pOldPen;
	WhiteBrush.CreateSolidBrush(RGB(255,255,255));
	RedBrush.CreateSolidBrush(RGB(255,0,0));	
	WhitePen.CreatePen(1,0,RGB(255,255,255));

	pOldPen=dc.SelectObject(&WhitePen);
	pOldBrush=dc.SelectObject(&WhiteBrush);
	dc.Rectangle(&rect);
	dc.SelectObject(pOldBrush);
	dc.SelectObject(pOldPen);	

	TRACE("l=%d,t=%d,r=%d,b=%d",rect.left,rect.top,rect.right,rect.bottom);


#define BATTERY_STEP 20
#define BATTERY_COUNT 13

	//Draw the battery
	int width_step = rect.right/BATTERY_STEP;

	for(int i=0;i<BATTERY_STEP;i++)
	{
		if (i < BATTERY_COUNT)
		{
			pOldBrush=dc.SelectObject(&RedBrush);
		}
		else
		{
			pOldBrush=dc.SelectObject(&WhiteBrush);
		}

		CRect tmprect(rect.left + i * width_step,rect.top,rect.left + (i + 1) * width_step,rect.bottom/2);
		dc.Rectangle(&tmprect);
		
		dc.SelectObject(pOldBrush);

	}
	//Draw the text
	CRect textrect(rect.left,rect.bottom/2,rect.right,rect.bottom);
	dc.DrawText("Low",rect,DT_LEFT|DT_BOTTOM|DT_SINGLELINE);
	dc.DrawText("Full",rect,DT_RIGHT|DT_BOTTOM|DT_SINGLELINE);
	// Do not call CStatic::OnPaint() for painting messages


}
