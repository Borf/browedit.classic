# Microsoft Developer Studio Project File - Name="roworldedit" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Console Application" 0x0103

CFG=roworldedit - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "roworldedit.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "roworldedit.mak" CFG="roworldedit - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "roworldedit - Win32 Release" (based on "Win32 (x86) Console Application")
!MESSAGE "roworldedit - Win32 Debug" (based on "Win32 (x86) Console Application")
!MESSAGE "roworldedit - Win32 Release_nocheck" (based on "Win32 (x86) Console Application")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
RSC=rc.exe

!IF  "$(CFG)" == "roworldedit - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "../obj/vs6/Release"
# PROP Intermediate_Dir "../obj/vs6/Release"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_CONSOLE" /D "_MBCS" /YX /FD /c
# ADD CPP /nologo /MD /W3 /GX /O2 /I "src" /I "libs/include" /I "..\libs\include\stlport" /I "..\src" /I "..\libs/include" /D "WIN32_LEAN_AND_MEAN" /D "WIN32" /D "NDEBUG" /D "_CONSOLE" /D "_MBCS" /D "GRF_STATIC" /FD /c
# SUBTRACT CPP /YX
# ADD BASE RSC /l 0x409 /d "NDEBUG"
# ADD RSC /l 0x409 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /machine:I386
# ADD LINK32 plugin_base.lib libcurl.lib SDL_mixer.lib ws2_32.lib sdl.lib sdlmain.lib zlib.lib bgd.lib opengl32.lib glu32.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /debug /debugtype:both /machine:I386 /out:"..\roworldedit.exe" /libpath:"libs/lib" /libpath:"..\libs/lib"
# Begin Special Build Tool
SOURCE="$(InputPath)"
PreLink_Cmds=..\svnver ..\src/version.h
# End Special Build Tool

!ELSEIF  "$(CFG)" == "roworldedit - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "../obj/vs6/Debug"
# PROP Intermediate_Dir "../obj/vs6/Debug"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_CONSOLE" /D "_MBCS" /YX /FD /GZ /c
# ADD CPP /nologo /MDd /W3 /Gm /Gi /GR /GX /ZI /Od /I "..\libs\include\stlport" /I "..\src" /I "..\libs/include" /D "WIN32_LEAN_AND_MEAN" /D "WIN32" /D "_DEBUG" /D "_CONSOLE" /D "_MBCS" /D "GRF_STATIC" /FR /FD /GZ /c
# SUBTRACT CPP /WX /YX
# ADD BASE RSC /l 0x409 /d "_DEBUG"
# ADD RSC /l 0x409 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /debug /machine:I386 /pdbtype:sept
# ADD LINK32 plugin_baseD.lib libcurl.lib sdl_mixer.lib ws2_32.lib bgd.lib sdl.lib sdlmain.lib zlib.lib opengl32.lib glaux.lib glu32.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib SHLWAPI.LIB /nologo /subsystem:console /pdb:"..\roworldeditd.pdb" /debug /machine:I386 /nodefaultlib:"MSVCRT" /nodefaultlib:"LIBCMT" /out:"..\roworldeditd.exe" /pdbtype:sept /libpath:"..\libs/lib"
# SUBTRACT LINK32 /pdb:none
# Begin Special Build Tool
SOURCE="$(InputPath)"
PreLink_Cmds=..\svnver ..\src/version.h
PostBuild_Desc=svn version...
PostBuild_Cmds=..\svnver.exe ..\src\version.h
# End Special Build Tool

