// LabelEx.cpp : implementation file
//

#include "stdafx.h"
#include "TestLabelEx.h"
#include "LabelEx.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CLabelEx

CLabelEx::CLabelEx()
{
	m_bClicked=FALSE;
	m_bOver=FALSE;
	m_bUnderLine=FALSE;
	m_bAutoUnderLine=FALSE;
	m_bBorder=FALSE;
	m_bHighLight=TRUE;
	m_bBackUnderLine=FALSE;

	m_crText=RGB(0,0,0);
	m_crBorder=RGB(0,0,0);
	m_crBG=RGB(255,255,255);
	m_crUnderLine=RGB(0,0,255);

	m_hHandCur=AfxGetApp()->LoadCursor(IDC_HAND);
}

CLabelEx::~CLabelEx()
{
	if(m_BGBm.GetSafeHandle()!=NULL)
		m_BGBm.DeleteObject();
	if(m_MouseOverBGBm.GetSafeHandle()!=NULL)
		m_MouseOverBGBm.DeleteObject();
	if(m_ClickedBGBm.GetSafeHandle()!=NULL)
		m_ClickedBGBm.DeleteObject();
	if(m_LabelBm.GetSafeHandle()!=NULL)
		m_LabelBm.DeleteObject();
	if(m_MouseOverLabelBm.GetSafeHandle()!=NULL)
		m_MouseOverLabelBm.DeleteObject();
	if(m_ClickedLabelBm.GetSafeHandle()!=NULL)
		m_ClickedLabelBm.DeleteObject();
}


BEGIN_MESSAGE_MAP(CLabelEx, CStatic)
	//{{AFX_MSG_MAP(CLabelEx)
	ON_WM_PAINT()
	ON_WM_MOUSEMOVE()
	ON_WM_LBUTTONDOWN()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CLabelEx message handlers

void CLabelEx::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	dc.SetTextColor(m_crText);
	dc.SetBkMode(TRANSPARENT);
	dc.SelectObject(this->GetFont());
	///准备工作
	CRect rect;
	CDC MemDC;
	CPen BorderPen,*pOldPen,UnderLinePen;
	CBrush BGBrush,*pOldBrush;
	BITMAP bm;
	int nTextLeft=0,nTextTop=0; //文字输出的位置

	this->GetClientRect(&rect);

	MemDC.CreateCompatibleDC(&dc);
	MemDC.SetMapMode(dc.GetMapMode());

	///画边框
	if(m_bBorder)
	{
		BorderPen.CreatePen(PS_SOLID,1,m_crBorder);
		BGBrush.CreateSolidBrush(m_crBG);

		pOldPen=dc.SelectObject(&BorderPen);
		pOldBrush=dc.SelectObject(&BGBrush);

		dc.Rectangle(&rect);

		dc.SelectObject(pOldPen);
		dc.SelectObject(pOldBrush);

		rect.DeflateRect(1,1);
	}
	///贴背景图
	if(m_bClicked && m_ClickedBGBm.GetSafeHandle()!=NULL)
	{
		MemDC.SelectObject(m_ClickedBGBm);
		dc.BitBlt(rect.left,rect.top,rect.Width(),rect.Height(),
			&MemDC,0,0,SRCCOPY);
	}
	else if(m_bOver && m_MouseOverBGBm.GetSafeHandle()!=NULL)//鼠标经过的时候
	{
		MemDC.SelectObject(m_MouseOverBGBm);
		dc.BitBlt(rect.left,rect.top,rect.Width(),rect.Height(),
			&MemDC,0,0,SRCCOPY);
	}
	else if(m_BGBm.GetSafeHandle()!=NULL)
	{
		MemDC.SelectObject(m_BGBm);
		dc.BitBlt(rect.left,rect.top,rect.Width(),rect.Height(),
			&MemDC,0,0,SRCCOPY);
	}
	///贴标签图片
	if(m_bClicked && m_ClickedLabelBm.GetSafeHandle()!=NULL)
	{
		m_ClickedLabelBm.GetBitmap(&bm);
		double fScal=bm.bmWidth*1.0/bm.bmHeight;
		nTextLeft=int(rect.Height()*fScal)+4;
		MemDC.SelectObject(m_ClickedLabelBm);
		dc.StretchBlt(rect.left,rect.top,int(rect.Height()*fScal),rect.Height(),
			&MemDC,0,0,bm.bmWidth,bm.bmHeight,SRCCOPY);
	}
	else if(m_bOver && m_MouseOverLabelBm.GetSafeHandle()!=NULL)
	{
		m_MouseOverLabelBm.GetBitmap(&bm);
		double fScal=bm.bmWidth*1.0/bm.bmHeight;
		nTextLeft=int(rect.Height()*fScal)+4;
		MemDC.SelectObject(m_MouseOverLabelBm);
		dc.StretchBlt(rect.left,rect.top,int(rect.Height()*fScal),rect.Height(),
			&MemDC,0,0,bm.bmWidth,bm.bmHeight,SRCCOPY);
	}
	else if(m_LabelBm.GetSafeHandle()!=NULL)
	{
		m_LabelBm.GetBitmap(&bm);
		double fScal=bm.bmWidth*1.0/bm.bmHeight;
		nTextLeft=int(rect.Height()*fScal)+4;
		MemDC.SelectObject(m_LabelBm);
		dc.StretchBlt(rect.left,rect.top,int(rect.Height()*fScal),rect.Height(),
			&MemDC,0,0,bm.bmWidth,bm.bmHeight,SRCCOPY);
	}
	else
	{
		nTextLeft=4;
	}
	///输出文字
	TEXTMETRIC tm;
	dc.GetTextMetrics(&tm);
	CString strText;
	this->GetWindowText(strText);
	nTextTop=rect.top+(rect.Height()-tm.tmHeight)/2;
	if(strText.GetLength()>0)
	{	
		dc.TextOut(nTextLeft,nTextTop,strText);
	}
	
	///画下划线
	if(m_bUnderLine)
	{
		nTextLeft-=2;
		nTextTop=nTextTop+tm.tmHeight+1;
		UnderLinePen.CreatePen(PS_SOLID,1,m_crUnderLine);
		pOldPen=dc.SelectObject(&UnderLinePen);
		dc.MoveTo(nTextLeft,nTextTop);
		dc.LineTo(nTextLeft+tm.tmAveCharWidth*strText.GetLength(),nTextTop);
	}
}

