@echo off

if [%1] == [] goto noParams

set compile=%1
goto validatePath

:noParams
if "%compile%"=="" goto undefinedPath
goto validatePath

:undefinedPath
set compile=basic
goto validatePath
echo please specify file name
goto exit

:validatePath
if exist c:\examples\%compile% goto compile
echo folder examples\%compile% doesnt exist
goto exit

:compile
cd c:\examples\%compile%
c:\tasm\bin\tasm.exe /zi /m2 /t /ic:\ main.asm
c:\tasm\bin\tlink.exe /v main.obj

cd c:\
echo compilation complete

:exit
echo.
