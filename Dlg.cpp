// Dlg.cpp  Version 1.0
//
// Author:  Hans Dietrich
//          hdietrich2@hotmail.com
//
// This software is released into the public domain.
// You are free to use it in any way you like, except
// that you may not sell this source code.
//
// This software is provided "as is" with no expressed
// or implied warranty.  I accept no liability for any
// damage or loss of business that this software may cause.
//
///////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "XCrashReport.h"
#include "Dlg.h"
#include "MiniVersion.h"
#include <io.h>
#include "SendEmail.h"
#include "XZip.h"
#include "EmailDefines.h"
#include "CrashFileNames.h"
#include "BackupFile.h"
#include "GetFileSizeAsString.h"
#include "WriteRegistry.h"
#include "RegistryDefines.h"
#include "IniDefines.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#ifndef IDC_HAND
#define IDC_HAND MAKEINTRESOURCE(32649)   // From WINUSER.H
#endif

#define MAX_USER_COMMENTS_SIZE	(64 * 1024)

/////////////////////////////////////////////////////////////////////////////
// CDlg dialog

BEGIN_MESSAGE_MAP(CDlg, CDialog)
	//{{AFX_MSG_MAP(CDlg)
	ON_BN_CLICKED(IDC_SEND_ERROR, OnSend)
	ON_BN_CLICKED(IDC_DO_NOT_SEND_ERROR, OnDoNotSend)
	ON_BN_CLICKED(IDC_APP_ICON, OnAppIcon)
	ON_WM_TIMER()
	ON_WM_SETCURSOR()
	//}}AFX_MSG_MAP

	// this message is sent by XHyperLink
	ON_REGISTERED_MESSAGE(WM_XHYPERLINK_CLICKED, OnClickHere)

END_MESSAGE_MAP()

///////////////////////////////////////////////////////////////////////////////
// ctor
CDlg::CDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlg)
	//}}AFX_DATA_INIT
	// Note that LoadIcon does not require a subsequent DestroyIcon in Win32
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	m_strZipFile  = _T("");
	m_nFilesInZip = 0;
	m_bOverIcon   = FALSE;
	m_hLinkCursor = NULL;
	m_hPrevCursor = NULL;
}

///////////////////////////////////////////////////////////////////////////////
// dtor
CDlg::~CDlg()
{

	for (int i = 0; i < m_aFileDetails.GetSize(); i++)
	{
		FILEDETAILS *fd = (FILEDETAILS *) m_aFileDetails[i];
		DeleteFile( fd->strFilePath);
		if (fd)
			delete fd;
	}

	if (m_hLinkCursor)
		DestroyCursor(m_hLinkCursor);
	m_hLinkCursor = NULL;
}

///////////////////////////////////////////////////////////////////////////////
// DoDataExchange
void CDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlg)
	DDX_Control(pDX, IDC_WHAT, m_What);
	DDX_Control(pDX, IDC_CLICK_HERE, m_ClickHere);
	DDX_Control(pDX, IDC_PLEASE_TELL_US, m_PleaseTellUs);
	DDX_Control(pDX, IDC_BANNER, m_Banner);
	DDX_Control(pDX, IDC_APP_ICON, m_Icon);
	//}}AFX_DATA_MAP
}

///////////////////////////////////////////////////////////////////////////////
// OnInitDialog
BOOL CDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon

	// get our path name - we assume that crashed exe is in same directory
	ZeroMemory(m_szModulePath, sizeof(m_szModulePath));
	::GetModuleFileName(NULL, m_szModulePath, _countof(m_szModulePath)-2);
	TCHAR *cp = _tcsrchr(m_szModulePath, _T('\\'));
	m_szFullPath= theApp.m_pszFullPath;
	m_szFullPath.TrimRight(theApp.m_pszModule);

	if (*cp)
	{
		*(cp+1) = _T('\0');		// remove file name
	}

	InitializeDisplay();

	GetRegistryDetails();

	GetFileDetails();

	m_nFilesInZip = ZipFiles();

	LoadHandCursor();

	SetTimer(1, 80, NULL);

	return TRUE;  // return TRUE  unless you set the focus to a control
}