!ELSEIF  "$(CFG)" == "roworldedit - Win32 Release_nocheck"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "roworldedit___Win32_Release_nocheck"
# PROP BASE Intermediate_Dir "roworldedit___Win32_Release_nocheck"
# PROP BASE Ignore_Export_Lib 0
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "../obj/vs6/Release_nocheck"
# PROP Intermediate_Dir "../obj/vs6/Release_nocheck"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MD /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_CONSOLE" /D "_MBCS" /D "GRF_STATIC" /YX /FD /c
# ADD CPP /nologo /MD /W3 /GX /Zi /O2 /I "..\libs\include\stlport" /I "..\src" /I "..\libs/include" /D "WIN32_LEAN_AND_MEAN" /D "WIN32" /D "NDEBUG" /D "_CONSOLE" /D "_MBCS" /D "GRF_STATIC" /D "_NOCHECK_" /FR /FD /c
# SUBTRACT CPP /YX
# ADD BASE RSC /l 0x409 /d "NDEBUG"
# ADD RSC /l 0x409 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 ws2_32.lib sdl.lib sdlmain.lib zlib.lib bgd.lib opengl32.lib glu32.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /machine:I386
# ADD LINK32 plugin_base.lib libcurl.lib SDL_Mixer.lib ws2_32.lib sdl.lib sdlmain.lib zlib.lib bgd.lib opengl32.lib glu32.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /debug /machine:I386 /out:"..\roworldedit.exe" /libpath:"..\..\libs\lib" /libpath:"..\libs/lib"

!ENDIF 

# Begin Target

# Name "roworldedit - Win32 Release"
# Name "roworldedit - Win32 Debug"
# Name "roworldedit - Win32 Release_nocheck"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Group "eventhandlers"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\src\detailheightedit.cpp
# End Source File
# Begin Source File

SOURCE=..\src\effectedit.cpp
# End Source File
# Begin Source File

SOURCE=..\src\gatedit.cpp
# End Source File
# Begin Source File

SOURCE=..\src\globalheightedit.cpp
# End Source File
# Begin Source File

SOURCE=..\src\lightedit.cpp
# End Source File
# Begin Source File

SOURCE=..\src\objectedit.cpp
# End Source File
# Begin Source File

SOURCE=..\src\objectgroupedit.cpp
# End Source File
# Begin Source File

SOURCE=..\src\soundedit.cpp
# End Source File
# Begin Source File

SOURCE=..\src\spriteedit.cpp
# End Source File
# Begin Source File

SOURCE=..\src\textureedit.cpp
# End Source File
# Begin Source File

SOURCE=..\src\texturepaintedit.cpp
# End Source File
# Begin Source File

SOURCE=..\src\walledit.cpp
# End Source File
# Begin Source File

SOURCE=..\src\wateredit.cpp
# End Source File
# End Group
# Begin Source File

SOURCE=..\src\bmutex.cpp
# End Source File
# Begin Source File

SOURCE=..\src\bthread.cpp
# End Source File
# Begin Source File

SOURCE=..\src\clipboard.cpp
# End Source File
# Begin Source File

SOURCE=..\src\common.cpp
# End Source File
# Begin Source File

SOURCE=..\src\filesystem.cpp
# End Source File
# Begin Source File

SOURCE=..\src\font.cpp
# End Source File
# Begin Source File

SOURCE=..\src\frustum.cpp
# End Source File
# Begin Source File

SOURCE=..\src\graphics.cpp
# End Source File
# Begin Source File

SOURCE=..\src\interfaceimplementation.cpp
# End Source File
# Begin Source File

SOURCE=..\src\main.cpp
# End Source File
# Begin Source File

SOURCE=..\src\memorymanagement.cpp
# End Source File
# Begin Source File

SOURCE=..\src\menu.cpp
# End Source File
# Begin Source File

SOURCE=..\src\menucommands.cpp
# End Source File
# Begin Source File

SOURCE=..\src\mymath.cpp
# End Source File
# Begin Source File

SOURCE=..\src\RSMModel.cpp
# End Source File
# Begin Source File

SOURCE=..\src\settings.cpp
# End Source File
# Begin Source File

SOURCE=..\src\sprite.cpp
# End Source File
# Begin Source File

SOURCE=..\src\StackWalker.cpp
# End Source File
# Begin Source File

SOURCE=..\src\svnver.cpp
# End Source File
# Begin Source File

