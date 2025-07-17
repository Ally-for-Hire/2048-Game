@echo off
echo Building 2048 Game...
C:\msys64\ucrt64\bin\g++.exe -fdiagnostics-color=always -O2 -s main.cpp Window.cpp -o 2048_game.exe -lgdi32 -luser32 -lkernel32 -static-libgcc -static-libstdc++

if %ERRORLEVEL% EQU 0 (
    echo Build successful! Running game...
    echo.
    2048_game.exe
) else (
    echo Build failed!
    pause
)