void CLabelEx::PreSubclassWindow() 
{
	// TODO: Add your specialized code here and/or call the base class
	DWORD dwStyle = GetStyle();
    ::SetWindowLong(GetSafeHwnd(), GWL_STYLE, dwStyle | SS_NOTIFY);
	
	SetFont(GetParent()->GetFont());
	CStatic::PreSubclassWindow();
}

void CLabelEx::OnMouseMove(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	if (m_bOver)        // Cursor is currently over control
    {
        CRect rect;
        GetClientRect(rect);

        if (!rect.PtInRect(point))
        {
            m_bOver = FALSE;
			if(m_bAutoUnderLine)  ///自动下划线
			{
				this->SetUnderLine(FALSE,RGB(0,0,0));
			}
			if(m_bHighLight)   //自动高亮
			{
				///恢复原来的字体颜色
				this->SetTextColor(m_crBackText);
			}
			RedrawWindow();
            ReleaseCapture();
            return;
        }
    }
    else                      // Cursor has just moved over control
    {
        m_bOver = TRUE;
		if(m_bAutoUnderLine)
		{
			this->SetUnderLine(TRUE,RGB(0,0,255));
		}
		if(m_bHighLight)
		{
			m_crBackText=m_crText;
			this->SetTextColor(RGB(0,0,255));
		}
        RedrawWindow();
        SetCapture();
		::SetCursor(m_hHandCur);
    }

	CStatic::OnMouseMove(nFlags, point);
}

void CLabelEx::SetTextColor(COLORREF crText)
{
	m_crText=crText;
}

void CLabelEx::SetUnderLine(BOOL bUnderLine, COLORREF crUnderLine)
{
	m_bUnderLine=bUnderLine;
	m_crUnderLine=crUnderLine;

}

void CLabelEx::SetBorder(BOOL bBorder, COLORREF crBorder)
{
	m_bBorder=bBorder;
	m_crBorder=crBorder;
}

void CLabelEx::SetBkColor(COLORREF crBkColor)
{
	m_crBG=crBkColor;

}

void CLabelEx::EnableAutoUnderLine(BOOL bAutoUnderLine)
{
	m_bAutoUnderLine=bAutoUnderLine;
	if(m_bAutoUnderLine)   ///设置了自动下划线
	{
		m_bBackUnderLine=m_bUnderLine;//保存原来下划线的状态
		m_bUnderLine=FALSE;
	}
	else   ///取消了自动下划线
	{
		m_bUnderLine=m_bBackUnderLine;//回复原来下划线的状态
	}
}

void CLabelEx::RedrawWindow()
{
	CStatic::RedrawWindow();
	//RECT rect;
	//this->GetWindowRect(&rect);
	//GetParent()->ScreenToClient(&rect);
	//GetParent()->InvalidateRect(&rect);
}

void CLabelEx::SetBGBitmap(UINT ID)
{
	if(m_BGBm.GetSafeHandle()!=NULL)
		m_BGBm.DeleteObject();
	m_BGBm.LoadBitmap(ID);

}

void CLabelEx::SetMouseOverBGBitmap(UINT ID)
{
	if(m_MouseOverBGBm.GetSafeHandle()!=NULL)
		m_MouseOverBGBm.DeleteObject();
	m_MouseOverBGBm.LoadBitmap(ID);
}

void CLabelEx::SetClickedBGBitmap(UINT ID)
{
	if(m_ClickedBGBm.GetSafeHandle()!=NULL)
		m_ClickedBGBm.DeleteObject();
	m_ClickedBGBm.LoadBitmap(ID);

}

void CLabelEx::SetLabelBitmap(UINT ID)
{
	if(m_LabelBm.GetSafeHandle()!=NULL)
		m_LabelBm.DeleteObject();
	m_LabelBm.LoadBitmap(ID);

}


void CLabelEx::OnLButtonDown(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	m_bClicked=!m_bClicked;
	RedrawWindow();
	CStatic::OnLButtonDown(nFlags, point);
}

void CLabelEx::SetMouseOverLabelBitmap(UINT ID)
{
	if(m_MouseOverLabelBm.GetSafeHandle()!=NULL)
		m_MouseOverLabelBm.DeleteObject();
	m_MouseOverLabelBm.LoadBitmap(ID);

}

void CLabelEx::SetClickedLabelBitmap(UINT ID)
{
	if(m_ClickedLabelBm.GetSafeHandle()!=NULL)
		m_ClickedLabelBm.DeleteObject();
	m_ClickedLabelBm.LoadBitmap(ID);

}

///自动感应字体颜色
void CLabelEx::EnableHighLight(BOOL bHighLight)
{
	m_bHighLight=bHighLight;
}
