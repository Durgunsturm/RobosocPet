@echo off
SETLOCAL ENABLEDELAYEDEXPANSION

mkdir "lib-pack"
echo. > "lib-pack\petlib.h"
echo #include "petlib.h" > "lib-pack\petlib.c"
for /r lib\ %%? in ("*.c") do (
        echo %%~sn?
        echo. >> "lib-pack\petlib.c"
        for /f "tokens=*" %%i in (%%~f?) do (
                for /f %%j in ("%%i") do (
                    if not "%%j" == "#include" echo %%i >> "lib-pack\petlib.c"
                )
        )
)
for /r lib\ %%? in ("*.h") do (
        echo %%~sn?
        echo. >> "lib-pack\petlib.h"
        type %%~f? >> "lib-pack\petlib.h"
)
robocopy .\lib-pack %UserProfile%\Documents\Arduino\libraries\petlib
pause