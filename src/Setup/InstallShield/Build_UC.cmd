@echo off

set rar_path="C:\Program Files\WinRAR"
set PATH=%PATH%;%rar_path%

REM ######## NOTICE: PLEASE CHANGE THIS VERSION NUMBER BEFORE PUBLISHING/DEPLOYING CLIENT INSTALLER ########
set filename=1.0.0_alpha1
set compressedfile=UCCLIENT_UPDATE_%filename%.zip

set dt=%date%
set tm=%time%
echo. ============= Build Start: %dt% %tm% ============= 
echo.

if exist %compressedfile% del %compressedfile%
if exist OUTPUT rmdir /s /q OUTPUT
if exist TEMP rmdir /S /Q TEMP

if exist windowclient.tar (goto :continue) else (echo Not found windowclient.tar)
Pause
exit

:continue

mkdir OUTPUT
mkdir TEMP

"C:\Program Files\WinRAR\WinRAR.exe" x windowclient.tar TEMP\

rmdir /s /q TEMP\windowclient\html

echo.
echo. ============ setp 1 : copy binary files=============== 
echo.

copy Binary\GVPrinter.exe OUTPUT\GVPrinter.exe
copy Binary\CookieOperator.exe OUTPUT\CookieOperator.exe
copy Binary\refresh.exe OUTPUT\refresh.exe
REM copy Binary\TailProcess.bat OUTPUT\TailProcess.bat

echo.
echo. ============ setp 2 : Checkout files from svn===================
echo.

REM svn export --username="huajian.yin" "http://192.168.11.216/svn/SME-MEETING/code/branches/v0.1/clientbuild/Printer" "OUTPUT\Printer"

svn export --username="huajian.yin" "http://192.168.11.203/svn/Client_frame/uc1.0/trunk/pcclient/html" "TEMP\windowclient\html"

svn export --username="huajian.yin" "http://192.168.11.203/svn/Client_frame/uc1.0/branches/WindowsClient/Installer/UI" "TEMP\windowclient\UI"

REM if exist "OUTPUT\Printer\gvprproc.dll" (goto :continue) else (echo Failed to export files from svn.)
REM Pause
REM exit

:continue

echo.
echo. ============ setp 3 : Signature files ===================
echo.

signcode -s my -cn G-NET "OUTPUT\GVPrinter.exe"
REM signcode -s my -cn G-NET "OUTPUT\Printer\gvprproc.dll"
REM signcode -s my -cn G-NET "OUTPUT\Printer\i386\gvprinter.dll"
signcode -s my -cn G-NET "OUTPUT\CookieOperator.exe"
signcode -s my -cn G-NET "OUTPUT\refresh.exe"
signcode -s my -cn G-NET "TEMP\windowclient\LiveUpgrade.exe"
signcode -s my -cn G-NET "TEMP\windowclient\UCClient.exe"
signcode -s my -cn G-NET "TEMP\windowclient\UCChat.exe"
signcode -s my -cn G-NET "TEMP\windowclient\UCUpdate.exe"
signcode -s my -cn G-NET "TEMP\windowclient\Sqlite3.dll"
signcode -s my -cn G-NET "TEMP\windowclient\LogUpload.exe"
signcode -s my -cn G-NET "TEMP\windowclient\LogDll.dll"
signcode -s my -cn G-NET "TEMP\windowclient\StartInfoRecorder.dll"
signcode -s my -cn G-NET "TEMP\windowclient\plugin\AVModule.dll"
signcode -s my -cn G-NET "TEMP\windowclient\plugin\DesktopSharing.dll"
signcode -s my -cn G-NET "TEMP\windowclient\plugin\DocConvertLib.dll"
signcode -s my -cn G-NET "TEMP\windowclient\plugin\DocSharing.dll"
signcode -s my -cn G-NET "TEMP\windowclient\plugin\MediaEngine.dll"
signcode -s my -cn G-NET "TEMP\windowclient\plugin\Text.dll"
signcode -s my -cn G-NET "TEMP\windowclient\plugin\Whiteboard.dll"
signcode -s my -cn G-NET "TEMP\windowclient\plugin\Conference.dll"
signcode -s my -cn G-NET "TEMP\windowclient\plugin\AVWizard.dll"
signcode -s my -cn G-NET "TEMP\windowclient\plugin\CatchScreen.dll"
signcode -s my -cn G-NET "TEMP\windowclient\plugin\ScreenSpyDLL.dll"
signcode -s my -cn G-NET "TEMP\windowclient\plugin\DesktopCodec.dll"
signcode -s my -cn G-NET "TEMP\windowclient\plugin\voice_engine_core.dll"