SOURCE=..\src\texture.cpp
# End Source File
# Begin Source File

SOURCE=..\src\texturecache.cpp
# End Source File
# Begin Source File

SOURCE=..\src\texturemodel.cpp
# End Source File
# Begin Source File

SOURCE=..\src\undo.cpp
# End Source File
# Begin Source File

SOURCE=..\src\world.cpp
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=..\src\bmutex.h
# End Source File
# Begin Source File

SOURCE=..\src\bthread.h
# End Source File
# Begin Source File

SOURCE=..\src\clipboard.h
# End Source File
# Begin Source File

SOURCE=..\src\common.h
# End Source File
# Begin Source File

SOURCE=..\src\filesystem.h
# End Source File
# Begin Source File

SOURCE=..\src\font.h
# End Source File
# Begin Source File

SOURCE=..\src\frustum.h
# End Source File
# Begin Source File

SOURCE=..\src\graphics.h
# End Source File
# Begin Source File

SOURCE=..\src\interfaceimplementation.h
# End Source File
# Begin Source File

SOURCE=..\src\menu.h
# End Source File
# Begin Source File

SOURCE=..\src\menucommands.h
# End Source File
# Begin Source File

SOURCE=..\src\mymath.h
# End Source File
# Begin Source File

SOURCE=..\src\RSMModel.h
# End Source File
# Begin Source File

SOURCE=..\src\settings.h
# End Source File
# Begin Source File

SOURCE=..\src\sprite.h
# End Source File
# Begin Source File

SOURCE=..\src\StackWalker.h
# End Source File
# Begin Source File

SOURCE=..\src\svnver.h
# End Source File
# Begin Source File

SOURCE=..\src\texture.h
# End Source File
# Begin Source File

SOURCE=..\src\texturecache.h
# End Source File
# Begin Source File

SOURCE=..\src\texturemodel.h
# End Source File
# Begin Source File

SOURCE=..\src\undo.h
# End Source File
# Begin Source File

SOURCE=..\src\version.h
# End Source File
# Begin Source File

SOURCE=..\src\world.h
# End Source File
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;rgs;gif;jpg;jpeg;jpe"
# Begin Source File

SOURCE=..\src\icon1.ico
# End Source File
# Begin Source File

SOURCE=..\src\resource.h
# End Source File
# Begin Source File

SOURCE=..\src\Script1.rc
# End Source File
# End Group
# Begin Group "grflib"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\src\grflib\grf.c
# End Source File
# Begin Source File

SOURCE=..\src\grflib\grf.h
# End Source File
# Begin Source File

SOURCE=..\src\grflib\grfcrypt.c
# End Source File
# Begin Source File

SOURCE=..\src\grflib\grfcrypt.h
# End Source File
# Begin Source File

SOURCE=..\src\grflib\grfsupport.c
# End Source File
# Begin Source File

SOURCE=..\src\grflib\grfsupport.h
# End Source File
# Begin Source File

SOURCE=..\src\grflib\grftypes.h
# End Source File
# Begin Source File

SOURCE=..\src\grflib\rgz.c
# End Source File
# Begin Source File

SOURCE=..\src\grflib\rgz.h
# End Source File
# End Group
# Begin Group "tinyxml"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\src\tinyxml\tinystr.cpp
# End Source File
# Begin Source File

SOURCE=..\src\tinyxml\tinystr.h
# End Source File
# Begin Source File

SOURCE=..\src\tinyxml\tinyxml.cpp
# End Source File
# Begin Source File

SOURCE=..\src\tinyxml\tinyxml.h
# End Source File
# Begin Source File

SOURCE=..\src\tinyxml\tinyxmlerror.cpp
# End Source File
# Begin Source File

SOURCE=..\src\tinyxml\tinyxmlparser.cpp
# End Source File
# End Group
# Begin Group "textureloaders"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\src\textureloaders\bitmaptextureloader.cpp
# End Source File
# Begin Source File

