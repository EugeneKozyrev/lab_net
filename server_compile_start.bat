@echo off
if exist server.exe (
	del /f /q server.exe
)
g++ server.cpp -o server -lws2_32
if not %errorlevel% == 0 (
	echo:
	echo Error: Can't compile server.cpp
	echo:
	pause
	goto :EOF
)
echo:
echo Info: Compile server.cpp successed
echo:
echo Info: Start server.exe
echo:
cmd /c server.exe
pause
:EOF
