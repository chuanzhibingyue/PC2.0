echo off

set mode=debug
set nodeps=false
set configuration=Test\Release
set operation=start

:parseargs
if "%~1"=="" goto endparseargs

if %~1==debug (
	set mode=debug
	set configuration=Test\Debug
)

if %~1==release (
	set mode=release
	set configuration=Test\Release
)

if %~1==nodeps (
	set nodeps=true
)

if %~1==stop (
	set operation=stop
)

if %~1==restart (
	set operation=restart
)

if %~1==setDeps (
	set operation=setDeps
)

shift
goto parseargs
:endparseargs

echo operation: %operation%, mode: %mode%, nodeps: %nodeps%

if %operation%==stop (
	echo stoping ucclient

	taskkill /f /im bee.exe
	taskkill /f /im bee.worker.exe

	goto exit
)

if %operation%==restart (
	echo restarting ucclient

	taskkill /f /im bee.exe
	taskkill /f /im bee.worker.exe

	goto run
)

if %nodeps%==false (
	echo copy dependences	

	rd /s /q %configuration%\locales
	md %configuration%\locales
	copy ..\third_party\cef\Resources\locales\*  %configuration%\locales
	copy ..\third_party\cef\Resources\cef.pak %configuration%\cef.pak
	copy ..\third_party\cef\Resources\devtools_resources.pak %configuration%\devtools_resources.pak

	copy  ..\third_party\cef\lib\%configuration%\d3dcompiler_43.dll %configuration%\d3dcompiler_43.dll
	copy  ..\third_party\cef\lib\%configuration%\d3dcompiler_46.dll %configuration%\d3dcompiler_46.dll
	copy  ..\third_party\cef\lib\%configuration%\icudt.dll %configuration%\icudt.dll
	copy  ..\third_party\cef\lib\%configuration%\libcef.dll %configuration%\libcef.dll
	copy  ..\third_party\cef\lib\%configuration%\libEGL.dll %configuration%\libEGL.dll
	copy  ..\third_party\cef\lib\%configuration%\libGLESv2.dll %configuration%\libGLESv2.dll
	
	rd /s /q %configuration%\plugin
	md %configuration%\plugin
	copy  ..\third_party\cef\plugin\*	%configuration%\plugin
	
	copy  ..\third_party\uc_sdk\third_party\cert\oncloud2CA.pem %configuration%\oncloud2CA.pem
	copy  ..\third_party\uc_sdk\third_party\pclib\release\Bee2MeetingPlugin.dll %configuration%\Bee2MeetingPlugin.dll
	
	rd /s /q %configuration%\resources\sharingSkin
	md %configuration%\resources\sharingSkin
	copy  ..\src\UCClient\res\sharingSkin\* %configuration%\resources\sharingSkin
	
	
	
	if %mode%==debug (		
		copy  ..\third_party\uc_sdk\lib\%configuration%\glog_d.dll %configuration%\glog_d.dll		
		copy  ..\third_party\uc_sdk\lib\%configuration%\UCSDK_d.dll %configuration%\UCSDK_d.dll		
		copy  ..\third_party\uc_sdk\lib\%configuration%\sqlite3.dll %configuration%\sqlite3.dll		
		copy  ..\third_party\uc_sdk\lib\%configuration%\zlib_d.dll %configuration%\zlib_d.dll
	)

	if %mode%==release (
		
		copy  ..\third_party\uc_sdk\lib\%configuration%\glog.dll %configuration%\glog.dll		
		copy  ..\third_party\uc_sdk\lib\%configuration%\UCSDK.dll %configuration%\UCSDK.dll		
		copy  ..\third_party\uc_sdk\lib\%configuration%\sqlite3.dll %configuration%\sqlite3.dll		
		copy  ..\third_party\uc_sdk\lib\%configuration%\zlib.dll %configuration%\zlib.dll
	)
)

if %operation%==setDeps (
	goto exit
)

if exist %configuration%\bee.exe (
	if exist %configuration%\bee.worker.exe (
		if exist %configuration%\CefIntegration.dll (
			goto run
		)
		else (
			echo %configuration%\CefIntegration.dll not found
			goto exit
		)
	)
	else (
		echo %configuration%\bee.worker.exe not found
		goto exit
	)
)
else (
	echo %configuration%\bee.exe not found
	goto exit
)


:run
echo starting ucclient
start %configuration%\bee.exe

:exit
