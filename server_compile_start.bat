@echo off
g++ server.cpp -o server -lws2_32
if not %errorlevel% == 0 (
	echo:
	echo Error: Can't compile server.cpp
	echo:
	pause
	exit
)
echo:
echo Info: Compile server.cpp successed
echo:
echo Info: Start server.exe
echo:
cmd /c server.exe
pause
