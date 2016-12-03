# Microsoft Developer Studio Project File - Name="FrontCad" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Application" 0x0101

CFG=FrontCad - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "FrontCad.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "FrontCad.mak" CFG="FrontCad - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "FrontCad - Win32 Release" (based on "Win32 (x86) Application")
!MESSAGE "FrontCad - Win32 Debug" (based on "Win32 (x86) Application")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "FrontCad - Win32 Release"

# PROP BASE Use_MFC 5
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 5
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Release"
# PROP Intermediate_Dir "Release"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MT /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /Yu"stdafx.h" /FD /c
# ADD CPP /nologo /MT /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /Yu"stdafx.h" /FD /c
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x409 /d "NDEBUG"
# ADD RSC /l 0x409 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /machine:I386
# ADD LINK32 /nologo /subsystem:windows /machine:I386

!ELSEIF  "$(CFG)" == "FrontCad - Win32 Debug"

# PROP BASE Use_MFC 5
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 5
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Debug"
# PROP Intermediate_Dir "Debug"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MTd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /Yu"stdafx.h" /FD /GZ /c
# ADD CPP /nologo /MTd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /Yu"stdafx.h" /FD /GZ /c
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x409 /d "_DEBUG"
# ADD RSC /l 0x409 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /debug /machine:I386 /pdbtype:sept
# ADD LINK32 /nologo /subsystem:windows /debug /machine:I386 /pdbtype:sept

!ENDIF 

# Begin Target

# Name "FrontCad - Win32 Release"
# Name "FrontCad - Win32 Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=.\ArcTan.cpp
# End Source File
# Begin Source File

SOURCE=.\ButtonMsg.cpp
# End Source File
# Begin Source File

SOURCE=.\CadArc.cpp
# End Source File
# Begin Source File

SOURCE=.\CadArcCentered.cpp
# End Source File
# Begin Source File

SOURCE=.\CadBitmap.cpp
# End Source File
# Begin Source File

SOURCE=.\CadDrawing.cpp
# End Source File
# Begin Source File

SOURCE=.\CadElipse.cpp
# End Source File
# Begin Source File

SOURCE=.\CadHoleRnd1Flat.cpp
# End Source File
# Begin Source File

SOURCE=.\CadHoleRnd2Flat.cpp
# End Source File
# Begin Source File

SOURCE=.\CadHoleRound.cpp
# End Source File
# Begin Source File

SOURCE=.\CadLibObject.cpp
# End Source File
# Begin Source File

SOURCE=.\CadLibrary.cpp
# End Source File
# Begin Source File

SOURCE=.\CadLine.cpp
# End Source File
# Begin Source File

SOURCE=.\CadObject.cpp
# End Source File
# Begin Source File

SOURCE=.\CadPolygon.cpp
# End Source File
# Begin Source File

SOURCE=.\CadPolygonFill.cpp
# End Source File
# Begin Source File

SOURCE=.\CadRect.cpp
# End Source File
# Begin Source File

SOURCE=.\CadRectHole.cpp
# End Source File
# Begin Source File

SOURCE=.\CadRoundRect.cpp
# End Source File
# Begin Source File

SOURCE=.\CadText.cpp
# End Source File
# Begin Source File

SOURCE=.\ComboBoxFontWeight.cpp
# End Source File
# Begin Source File

SOURCE=.\ComboBoxHoleType.cpp
# End Source File
# Begin Source File

SOURCE=.\ComboBoxLibPartSelector.cpp
# End Source File
# Begin Source File

SOURCE=.\ComboBoxLibrary.cpp
# End Source File
# Begin Source File

SOURCE=.\DialogChoose.cpp
# End Source File
# Begin Source File

SOURCE=.\DialogGridSettings.cpp
# End Source File
# Begin Source File

SOURCE=.\EditDecimal.cpp
# End Source File
# Begin Source File

SOURCE=.\FileParser.cpp
# End Source File
# Begin Source File

SOURCE=.\FrontCad.cpp
# End Source File
# Begin Source File

SOURCE=.\FrontCad.rc
# End Source File
# Begin Source File

SOURCE=.\FrontCadDoc.cpp
# End Source File
# Begin Source File

SOURCE=.\FrontCadView.cpp
# End Source File
# Begin Source File

SOURCE=.\HorzScrollBar.cpp
# End Source File
# Begin Source File

SOURCE=.\LibFormView.cpp
# End Source File
# Begin Source File

SOURCE=.\LibSplitterWnd.cpp
# End Source File
# Begin Source File

SOURCE=.\MainFrm.cpp
# End Source File
# Begin Source File

SOURCE=.\MoveObjects.cpp
# End Source File
# Begin Source File

SOURCE=.\MyBitmap.cpp
# End Source File
# Begin Source File

SOURCE=.\MyComboBox.cpp
# End Source File
# Begin Source File

SOURCE=.\MyEdit.cpp
# End Source File
# Begin Source File

SOURCE=.\MySplitterWnd.cpp
# End Source File
# Begin Source File

SOURCE=.\MyTabCtrl.cpp
# End Source File
# Begin Source File

SOURCE=.\NewLibDialog.cpp
# End Source File
# Begin Source File

SOURCE=.\RoundRectPropDialog.cpp
# End Source File
# Begin Source File

SOURCE=.\ruler.cpp
# End Source File
# Begin Source File

SOURCE=.\SaveToLibDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\Scale.cpp
# End Source File
# Begin Source File

SOURCE=.\ScaleWizDialog.cpp
# End Source File
# Begin Source File

SOURCE=.\ScaleWizTabOne.cpp
# End Source File
# Begin Source File

