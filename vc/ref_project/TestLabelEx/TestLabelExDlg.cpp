// TestLabelExDlg.cpp : implementation file
//

#include "stdafx.h"
#include "TestLabelEx.h"
#include "TestLabelExDlg.h"

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
// CTestLabelExDlg dialog

CTestLabelExDlg::CTestLabelExDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CTestLabelExDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CTestLabelExDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	// Note that LoadIcon does not require a subsequent DestroyIcon in Win32
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CTestLabelExDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CTestLabelExDlg)
	DDX_Control(pDX, IDC_STATIC_2, m_Label2);
	DDX_Control(pDX, IDC_STATIC_1, m_Label1);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CTestLabelExDlg, CDialog)
	//{{AFX_MSG_MAP(CTestLabelExDlg)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_STATIC_1, OnStatic1)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CTestLabelExDlg message handlers

BOOL CTestLabelExDlg::OnInitDialog()
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
	
	// TODO: Add extra initialization here
	m_Label1.SetTextColor(RGB(255,0,0));
	m_Label1.SetUnderLine(TRUE,RGB(0,0,0xff));
	m_Label1.SetBorder(TRUE,RGB(0,255,0));
	m_Label1.SetBkColor(RGB(255,255,231));
	m_Label1.EnableAutoUnderLine(TRUE);
	m_Label1.SetLabelBitmap(IDB_BITMAP3);
	m_Label1.SetMouseOverLabelBitmap(IDB_BITMAP4);
	m_Label1.SetClickedLabelBitmap(IDB_BITMAP5);

	m_Label2.SetBGBitmap(IDB_BITMAP1);
	m_Label2.SetMouseOverBGBitmap(IDB_BITMAP2);
	m_Label2.SetClickedBGBitmap(IDB_BITMAP4);
	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CTestLabelExDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

void CTestLabelExDlg::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	if (IsIconic())
	{
		

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
	RECT rect;
	this->GetClientRect(&rect);
	CBrush BGBrush;
	BGBrush.CreateSolidBrush(RGB(100,100,100));
	dc.SelectObject(&BGBrush);
	dc.Rectangle(&rect);
}

// The system calls this to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CTestLabelExDlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}

void CTestLabelExDlg::OnStatic1() 
{
	// TODO: Add your control notification handler code here
	AfxMessageBox("hello");
}