///////////////////////////////////////////////////////////////////////////////
// InitializeDisplay
void CDlg::InitializeDisplay()
{
	m_Banner.SetTextColor(RGB(0,0,0));
	m_Banner.SetBackgroundColor(RGB(255,255,255));
	m_Banner.SetBold(TRUE);

	// get path to exe that crashed - we assume it is in same directory
	CString strExe = m_szFullPath; // m_szModulePath;
	strExe += theApp.m_pszModule;

	// get version resource
	CMiniVersion ver(strExe);
	TCHAR szBuf[1000];
	szBuf[0] = _T('\0');

	// try to get description
	ver.GetFileDescription(szBuf, _countof(szBuf)-2);

	// if that failed try to get product name
	if (szBuf[0] == _T('\0'))
		ver.GetProductName(szBuf, _countof(szBuf)-2);

	ver.Release();

	// if that failed just use exe name
	if (szBuf[0] == _T('\0'))
		_tcscpy(szBuf, theApp.m_pszModule);

	SetWindowText(szBuf);

	CString strBanner;
	strBanner.Format(_T("%s Ha encontrado un error y necesita cerrarse.  ")
					 _T("Sentimos los incovenientes."),
					 szBuf);

	m_Banner.SetWindowText(strBanner);
	m_Banner.SetMargins(22, 8);

	// get icon of crashed app
	HICON hIcon = ::ExtractIcon(AfxGetInstanceHandle(), strExe, 0);

	m_Icon.SetBackgroundColor(RGB(255,255,255), FALSE);
	if (hIcon)
		m_Icon.SetIcon(hIcon, FALSE);

	m_PleaseTellUs.SetBold(TRUE);

	m_What.SetLimitText(128*1024);	// set edit box to 128 KB

    m_ClickHere.SetColours(m_ClickHere.GetLinkColour(),
						   RGB(0,0,255), 
						   m_ClickHere.GetHoverColour());
	m_ClickHere.EnableURL(FALSE);
	m_ClickHere.EnableTooltip(FALSE);
	m_ClickHere.SetNotifyParent(TRUE);
}