echo.
echo. ============ setp 4 : Copy,Move files ===================
echo.
mkdir OUTPUT\Printer
xcopy Binary\Printer\ OUTPUT\Printer\ /s /y /e
move "TEMP\windowclient\LiveUpgrade.exe" "OUTPUT\LiveUpgrade.exe"
copy "TEMP\windowclient\Sqlite3.dll" "OUTPUT\Sqlite3.dll"
copy "TEMP\windowclient\LogDll.dll" "OUTPUT\LogDll.dll"
copy "TEMP\windowclient\Microsoft.VC80.ATL.manifest" "OUTPUT\Microsoft.VC80.ATL.manifest"
copy "TEMP\windowclient\Microsoft.VC80.CRT.manifest" "OUTPUT\Microsoft.VC80.CRT.manifest"
copy "TEMP\windowclient\msvcp80.dll" "OUTPUT\msvcp80.dll"
copy "TEMP\windowclient\msvcr80.dll" "OUTPUT\msvcr80.dll"

mkdir OUTPUT\Update

move "TEMP\windowclient\UCUpdate.exe" "OUTPUT\Update\UCUpdate.exe"
move "TEMP\windowclient\UCDaemonSvc.exe" "OUTPUT\Update\UCDaemonSvc.exe"
move "TEMP\windowclient\UCUpdate.tlb" "OUTPUT\Update\UCUpdate.tlb"
move "TEMP\windowclient\UCUpdate.exe.manifest" "OUTPUT\Update\UCUpdate.exe.manifest"
copy "TEMP\windowclient\Microsoft.VC80.ATL.manifest" "OUTPUT\Update\Microsoft.VC80.ATL.manifest"
copy "TEMP\windowclient\Microsoft.VC80.CRT.manifest" "OUTPUT\Update\Microsoft.VC80.CRT.manifest"
copy "TEMP\windowclient\msvcp80.dll" "OUTPUT\Update\msvcp80.dll"
copy "TEMP\windowclient\msvcr80.dll" "OUTPUT\Update\msvcr80.dll"
copy "TEMP\windowclient\ATL80.dll" "OUTPUT\Update\ATL80.dll"


copy "TEMP\windowclient\LogDll.dll" "OUTPUT\Update\LogDll.dll"
copy "TEMP\windowclient\TangConfig.xml" "OUTPUT\Update\TangConfig.xml"
copy "TEMP\windowclient\LogConfig.xml" "OUTPUT\Update\LogConfig.xml"

copy "Site.xml" "OUTPUT\Site.xml"

copy UCCookies.xml OUTPUT\UCCookies.xml

echo.
echo. ============ setp 5 : Create Config xml file ===================
echo.

REM (1)Read Site.xml(default, or use /f:XML_FILE_NAME to set), get 'hold', siteid' 'appid' 'skinid' 'version'
REM (2)create 'appid\siteid' folder
REM (3)Copy TEMP\windowclient\*.* to OUTPUT\appid\siteid\*.*
REM (4)Create Config.xml to OUTPUT\ and OUTPUT\appid\siteid\
ConfigGene.exe

echo.
echo. ============ step 6 : Create compressed file of completed client package ======================= 
echo

WinRAR a -r -ep1 %compressedfile% OUTPUT\*.*

REM 考虑到非云会议用户安装时不覆盖1.x的Site.xml，先安装Site2.xml
REM 如果没有安装1.x，在文件拷贝完成后，再将Site2.xml改名成Site.xml
REM 这个逻辑由安装脚本完成，详情参考QuanshiMeeting_User_Setup_Script.nsi
REM rename OUTPUT\Site.xml OUTPUT\Site2.xml

REM copy MeetNowUserCookies.xml OUTPUT\MeetNowUserCookies.xml
REM 非云会议用户安装包不能、也不必直接双击TangRoute进入
REM shit! the product manager change her mind! so comment the following statement.
REM del OUTPUT\Site.xml

REM rename OUTPUT\Site2.xml OUTPUT\Site.xml

set dt=%date%
set dt1=%dt:-=%
set dt2=%dt1: =%
set dt3=%dt2:~4,4%

set tm=%time%
set tm1=%tm::=%
set tm2=%tm1:.=%
set tm3=%tm2:~0,4%
set tm3=%tm3: =0%

echo.
echo. ============ Build Finished: %dt% %tm% ============ 
echo.

REM rmdir /s /q OUTPUT
REM rmdir /s /q TEMP

echo CONGRATULATIONS! ALL DONE.

pause
