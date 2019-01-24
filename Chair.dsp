# Microsoft Developer Studio Project File - Name="Chair" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Console Application" 0x0103

CFG=Chair - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "Chair.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "Chair.mak" CFG="Chair - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "Chair - Win32 Release" (based on "Win32 (x86) Console Application")
!MESSAGE "Chair - Win32 Debug" (based on "Win32 (x86) Console Application")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
RSC=rc.exe

!IF  "$(CFG)" == "Chair - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Release"
# PROP Intermediate_Dir "Release"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_CONSOLE" /D "_MBCS" /YX /FD /c
# ADD CPP /nologo /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /YX /FD /c
# ADD BASE RSC /l 0x409 /d "NDEBUG"
# ADD RSC /l 0x409 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /machine:I386
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /machine:I386 /out:"Chair.exe"
# SUBTRACT LINK32 /pdb:none

!ELSEIF  "$(CFG)" == "Chair - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Debug"
# PROP Intermediate_Dir "Debug"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_CONSOLE" /D "_MBCS" /YX /FD /GZ /c
# ADD CPP /nologo /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /FR /YX /FD /GZ /c
# ADD BASE RSC /l 0x409 /d "_DEBUG"
# ADD RSC /l 0x409 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /debug /machine:I386 /pdbtype:sept
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /debug /machine:I386 /pdbtype:sept
# SUBTRACT LINK32 /pdb:none

!ENDIF 

# Begin Target

# Name "Chair - Win32 Release"
# Name "Chair - Win32 Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=.\Board.cpp
# End Source File
# Begin Source File

SOURCE=.\dlgSimple.cpp
# End Source File
# Begin Source File

SOURCE=.\Game.cpp
# End Source File
# Begin Source File

SOURCE=.\Main.cpp
# End Source File
# Begin Source File

SOURCE=.\Procedure.cpp
# End Source File
# Begin Source File

SOURCE=.\Room.cpp
# End Source File
# Begin Source File

SOURCE=.\RoomSet.cpp
# End Source File
# Begin Source File

SOURCE=.\View.cpp
# End Source File
# Begin Source File

SOURCE=.\World.cpp
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=.\Board.h
# End Source File
# Begin Source File

SOURCE=.\dlgCommon.h
# End Source File
# Begin Source File

SOURCE=.\dlgSimple.h
# End Source File
# Begin Source File

SOURCE=.\Game.h
# End Source File
# Begin Source File

SOURCE=.\List.h
# End Source File
# Begin Source File

SOURCE=.\Main.h
# End Source File
# Begin Source File

SOURCE=.\Procedure.h
# End Source File
# Begin Source File

SOURCE=.\Room.h
# End Source File
# Begin Source File

SOURCE=.\RoomSet.h
# End Source File
# Begin Source File

SOURCE=.\View.h
# End Source File
# Begin Source File

SOURCE=.\World.h
# End Source File
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;rgs;gif;jpg;jpeg;jpe"
# Begin Source File

SOURCE=.\art\black.bmp
# End Source File
# Begin Source File

SOURCE=.\art\chair.bmp
# End Source File
# Begin Source File

SOURCE=.\art\Medium\chair.bmp
# End Source File
# Begin Source File

SOURCE=.\art\Small\chair.bmp
# End Source File
# Begin Source File

SOURCE=.\art\chair.ico
# End Source File
# Begin Source File

SOURCE=.\art\hint.bmp
# End Source File
# Begin Source File

SOURCE=.\art\icon1.ico
# End Source File
# Begin Source File

SOURCE=.\art\instructions.bmp
# End Source File
# Begin Source File

SOURCE=.\art\lastblack.bmp
# End Source File
# Begin Source File

SOURCE=.\art\lastwhite.bmp
# End Source File
# Begin Source File

SOURCE=.\lights.ico
# End Source File
# Begin Source File

SOURCE=.\art\Medium\money.bmp
# End Source File
# Begin Source File

SOURCE=.\art\money.bmp
# End Source File
# Begin Source File

SOURCE=.\art\Small\money.bmp
# End Source File
# Begin Source File

SOURCE=.\art\off.bmp
# End Source File
# Begin Source File

SOURCE=.\art\on.bmp
# End Source File
# Begin Source File

SOURCE=.\art\Medium\person.bmp
# End Source File
# Begin Source File

SOURCE=.\art\person.bmp
# End Source File
# Begin Source File

SOURCE=.\art\Small\person.bmp
# End Source File
# Begin Source File

SOURCE=.\art\Medium\personspot.bmp
# End Source File
# Begin Source File

SOURCE=.\art\personspot.bmp
# End Source File
# Begin Source File

SOURCE=.\art\Small\personspot.bmp
# End Source File
# Begin Source File

SOURCE=.\art\possible.bmp
# End Source File
# Begin Source File

SOURCE=.\resource.rc
# End Source File
# Begin Source File

SOURCE=.\Reversi.ico
# End Source File
# Begin Source File

SOURCE=.\art\smallblack.bmp
# End Source File
# Begin Source File

SOURCE=.\art\smallwhite.bmp
# End Source File
# Begin Source File

SOURCE=.\art\Medium\spot.bmp
# End Source File
# Begin Source File

SOURCE=.\art\Small\spot.bmp
# End Source File
# Begin Source File

SOURCE=.\art\spot.bmp
# End Source File
# Begin Source File

SOURCE=.\art\title.bmp
# End Source File
# Begin Source File

SOURCE=.\art\Medium\wall.bmp
# End Source File
# Begin Source File

SOURCE=.\art\Small\wall.bmp
# End Source File
# Begin Source File

SOURCE=.\art\wall.bmp
# End Source File
# Begin Source File

SOURCE=.\art\white.bmp
# End Source File
# End Group
# Begin Group "Libary Sources"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\lib\App.cpp
# End Source File
# Begin Source File

SOURCE=.\lib\other\Bitmap.cpp
# End Source File
# Begin Source File

SOURCE=.\lib\winproc.cpp
# End Source File
# End Group
# Begin Group "Library Headers"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\lib\App.h
# End Source File
# Begin Source File

SOURCE=.\lib\other\Bitmap.h
# End Source File
# Begin Source File

SOURCE=.\lib\other\Canvas.h
# End Source File
# Begin Source File

SOURCE=.\lib\Menuitem.h
# End Source File
# Begin Source File

SOURCE=.\lib\other\Tools.h
# End Source File
# Begin Source File

SOURCE=.\lib\other\winex.h
# End Source File
# End Group
# End Target
# End Project
