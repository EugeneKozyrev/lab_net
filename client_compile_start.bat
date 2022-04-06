@echo off
g++ client.cpp -o client -lws2_32
if not %errorlevel% == 0 (
	echo:
	echo Error: Can't compile client.cpp
	echo:
	pause
	exit
)
echo:
echo Info: Compile client.cpp successed
echo:
echo Info: Start client.exe
echo:
cmd /c client.exe
pause
