set QB=C:\Qt\5.15.2\msvc2019
set QDIR=%QB%\bin
set QDLL=.dll
set DST=.\RTE
mkdir %DST%
copy %QDIR%\Qt5Widgets%QDLL% %DST%
copy %QDIR%\Qt5Gui%QDLL% %DST%
copy %QDIR%\Qt5Core%QDLL% %DST%
copy %QDIR%\Qt5Network%QDLL% %DST%
copy %QDIR%\Qt5SerialPort%QDLL% %DST%
copy %QDIR%\Qt5SerialBus%QDLL% %DST%
REM copy %QDIR%\Qt5Xml%QDLL% %DST%

mkdir %DST%\platforms
copy %QB%\plugins\platforms\qwindows%QDLL% %DST%\platforms\qwindows%QDLL%
copy ..\build-QtDPS3005Rest-Desktop_Qt_5_15_2_MSVC2019_32bit-Release\release\*.exe %DST%
pause