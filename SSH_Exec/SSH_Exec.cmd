@echo off

for /l %%i in (101 1 120) do SSH_Exec.exe 192.168.10.%%i 22 pi huode

echo.
echo Press any key to quit...
pause>nul