///////////////////////////////////////////////////////////////////////////////
// GetFileDetails
void CDlg::GetFileDetails()
{
	// first try to read file details from INI file - 
	// INI file has format:
	//     [FilesToAdd]
	//     File001=<file-path>,<description>,<file-type>
	//     File002=<file-path>,<description>,<file-type>
	//     etc.

	TCHAR szBuf[2000];
	CString strEntry       = _T("");
	CString strPath        = _T("");
	CString strDescription = _T("");
	CString strType        = _T("");
	CString strFileSize    = _T("");

	/*CString strIniPath = m_szModulePath;*/
	CString strIniPath = m_szFullPath;
	strIniPath += INI_FILE_NAME;

	int nCount = 0;

	for (int n = 1; n <= MAX_INI_ITEMS; n++)
	{
		szBuf[0]       = _T('\0');
		strPath        = _T("");
		strDescription = _T("");
		strType        = _T("");
		strFileSize    = _T("");

		strEntry.Format(INI_FILE_TEMPLATE, n);

		::GetPrivateProfileString(INI_FILE_SECTION, 
								  strEntry, 
								  _T(""), 
								  szBuf, 
								  _countof(szBuf)-10,
								  strIniPath);

		if (szBuf[0] == _T('\0'))
			break;

		// get path
		TCHAR *delims = _T(",\r\n");
		TCHAR *cp = _tcstok(szBuf, delims);
		if (!cp)
		{
			TRACE(_T("ERROR - bad file entry\n"));
			break;
		}
		strPath = cp;

		// get description
		cp = _tcstok(NULL, delims);
		if (!cp)
		{
			TRACE(_T("ERROR - bad file entry\n"));
			break;
		}
		strDescription = cp;

		// get type
		cp = _tcstok(NULL, delims);
		if (!cp)
		{
			TRACE(_T("ERROR - bad file entry\n"));
			break;
		}
		strType = cp;

		if (strPath.IsEmpty())
			continue;

		CString strFilePath = strPath;
		if (strPath.Find(_T('\\')) == -1)
		{
			// no path, so use exe's directory
			// strFilePath = m_szModulePath;
			strFilePath= m_szFullPath;
			strFilePath += strPath;
		}

		if (!GetFileSizeAsString(strFilePath, strFileSize))
			continue;

		TRACE(_T("adding file %s from ini\n"), strFilePath);

		FILEDETAILS *fd    = new FILEDETAILS;
		ASSERT(fd);
		fd->strFilePath    = strFilePath;
		fd->strDescription = strDescription;
		fd->strType        = strType;
		fd->strFileSize    = strFileSize;

		m_aFileDetails.Add((DWORD) fd);

		nCount++;
	}

	// if there are no files, try to add the default files -
	// these names are defined in CrashFileNames.h.
	if (nCount == 0)
	{
		TRACE(_T("no files added from ini, using defaults\n"));

		CString strFilePath = m_szFullPath; 
		strFilePath += XCRASHREPORT_ERROR_LOG_FILE;

		if (_taccess(strFilePath, 00) == 0)
		{
			if (GetFileSizeAsString(strFilePath, strFileSize))
			{
				TRACE(_T("adding default file %s\n"), 
					strFilePath);
				FILEDETAILS *fd    = new FILEDETAILS;
				ASSERT(fd);
				fd->strFilePath    = strFilePath;
				fd->strDescription = _T("Registro del Error");
				fd->strType        = _T("Documento de Texto");
				fd->strFileSize    = strFileSize;
				m_aFileDetails.Add((DWORD) fd);
			}
		}

		strFilePath= m_szFullPath;
		strFilePath += XCRASHREPORT_MINI_DUMP_FILE;

		if (_taccess(strFilePath, 00) == 0)
		{
			if (GetFileSizeAsString(strFilePath, strFileSize))
			{
				TRACE(_T("adding default file %s\n"), 
					strFilePath);
				FILEDETAILS *fd    = new FILEDETAILS;
				ASSERT(fd);
				fd->strFilePath    = strFilePath;
				fd->strDescription = _T("Volcado del Error");
				fd->strType        = _T("Archivo DMP");
				fd->strFileSize    = strFileSize;
				m_aFileDetails.Add((DWORD) fd);
			}
		}
	}

#ifdef _DEBUG
	for (int i = 0; i < m_aFileDetails.GetSize(); i++)
	{
		FILEDETAILS *fd = (FILEDETAILS *) m_aFileDetails[i];
		TRACE(_T("fd[%d] = %s\n"), i, fd->strFilePath);
	}
#endif

}