SOURCE=..\src\textureloaders\bitmaptextureloader.h
# End Source File
# Begin Source File

SOURCE=..\src\textureloaders\giftextureloader.cpp
# End Source File
# Begin Source File

SOURCE=..\src\textureloaders\giftextureloader.h
# End Source File
# Begin Source File

SOURCE=..\src\textureloaders\jpegtextureloader.cpp
# End Source File
# Begin Source File

SOURCE=..\src\textureloaders\jpegtextureloader.h
# End Source File
# Begin Source File

SOURCE=..\src\textureloaders\textureloader.cpp
# End Source File
# Begin Source File

SOURCE=..\src\textureloaders\textureloader.h
# End Source File
# Begin Source File

SOURCE=..\src\textureloaders\tgatextureloader.cpp
# End Source File
# Begin Source File

SOURCE=..\src\textureloaders\tgatextureloader.h
# End Source File
# End Group
# Begin Group "wm"

# PROP Default_Filter ""
# Begin Group "windows"

# PROP Default_Filter ""
# Begin Group "headers"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\src\windows\ambientlightwindow.h
# End Source File
# Begin Source File

SOURCE=..\src\windows\areacopywindow.h
# End Source File
# Begin Source File

SOURCE=..\src\windows\confirmwindow.h
# End Source File
# Begin Source File

SOURCE=..\src\windows\effectwindow.h
# End Source File
# Begin Source File

SOURCE=..\src\windows\favoritelights.h
# End Source File
# Begin Source File

SOURCE=..\src\windows\filewindow.h
# End Source File
# Begin Source File

SOURCE=..\src\windows\hotkeywindow.h
# End Source File
# Begin Source File

SOURCE=..\src\windows\inputwindow.h
# End Source File
# Begin Source File

SOURCE=..\src\windows\keybindwindow.h
# End Source File
# Begin Source File

SOURCE=..\src\windows\lightoverviewwindow.h
# End Source File
# Begin Source File

SOURCE=..\src\windows\lightwindow.h
# End Source File
# Begin Source File

SOURCE=..\src\windows\mapdetailwindow.h
# End Source File
# Begin Source File

SOURCE=..\src\windows\mapswindow.h
# End Source File
# Begin Source File

SOURCE=..\src\windows\messagewindow.h
# End Source File
# Begin Source File

SOURCE=..\src\windows\minimapwindow.h
# End Source File
# Begin Source File

SOURCE=..\src\windows\modeloverviewwindow.h
# End Source File
# Begin Source File

SOURCE=..\src\windows\modelswindow.h
# End Source File
# Begin Source File

SOURCE=..\src\windows\objectwindow.h
# End Source File
# Begin Source File

SOURCE=..\src\windows\progresswindow.h
# End Source File
# Begin Source File

SOURCE=..\src\windows\rsmeditwindow.h
# End Source File
# Begin Source File

SOURCE=..\src\windows\soundoverviewwindow.h
# End Source File
# Begin Source File

SOURCE=..\src\windows\soundselectwindow.h
# End Source File
# Begin Source File

SOURCE=..\src\windows\soundwindow.h
# End Source File
# Begin Source File

SOURCE=..\src\windows\spritewindow.h
# End Source File
# Begin Source File

SOURCE=..\src\windows\texturebrushwindow.h
# End Source File
# Begin Source File

SOURCE=..\src\windows\texturetoolswindow.h
# End Source File
# Begin Source File

SOURCE=..\src\windows\texturewindow.h
# End Source File
# Begin Source File

SOURCE=..\src\windows\walltexturewindow.h
# End Source File
# Begin Source File

SOURCE=..\src\windows\waterwindow.h
# End Source File
# Begin Source File

SOURCE=..\src\windows\xmlwindow.h
# End Source File
# End Group
# Begin Source File

SOURCE=..\src\windows\ambientlightwindow.cpp
# End Source File
# Begin Source File

SOURCE=..\src\windows\confirmwindow.cpp
# End Source File
# Begin Source File

