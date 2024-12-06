@echo off
mkdir "lib-pack"
echo. > "lib-pack\lib.h"
echo. > "lib-pack\lib.c"
for /r lib\ %%? in ("*.h") do (
        echo %%~sn?
        echo. >> "lib-pack\lib.h"
        type %%~f? >> "lib-pack\lib.h"
)
for /r lib\ %%? in ("*.c") do (
        echo %%~sn?
        echo. >> "lib-pack\lib.c"
        type %%~f? >> "lib-pack\lib.c"
)
rem robocopy .\lib-pack %UserProfile%\Documents\Arduino\libraries\petlib
pause