///////////////////////////////////////////////////////////////////////////////
// GetRegistryDetails
void CDlg::GetRegistryDetails()
{
	TRACE(_T("in CDlg::GetRegistryDetails\n"));

// #ifdef XCRASHREPORT_DUMP_REGISTRY

	// first try to read registry details from INI file - 
	// INI file has format:
	//     [RegistryToAdd]
	//     Registry001=<registry-path>,<description>
	//     Registry002=<registry-path>,<description>
	//     etc.
	//
	// For each entry, registry file RegistryNNN.txt
	// will be created.

	TCHAR szBuf[2000];
	CString strEntry       = _T("");
	CString strPath        = _T("");
	CString strDescription = _T("");
	CString strType        = _T("Documento de Texto");	// default for reg files
	CString strFileSize    = _T("");
	CString strFilePath    = _T("");

	// CString strIniPath = m_szModulePath;
	CString strIniPath = m_szFullPath;
	strIniPath += INI_FILE_NAME;

	int nCount = 0;

	for (int n = 1; n <= MAX_INI_ITEMS; n++)
	{
		szBuf[0]       = _T('\0');
		strPath        = _T("");
		strDescription = _T("");
		strFileSize    = _T("");

		strEntry.Format(INI_REG_TEMPLATE, n);

		::GetPrivateProfileString(INI_REG_SECTION, 
								  strEntry, 
								  _T(""), 
								  szBuf, 
								  _countof(szBuf)-10,
								  strIniPath);

		if (szBuf[0] == _T('\0'))
			break;

		// get registry path
		TCHAR *delims = _T(",\r\n");
		TCHAR *cp = _tcstok(szBuf, delims);
		if (!cp)
		{
			TRACE(_T("ERROR - bad registry entry\n"));
			break;
		}

		strPath = cp;

		if (strPath.IsEmpty())
			continue;

		// get description
		cp = _tcstok(NULL, delims);
		if (!cp)
		{
			TRACE(_T("ERROR - bad registry entry\n"));
			break;
		}

		strDescription = cp;

		// dump registry to file - note we do not use ".reg"
		// because we don't want people double-clicking it
		// into the registry
		CString strTemp = _T("");
		strTemp.Format(INI_REG_TEMPLATE, n);
		strFilePath.Format(_T("%s%s.txt"),
			m_szModulePath, strTemp);

		if (!WriteRegistryTreeToFile(strPath, strFilePath))
		{
			TRACE(_T("ERROR - Could not write registry hive %s\n"), strPath);
			continue;
		}

		if (!GetFileSizeAsString(strFilePath, strFileSize))
			continue;

		TRACE(_T("adding reg file %s from ini\n"), strFilePath);

		FILEDETAILS *fd    = new FILEDETAILS;
		ASSERT(fd);
		fd->strFilePath    = strFilePath;
		fd->strDescription = strDescription;
		fd->strType        = strType;
		fd->strFileSize    = strFileSize;

		m_aFileDetails.Add((DWORD) fd);

		nCount++;
	}

	// if there are no files, try to add the default reg entry -
	// this reg path is defined in RegistryDefines.h.
	if (nCount == 0)
	{
		TRACE(_T("no registry files added from ini, using default\n"));

		// dump registry to file
		CString strKey = XCRASHREPORT_REGISTRY_KEY;
		if (!strKey.IsEmpty())
		{
			CString strFile = XCRASHREPORT_REGISTRY_DUMP_FILE;
			if (!strFile.IsEmpty())
			{
				strFilePath= m_szFullPath;
				strFilePath += strFile;

				if (!WriteRegistryTreeToFile(strKey, strFilePath))
				{
					TRACE(_T("ERROR - Could not write registry hive %s\n"), 
						strKey);
				}
				else
				{
					if (GetFileSizeAsString(strFilePath, strFileSize))
					{
						TRACE(_T("adding default reg file %s\n"), 
							strFilePath);
						FILEDETAILS *fd    = new FILEDETAILS;
						ASSERT(fd);
						fd->strFilePath    = strFilePath;
						fd->strDescription = _T("Archivo del registro");
						fd->strType        = strType;
						fd->strFileSize    = strFileSize;
						m_aFileDetails.Add((DWORD) fd);
					}
				}
			}
		}
	}

#ifdef _DEBUG
	for (int i = 0; i < m_aFileDetails.GetSize(); i++)
	{
		FILEDETAILS *fd = (FILEDETAILS *) m_aFileDetails[i];
		TRACE(_T("fd[%d] = %s\n"), i, fd->strFilePath);
	}
#endif

// #endif	// XCRASHREPORT_DUMP_REGISTRY

}