SOURCE=.\SplitterUtility.cpp
# End Source File
# Begin Source File

SOURCE=.\StaticBitmap.cpp
# End Source File
# Begin Source File

SOURCE=.\StaticPreview.cpp
# End Source File
# Begin Source File

SOURCE=.\StdAfx.cpp
# ADD CPP /Yc"stdafx.h"
# End Source File
# Begin Source File

SOURCE=.\UtilView.cpp
# End Source File
# Begin Source File

SOURCE=.\WizTab2Dialog.cpp
# End Source File
# Begin Source File

SOURCE=.\WizTab3Dialog.cpp
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=.\ButtonMsg.h
# End Source File
# Begin Source File

SOURCE=.\CadArc.h
# End Source File
# Begin Source File

SOURCE=.\CadArcCentered.h
# End Source File
# Begin Source File

SOURCE=.\CadBitmap.h
# End Source File
# Begin Source File

SOURCE=.\CadDrawing.h
# End Source File
# Begin Source File

SOURCE=.\CadElipse.h
# End Source File
# Begin Source File

SOURCE=.\CadHoleRnd1Flat.h
# End Source File
# Begin Source File

SOURCE=.\CadHoleRnd2Flat.h
# End Source File
# Begin Source File

SOURCE=.\CadHoleRound.h
# End Source File
# Begin Source File

SOURCE=.\CadLibObject.h
# End Source File
# Begin Source File

SOURCE=.\CadLibrary.h
# End Source File
# Begin Source File

SOURCE=.\CadLine.h
# End Source File
# Begin Source File

SOURCE=.\CadObject.h
# End Source File
# Begin Source File

SOURCE=.\CadPolygon.h
# End Source File
# Begin Source File

SOURCE=.\CadPolygonFill.h
# End Source File
# Begin Source File

SOURCE=.\CadRect.h
# End Source File
# Begin Source File

SOURCE=.\CadRectHole.h
# End Source File
# Begin Source File

SOURCE=.\CadRoundRect.h
# End Source File
# Begin Source File

SOURCE=.\CadText.h
# End Source File
# Begin Source File

SOURCE=.\ComboBoxFontWeight.h
# End Source File
# Begin Source File

SOURCE=.\ComboBoxHoleType.h
# End Source File
# Begin Source File

SOURCE=.\ComboBoxLibPartSelector.h
# End Source File
# Begin Source File

SOURCE=.\ComboBoxLibrary.h
# End Source File
# Begin Source File

SOURCE=.\DialogChoose.h
# End Source File
# Begin Source File

SOURCE=.\DialogGridSettings.h
# End Source File
# Begin Source File

SOURCE=.\EditDecimal.h
# End Source File
# Begin Source File

SOURCE=.\FileParser.h
# End Source File
# Begin Source File

SOURCE=.\FrontCad.h
# End Source File
# Begin Source File

SOURCE=.\FrontCadDefines.h
# End Source File
# Begin Source File

SOURCE=.\FrontCadDoc.h
# End Source File
# Begin Source File

SOURCE=.\FrontCadView.h
# End Source File
# Begin Source File

SOURCE=.\HorzScrollBar.h
# End Source File
# Begin Source File

SOURCE=.\LibFormView.h
# End Source File
# Begin Source File

SOURCE=.\LibSplitterWnd.h
# End Source File
# Begin Source File

SOURCE=.\MainFrm.h
# End Source File
# Begin Source File

SOURCE=.\MoveObjects.h
# End Source File
# Begin Source File

SOURCE=.\MyBitmap.h
# End Source File
# Begin Source File

SOURCE=.\MyComboBox.h
# End Source File
# Begin Source File

SOURCE=.\MyEdit.h
# End Source File
# Begin Source File

SOURCE=.\MySplitterWnd.h
# End Source File
# Begin Source File

SOURCE=.\MyTabCtrl.h
# End Source File
# Begin Source File

SOURCE=.\NewLibDialog.h
# End Source File
# Begin Source File

SOURCE=.\Resource.h
# End Source File
# Begin Source File

SOURCE=.\RoundRectPropDialog.h
# End Source File
# Begin Source File

SOURCE=.\ruler.h
# End Source File
# Begin Source File

SOURCE=.\SaveToLibDlg.h
# End Source File
# Begin Source File

SOURCE=.\Scale.h
# End Source File
# Begin Source File

SOURCE=.\ScaleWizDialog.h
# End Source File
# Begin Source File

SOURCE=.\ScaleWizTabOne.h
# End Source File
# Begin Source File

SOURCE=.\SplitterUtility.h
# End Source File
# Begin Source File

SOURCE=.\StaticBitmap.h
# End Source File
# Begin Source File

SOURCE=.\StaticPreview.h
# End Source File
# Begin Source File

SOURCE=.\StdAfx.h
# End Source File
# Begin Source File

SOURCE=.\UtilView.h
# End Source File
# Begin Source File

SOURCE=.\WizTab2Dialog.h
# End Source File
# Begin Source File

SOURCE=.\WizTab3Dialog.h
# End Source File
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;rgs;gif;jpg;jpeg;jpe"
# Begin Source File

SOURCE=.\res\FrontCad.ico
# End Source File
# Begin Source File

SOURCE=.\res\FrontCad.rc2
# End Source File
# Begin Source File

SOURCE=.\res\FrontCadDoc.ico
# End Source File
# Begin Source File

SOURCE=.\res\Toolbar.bmp
# End Source File
# End Group
# Begin Source File

SOURCE=.\FrontCad.reg
# End Source File
# Begin Source File

SOURCE=.\ReadMe.txt
# End Source File
# End Target
# End Project