SOURCE=..\src\windows\effectwindow.cpp
# End Source File
# Begin Source File

SOURCE=..\src\windows\favoritelights.cpp
# End Source File
# Begin Source File

SOURCE=..\src\windows\filewindow.cpp
# End Source File
# Begin Source File

SOURCE=..\src\windows\hotkeywindow.cpp
# End Source File
# Begin Source File

SOURCE=..\src\windows\inputwindow.cpp
# End Source File
# Begin Source File

SOURCE=..\src\windows\keybindwindow.cpp
# End Source File
# Begin Source File

SOURCE=..\src\windows\lightoverviewwindow.cpp
# End Source File
# Begin Source File

SOURCE=..\src\windows\lightwindow.cpp
# End Source File
# Begin Source File

SOURCE=..\src\windows\mapdetailwindow.cpp
# End Source File
# Begin Source File

SOURCE=..\src\windows\mapswindow.cpp
# End Source File
# Begin Source File

SOURCE=..\src\windows\messagewindow.cpp
# End Source File
# Begin Source File

SOURCE=..\src\windows\minimapwindow.cpp
# End Source File
# Begin Source File

SOURCE=..\src\windows\modeloverviewwindow.cpp
# End Source File
# Begin Source File

SOURCE=..\src\windows\modelswindow.cpp
# End Source File
# Begin Source File

SOURCE=..\src\windows\objectwindow.cpp
# End Source File
# Begin Source File

SOURCE=..\src\windows\progresswindow.cpp
# End Source File
# Begin Source File

SOURCE=..\src\windows\rsmeditwindow.cpp
# End Source File
# Begin Source File

SOURCE=..\src\windows\soundoverviewwindow.cpp
# End Source File
# Begin Source File

SOURCE=..\src\windows\soundselectwindow.cpp
# End Source File
# Begin Source File

SOURCE=..\src\windows\soundwindow.cpp
# End Source File
# Begin Source File

SOURCE=..\src\windows\spritewindow.cpp
# End Source File
# Begin Source File

SOURCE=..\src\windows\texturebrushwindow.cpp
# End Source File
# Begin Source File

SOURCE=..\src\windows\texturetoolswindow.cpp
# End Source File
# Begin Source File

SOURCE=..\src\windows\texturewindow.cpp
# End Source File
# Begin Source File

SOURCE=..\src\windows\walltexturewindow.cpp
# End Source File
# Begin Source File

SOURCE=..\src\windows\waterwindow.cpp
# End Source File
# Begin Source File

SOURCE=..\src\windows\xmlwindow.cpp
# End Source File
# End Group
# Begin Group "components"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\src\wm\windowbutton.cpp
# End Source File
# Begin Source File

SOURCE=..\src\wm\windowbutton.h
# End Source File
# Begin Source File

SOURCE=..\src\wm\windowcheckbox.cpp
# End Source File
# Begin Source File

SOURCE=..\src\wm\windowcheckbox.h
# End Source File
# Begin Source File

SOURCE=..\src\wm\windowframe.cpp
# End Source File
# Begin Source File

SOURCE=..\src\wm\windowframe.h
# End Source File
# Begin Source File

SOURCE=..\src\wm\windowinputbox.cpp
# End Source File
# Begin Source File

SOURCE=..\src\wm\windowinputbox.h
# End Source File
# Begin Source File

SOURCE=..\src\wm\windowlabel.cpp
# End Source File
# Begin Source File

SOURCE=..\src\wm\windowlabel.h
# End Source File
# Begin Source File

SOURCE=..\src\wm\windowline.h
# End Source File
# Begin Source File

SOURCE=..\src\wm\windowlistbox.cpp
# End Source File
# Begin Source File

SOURCE=..\src\wm\windowlistbox.h
# End Source File
# Begin Source File

SOURCE=..\src\wm\windowmainbutton.cpp
# End Source File
# Begin Source File

SOURCE=..\src\wm\windowmainbutton.h
# End Source File
# Begin Source File