///////////////////////////////////////////////////////////////////////////////
// ZipFiles
int CDlg::ZipFiles()
{
	int nFiles = 0;

	// generate zip file name
	char buffer[MAX_PATH];
	
	GetTempPath( MAX_PATH, buffer);

	m_strZipFile= buffer;
	m_strZipFile += theApp.m_pszModule;
	int index = m_strZipFile.ReverseFind(_T('.'));
	if (index > 0)
		m_strZipFile = m_strZipFile.Left(index);
	m_strZipFile += _T(".zip");
	TRACE(_T("m_strZipPath = <%s>\n"), m_strZipFile);

	// backup old zip if it exists
	BackupFile(m_strZipFile);

	HZIP hZip = NULL;

	for (int i = 0; i < m_aFileDetails.GetSize(); i++)
	{
		FILEDETAILS *fd = (FILEDETAILS *) m_aFileDetails[i];

		if (fd)
		{
			// make sure file exists
			if ((_taccess(fd->strFilePath, 00) == 0) && fd->bSend)
			{
				// if this is first file, create zip now
				if (!hZip)
				{
					hZip = CreateZip((LPVOID)(LPCTSTR)m_strZipFile, 0, ZIP_FILENAME);

					if (!hZip)
					{
						TRACE(_T("ERROR - failed to create %s\n"), m_strZipFile);
						break;
					}
				}

				// zip is open, add file

				CString strName = fd->strFilePath;
				int index = strName.ReverseFind(_T('\\'));
				if (index > 0)
					strName = strName.Right(strName.GetLength()-index-1);
				TRACE(_T("strName=<%s>\n"), strName);

				ZRESULT zr = ZipAdd(hZip, 
									strName, 
									(LPVOID)(LPCTSTR)fd->strFilePath, 
									0, 
									ZIP_FILENAME);
				if (zr == ZR_OK)
				{
					nFiles++;
				}
				else
				{
					TRACE(_T("ERROR - failed to add '%s' to zip\n"), fd->strFilePath);
				}
			}
		}
	}

	if (hZip)
		CloseZip(hZip);

	return nFiles;
}

///////////////////////////////////////////////////////////////////////////////
// OnClickHere
LRESULT CDlg::OnClickHere(WPARAM, LPARAM)
{
	if (m_aFileDetails.GetSize() > 0)
	{
		CDetails dlg(&m_aFileDetails);
		dlg.DoModal();
	}

	return 0;
}

///////////////////////////////////////////////////////////////////////////////
// OnSend
void CDlg::OnSend() 
{
	TCHAR szSubject[1000];
	memset(szSubject, 0, sizeof(szSubject));
	_sntprintf(szSubject, _countof(szSubject)-2, 
		_T("Reporte de Error de %s"), theApp.m_pszModule);

	TCHAR szMessage[MAX_USER_COMMENTS_SIZE];
	memset(szMessage, 0, sizeof(szMessage));
	m_What.GetWindowText(szMessage, _countof(szMessage)-2);

	TCHAR szAttachmentPath[MAX_PATH*2];
	memset(szAttachmentPath, 0, sizeof(szAttachmentPath));

	// create new zip file, in case user unchecked some files
	m_nFilesInZip = ZipFiles();

	if (m_nFilesInZip)
	{
		_tcsncpy(szAttachmentPath, m_strZipFile, _countof(szAttachmentPath)-2);
	}
	else
	{
		// no files in zip - check if there is a message
		if (szMessage[0] == _T('\0'))
		{
			AfxMessageBox(_T("Por favor seleccione un archivo a enviar, o Inserte   \r\n")
						  _T("un mensaje describiendo el problema."));
			return;
		}
	}

	BOOL bRet = TRUE;

	__try
	{
		bRet = SendEmail(m_hWnd,
						 XCRASHREPORT_SEND_TO_ADDRESS, 
						 XCRASHREPORT_SEND_TO_NAME, 
						 szSubject, 
						 szMessage, 
						 szAttachmentPath);
	}
	__except(EXCEPTION_EXECUTE_HANDLER)
	{
		TRACE(_T("ERROR: exception in SendEmail()\n"));
		bRet = FALSE;
	}

	if (!bRet)
	{
		// error - tell user to send file
		TCHAR szMsg[2000];
		memset(szMsg, 0, sizeof(szMsg));
		_sntprintf(szMsg, _countof(szMsg)-2, 
					_T("El reporte de error no fue enviado.  Por favor envienos el archivo\r\n")
					_T("    '%s'\r\n")
					_T("a %s."),
					szAttachmentPath, XCRASHREPORT_SEND_TO_ADDRESS);
		AfxMessageBox(szMsg);
	}

	CDialog::OnOK();
}

