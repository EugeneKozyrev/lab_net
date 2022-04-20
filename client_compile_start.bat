@echo off
SETLOCAL
set /a "number=1"
ENDLOCAL
set "client_exe=client_%number%.exe"

if exist %client_exe% (
	echo "%client_exe% exists"
	echo "%number% before"
  SETLOCAL
  set /a "number=10"
  ENDLOCAL
  echo "%number% after"
  set "client_exe=client_%number%.exe"
  echo "Now client is %client_exe%"
)
g++ client.cpp -o %client_exe% -lws2_32
if not %errorlevel% == 0 (
	echo:
	echo Error: Can't compile client.cpp
	echo:
	pause
	goto :EOF
)
echo:
echo Info: Compile client.cpp successed
echo:
echo Info: Start %client_exe%
echo:
cmd /c %client_exe%
pause
:EOF