SOURCE=..\src\wm\windowobject.cpp
# End Source File
# Begin Source File

SOURCE=..\src\wm\windowobject.h
# End Source File
# Begin Source File

SOURCE=..\src\wm\windowpicturebox.cpp
# End Source File
# Begin Source File

SOURCE=..\src\wm\windowpicturebox.h
# End Source File
# Begin Source File

SOURCE=..\src\wm\windowprogressbar.cpp
# End Source File
# Begin Source File

SOURCE=..\src\wm\windowprogressbar.h
# End Source File
# Begin Source File

SOURCE=..\src\wm\windowrgbpicker.cpp
# End Source File
# Begin Source File

SOURCE=..\src\wm\windowrgbpicker.h
# End Source File
# Begin Source File

SOURCE=..\src\wm\windowroundbutton.cpp
# End Source File
# Begin Source File

SOURCE=..\src\wm\windowroundbutton.h
# End Source File
# Begin Source File

SOURCE=..\src\wm\windowscrollpanel.cpp
# End Source File
# Begin Source File

SOURCE=..\src\wm\windowscrollpanel.h
# End Source File
# Begin Source File

SOURCE=..\src\wm\windowtabpanel.cpp
# End Source File
# Begin Source File

SOURCE=..\src\wm\windowtabpanel.h
# End Source File
# Begin Source File

SOURCE=..\src\wm\windowtree.cpp
# End Source File
# Begin Source File

SOURCE=..\src\wm\windowtree.h
# End Source File
# End Group
# Begin Source File

SOURCE=..\src\wm\window.cpp
# End Source File
# Begin Source File

SOURCE=..\src\wm\window.h
# End Source File
# Begin Source File

SOURCE=..\src\wm\wm.cpp
# End Source File
# Begin Source File

SOURCE=..\src\wm\wm.h
# End Source File
# End Group
# Begin Group "undo"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\src\undo\effectchange.h
# End Source File
# Begin Source File

SOURCE=..\src\undo\effectdelete.h
# End Source File
# Begin Source File

SOURCE=..\src\undo\effectsdelete.h
# End Source File
# Begin Source File

SOURCE=..\src\undo\gatheightedit.h
# End Source File
# Begin Source File

SOURCE=..\src\undo\gattileedit.h
# End Source File
# Begin Source File

SOURCE=..\src\undo\heightedit.h
# End Source File
# Begin Source File

SOURCE=..\src\undo\lightchange.h
# End Source File
# Begin Source File

SOURCE=..\src\undo\lightdelete.h
# End Source File
# Begin Source File

SOURCE=..\src\undo\lightnew.h
# End Source File
# Begin Source File

SOURCE=..\src\undo\objectchange.h
# End Source File
# Begin Source File

SOURCE=..\src\undo\objectdelete.h
# End Source File
# Begin Source File

SOURCE=..\src\undo\objectnew.h
# End Source File
# Begin Source File

SOURCE=..\src\undo\objectschange.h
# End Source File
# Begin Source File

SOURCE=..\src\undo\objectsdelete.h
# End Source File
# Begin Source File

SOURCE=..\src\undo\objectsnew.h
# End Source File
# Begin Source File

SOURCE=..\src\undo\soundchange.h
# End Source File
# Begin Source File

SOURCE=..\src\undo\sounddelete.h
# End Source File
# Begin Source File

SOURCE=..\src\undo\soundnew.h
# End Source File
# Begin Source File

SOURCE=..\src\undo\texture.h
# End Source File
# Begin Source File

SOURCE=..\src\undo\tileedit.h
# End Source File
# Begin Source File

SOURCE=..\src\undo\wallchange.h
# End Source File
# Begin Source File

SOURCE=..\src\undo\wallschange.h
# End Source File
# Begin Source File

SOURCE=..\src\undo\waterchange.h
# End Source File
# End Group
# Begin Source File

SOURCE=..\Makefile
# End Source File
# End Target
# End Project