///////////////////////////////////////////////////////////////////////////////
// OnDoNotSend
void CDlg::OnDoNotSend() 
{
	CDialog::OnCancel();
}

///////////////////////////////////////////////////////////////////////////////
// OnCancel
void CDlg::OnCancel() 
{
	// do not let Cancel close the dialog	
	//CDialog::OnCancel();
}

///////////////////////////////////////////////////////////////////////////////
// OnOK
void CDlg::OnOK() 
{
	// do not let Enter close the dialog	
	//CDialog::OnOK();
}

///////////////////////////////////////////////////////////////////////////////
// OnAppIcon
void CDlg::OnAppIcon() 
{
	CString strExe = m_szModulePath;
	strExe += theApp.m_pszModule;

	SHELLEXECUTEINFO sei;

	ZeroMemory(&sei,sizeof(sei));
	sei.cbSize = sizeof(sei);
	sei.lpFile = strExe;
	sei.lpVerb = _T("properties");
	sei.fMask  = SEE_MASK_INVOKEIDLIST;
	sei.nShow  = SW_SHOWNORMAL;

	ShellExecuteEx(&sei); 
}

///////////////////////////////////////////////////////////////////////////////
// OnTimer
void CDlg::OnTimer(UINT /*nIDEvent*/) 
{
	CPoint point(GetMessagePos());
	ScreenToClient(&point);

	CRect rect;
	m_Icon.GetWindowRect(&rect);
	ScreenToClient(&rect);

	// check if cursor is over icon
	if (!m_bOverIcon && rect.PtInRect(point))
	{
		m_bOverIcon = TRUE;
		if (m_hLinkCursor)
			m_hPrevCursor = ::SetCursor(m_hLinkCursor);
	}
	else if (m_bOverIcon && !rect.PtInRect(point))
	{
		m_bOverIcon = FALSE;
		if (m_hPrevCursor)
			::SetCursor(m_hPrevCursor);
	}
}

///////////////////////////////////////////////////////////////////////////////
// OnSetCursor
BOOL CDlg::OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message) 
{
	if (m_bOverIcon && m_hLinkCursor)
	{
		::SetCursor(m_hLinkCursor);
		return TRUE;
	}
	return CDialog::OnSetCursor(pWnd, nHitTest, message);
}

///////////////////////////////////////////////////////////////////////////////
// LoadHandCursor
void CDlg::LoadHandCursor()
{
	if (m_hLinkCursor == NULL)				// No cursor handle - try to load one
	{
		// First try to load the Win98 / Windows 2000 hand cursor

		TRACE(_T("loading from IDC_HAND\n"));
		m_hLinkCursor = AfxGetApp()->LoadStandardCursor(IDC_HAND);

		if (m_hLinkCursor == NULL)			// Still no cursor handle - 
											// load the WinHelp hand cursor
		{
			// The following appeared in Paul DiLascia's Jan 1998 MSJ articles.
			// It loads a "hand" cursor from the winhlp32.exe module.

			TRACE(_T("loading from winhlp32\n"));

			// Get the windows directory
			CString strWndDir;
			GetWindowsDirectory(strWndDir.GetBuffer(MAX_PATH), MAX_PATH);
			strWndDir.ReleaseBuffer();

			strWndDir += _T("\\winhlp32.exe");

			// This retrieves cursor #106 from winhlp32.exe, which is a hand pointer
			HMODULE hModule = LoadLibrary(strWndDir);
			if (hModule) 
			{
				HCURSOR hHandCursor = ::LoadCursor(hModule, MAKEINTRESOURCE(106));
				if (hHandCursor)
					m_hLinkCursor = CopyCursor(hHandCursor);
				FreeLibrary(hModule);
			}
		}
	}
}
