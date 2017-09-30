echo F | xcopy ".\UpdateTemp\Client" ".\" /y /f
echo F | xcopy ".\UpdateTemp\Client\update\common.dll" ".\update\common.dll" /y /f
echo F | xcopy ".\UpdateTemp\Client\update\DuiLib.dll" ".\update\DuiLib.dll" /y /f
echo F | xcopy ".\UpdateTemp\Client\update\glog.dll" ".\update\glog.dll" /y /f
echo F | xcopy ".\UpdateTemp\Client\update\LogManager.dll" ".\update\LogManager.dll" /y /f
echo F | xcopy ".\UpdateTemp\Client\update\mfc120u.dll" ".\update\mfc120u.dll" /y /f
echo F | xcopy ".\UpdateTemp\Client\update\msvcp100.dll" ".\update\msvcp100.dll" /y /f
echo F | xcopy ".\UpdateTemp\Client\update\msvcp120.dll" ".\update\msvcp120.dll" /y /f
echo F | xcopy ".\UpdateTemp\Client\update\msvcr100.dll" ".\update\msvcr100.dll" /y /f
echo F | xcopy ".\UpdateTemp\Client\update\msvcr120.dll" ".\update\msvcr120.dll" /y /f
echo F | xcopy ".\UpdateTemp\Client\update\update.exe" ".\update\update.exe" /y /f
echo F | xcopy ".\UpdateTemp\Client\update\zlib.dll" ".\update\zlib.dll" /y /f

