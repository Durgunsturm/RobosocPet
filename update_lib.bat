@echo off
SETLOCAL ENABLEDELAYEDEXPANSION

mkdir "lib-pack" > nul
echo #pragma once > "lib-pack\petlib.h"
echo #include "petlib.h" > "lib-pack\petlib.cpp"
echo #include ^<Arduino.h^> >> "lib-pack\petlib.h"
for /r lib\ %%? in ("*.cpp") do (
        echo %%~sn?
        echo. >> "lib-pack\petlib.cpp"
        for /f "tokens=*" %%i in (%%~f?) do (
                for /f %%j in ("%%i") do (
                    if not "%%j" == "#include" echo %%i >> "lib-pack\petlib.cpp"
                )
        )
)
for /r lib\ %%? in ("*.h") do (
        echo %%~sn?
        echo. >> "lib-pack\petlib.h"
        type %%~f? >> "lib-pack\petlib.h"
)
robocopy .\lib-pack %UserProfile%\Documents\Arduino\libraries\petlib > nul
pause