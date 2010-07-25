// ResetDialog.cpp : implementation file
//

#include "stdafx.h"
#include "datalogger.h"
#include "ResetDialog.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CResetDialog dialog


CResetDialog::CResetDialog(CWnd* pParent /*=NULL*/)
	: CDialog(CResetDialog::IDD, pParent)
{
	//{{AFX_DATA_INIT(CResetDialog)
	m_Start_Date = 0;
	m_Start_Time = 0;
	//}}AFX_DATA_INIT
    TIME_ZONE_INFORMATION tz;
    GetTimeZoneInformation(&tz);
    m_iUTC = -(tz.Bias/60.0);

}


void CResetDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CResetDialog)
	DDX_Text(pDX, IDC_EDIT_UTC, m_iUTC);
	DDX_DateTimeCtrl(pDX, IDC_DATE, m_Start_Date);
	DDX_DateTimeCtrl(pDX, IDC_TIME, m_Start_Time);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CResetDialog, CDialog)
	//{{AFX_MSG_MAP(CResetDialog)
	ON_WM_CANCELMODE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CResetDialog message handlers

BOOL CResetDialog::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here



	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CResetDialog::OnCancelMode() 
{
	CDialog::OnCancelMode();
	
	// TODO: Add ymour message handler code here
	
}

void CResetDialog::OnOK() 
{
	// TODO: Add extra validation here
    this->UpdateData(TRUE);

	CString strFilePath;
	char strBuff[256];
	strFilePath=GetCurrentDirectory(256,strBuff); //Get current path
	strFilePath.Format("%s\\startime.ini",strBuff);

    CFile cf;
    cf.Open(strFilePath,CFile::modeReadWrite|CFile::modeCreate);
    CString wstr;
    
    wstr.Format("[Start-Time]:%s:%s\r\n",m_Start_Date.Format("%Y-%m-%d"),m_Start_Time.Format("%H-%M"));

    cf.Write(wstr,wstr.GetLength());
    wstr.Format("[NeedToClear]:1\r\n");
    cf.Write(wstr,wstr.GetLength());
    
    CTime write_time(m_Start_Date.GetYear(),m_Start_Date.GetMonth(),m_Start_Date.GetDay(),
        m_Start_Time.GetHour(),m_Start_Time.GetMinute(),m_Start_Time.GetSecond());

    time_t ob = write_time.GetTime();
    wstr.Format("[Start-Time-Second]:%ld\r\n",ob);    
    cf.Write(wstr,wstr.GetLength());


    CTime ct = CTime::GetCurrentTime();
    time_t osBinaryTime = ct.GetTime();
    wstr.Format("[Current-Time-Second]:%ld\r\n",osBinaryTime);
    cf.Write(wstr,wstr.GetLength());


/**********************/
    wstr.Format("[UTC]:%.2f\r\n",m_iUTC);
    cf.Write(wstr,wstr.GetLength());


    cf.Close();



    CDialog::OnOK();
}
