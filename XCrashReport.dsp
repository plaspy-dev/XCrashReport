# Microsoft Developer Studio Project File - Name="XCrashReport" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Application" 0x0101

CFG=XCrashReport - Win32 Release
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "XCrashReport.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "XCrashReport.mak" CFG="XCrashReport - Win32 Release"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "XCrashReport - Win32 Release" (based on "Win32 (x86) Application")
!MESSAGE "XCrashReport - Win32 Debug" (based on "Win32 (x86) Application")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "XCrashReport - Win32 Release"

# PROP BASE Use_MFC 6
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 6
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Release"
# PROP Intermediate_Dir "Release"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MD /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_AFXDLL" /Yu"stdafx.h" /FD /c
# ADD CPP /nologo /MD /W4 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_AFXDLL" /D "_MBCS" /Yu"stdafx.h" /FD /c
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x409 /d "NDEBUG" /d "_AFXDLL"
# ADD RSC /l 0x409 /d "NDEBUG" /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /machine:I386
# ADD LINK32 /nologo /subsystem:windows /machine:I386
# Begin Special Build Tool
SOURCE="$(InputPath)"
PostBuild_Desc=Copying exe...
PostBuild_Cmds=copy .\Release\XCrashReport.exe ..\Release
# End Special Build Tool

!ELSEIF  "$(CFG)" == "XCrashReport - Win32 Debug"

# PROP BASE Use_MFC 6
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 6
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Debug"
# PROP Intermediate_Dir "Debug"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MDd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_AFXDLL" /Yu"stdafx.h" /FD /GZ /c
# ADD CPP /nologo /MDd /W4 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_AFXDLL" /D "_MBCS" /Yu"stdafx.h" /FD /GZ /c
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x409 /d "_DEBUG" /d "_AFXDLL"
# ADD RSC /l 0x409 /d "_DEBUG" /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /debug /machine:I386 /pdbtype:sept
# ADD LINK32 /nologo /subsystem:windows /debug /machine:I386 /pdbtype:sept
# Begin Special Build Tool
SOURCE="$(InputPath)"
PostBuild_Desc=Copying exe...
PostBuild_Cmds=copy .\Debug\XCrashReport.exe ..\Debug
# End Special Build Tool

!ENDIF 

# Begin Target

# Name "XCrashReport - Win32 Release"
# Name "XCrashReport - Win32 Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=.\BackupFile.cpp
# End Source File
# Begin Source File

SOURCE=.\Details.cpp
# End Source File
# Begin Source File

SOURCE=.\Dlg.cpp
# End Source File
# Begin Source File

SOURCE=.\FontSize.cpp
# End Source File
# Begin Source File

SOURCE=.\FormatNumberForLocale.cpp
# End Source File
# Begin Source File

SOURCE=.\GetFilePart.cpp
# SUBTRACT CPP /YX /Yc /Yu
# End Source File
# Begin Source File

SOURCE=.\GetFileSizeAsString.cpp
# End Source File
# Begin Source File

SOURCE=.\IsAsciiFile.cpp
# End Source File
# Begin Source File

SOURCE=.\MiniVersion.cpp
# End Source File
# Begin Source File

SOURCE=.\ROEdit.cpp
# End Source File
# Begin Source File

SOURCE=.\SendEmail.cpp
# SUBTRACT CPP /YX /Yc /Yu
# End Source File
# Begin Source File

SOURCE=.\StdAfx.cpp
# ADD CPP /Yc"stdafx.h"
# End Source File
# Begin Source File

SOURCE=.\strcvt.cpp
# SUBTRACT CPP /YX /Yc /Yu
# End Source File
# Begin Source File

SOURCE=.\WriteRegistry.cpp
# End Source File
# Begin Source File

SOURCE=.\XColorStatic.cpp
# End Source File
# Begin Source File

SOURCE=.\XCrashReport.cpp
# End Source File
# Begin Source File

SOURCE=.\XCrashReport.rc
# End Source File
# Begin Source File

SOURCE=.\XHeaderCtrl.cpp
# End Source File
# Begin Source File

SOURCE=.\XHyperLink.cpp
# End Source File
# Begin Source File

SOURCE=.\XListCtrl.cpp
# End Source File
# Begin Source File

SOURCE=.\XZip.cpp
# SUBTRACT CPP /YX /Yc /Yu
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=.\BackupFile.h
# End Source File
# Begin Source File

SOURCE=.\CrashFileNames.h
# End Source File
# Begin Source File

SOURCE=.\Details.h
# End Source File
# Begin Source File

SOURCE=.\Dlg.h
# End Source File
# Begin Source File

SOURCE=.\EmailDefines.h
# End Source File
# Begin Source File

SOURCE=.\FontSize.h
# End Source File
# Begin Source File

SOURCE=.\FormatNumberForLocale.h
# End Source File
# Begin Source File

SOURCE=.\GetFilePart.h
# End Source File
# Begin Source File

SOURCE=.\GetFileSizeAsString.h
# End Source File
# Begin Source File

SOURCE=.\IniDefines.h
# End Source File
# Begin Source File

SOURCE=.\IsAsciiFile.h
# End Source File
# Begin Source File

SOURCE=.\MemDC.h
# End Source File
# Begin Source File

SOURCE=.\MiniVersion.h
# End Source File
# Begin Source File

SOURCE=.\RegistryDefines.h
# End Source File
# Begin Source File

SOURCE=.\Resource.h
# End Source File
# Begin Source File

SOURCE=.\ROEdit.h
# End Source File
# Begin Source File

SOURCE=.\SendEmail.h
# End Source File
# Begin Source File

SOURCE=.\StdAfx.h
# End Source File
# Begin Source File

SOURCE=.\strcvt.h
# End Source File
# Begin Source File

SOURCE=.\WriteRegistry.h
# End Source File
# Begin Source File

SOURCE=.\XColorStatic.h
# End Source File
# Begin Source File

SOURCE=.\XCrashReport.h
# End Source File
# Begin Source File

SOURCE=.\XHeaderCtrl.h
# End Source File
# Begin Source File

SOURCE=.\XHyperLink.h
# End Source File
# Begin Source File

SOURCE=.\XListCtrl.h
# End Source File
# Begin Source File

SOURCE=.\XTrace.h
# End Source File
# Begin Source File

SOURCE=.\XZip.h
# End Source File
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;rgs;gif;jpg;jpeg;jpe"
# Begin Source File

SOURCE=.\checkboxes.bmp
# End Source File
# Begin Source File

SOURCE=.\XCrashReport.ico
# End Source File
# End Group
# Begin Source File

SOURCE=.\app.manifest
# End Source File
# End Target
# End Project
