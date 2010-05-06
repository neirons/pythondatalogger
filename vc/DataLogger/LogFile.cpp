// LogFile.cpp: implementation of the CLogFile class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
//#include "KJ83NUpper.h"
#include "LogFile.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CLogFile::CLogFile()
{

}

CLogFile::~CLogFile()
{

}

CString CLogFile::GetFilePath()
{   
	CString csFilePath;


	CString strFilePath;
	char strBuff[256];
	GetCurrentDirectory(256,strBuff); //Get current path
	strFilePath.Format("%s\\config.ini",strBuff);
	GetPrivateProfileString("DataLogger","logfile",NULL,strBuff,80,strFilePath);
	csFilePath.Format("%s",strBuff);
	return csFilePath;

#if 0

	GetModuleFileName(NULL,m_FilePath.GetBufferSetLength(MAX_PATH+1),MAX_PATH);

	m_FilePath.ReleaseBuffer();

	int m_iPosIndex;

	m_iPosIndex = m_FilePath.ReverseFind('\\'); 

	m_FilePath = m_FilePath.Left(m_iPosIndex);

#endif 

}

BOOL CLogFile::WriteLog(CString LogText)
{
	try
	{
		CFile cFile;
		CString csFilePath = GetFilePath();
		if(csFilePath.IsEmpty())
		{
			TRACE("Do nothing");
		}
		else
		{
			CString csCurrentTime = CTime::GetCurrentTime().Format("%Y-%m-%d %X");

			if(!cFile.Open(csFilePath,CFile::modeReadWrite))
			{
				cFile.Open(csFilePath,CFile::modeCreate | CFile::modeReadWrite );
			}
			
			cFile.SeekToEnd(); 

			LogText = csCurrentTime +":\t"+ LogText + "\r\n";
			char* m_szMessage;

			m_szMessage=(LPTSTR)(LPCTSTR)LogText;

			cFile.Write(m_szMessage,lstrlen(m_szMessage));   

			cFile.Close();
		}
	}
	catch(CFileException fileException)
	{
		AfxMessageBox("Log file write error!!!!");
		return false;
	}
	return true;
}