@echo off
setlocal enabledelayedexpansion

set kOptInclude=^
/I "C:\Program Files (x86)\Microsoft Visual Studio\2022\BuildTools\VC\Tools\MSVC\14.30.30705\include" ^
/I "C:\Program Files (x86)\Windows Kits\10\Include\10.0.19041.0\ucrt" ^
/I "C:\Program Files (x86)\Windows Kits\10\Include\10.0.19041.0\um" ^
/I "C:\Program Files (x86)\Windows Kits\10\Include\10.0.19041.0\shared" ^
/I "C:\Program Files (x86)\Windows Kits\10\Include\10.0.19041.0\winrt"

set delimiter= 
set pathFiles=
if exist "..\directx\*.cpp" (
    for /f "tokens=* delims=" %%x in ('dir ..\directx\*.cpp /S /B') do (
        set pathFiles=!pathFiles!%%x!delimiter!
    )
)

if "!pathFiles!"=="" (
    echo There is no file that should be compiled.
    exit /B
)

cl /c /EHsc !kOptInclude! !pathFiles!

set pathFilesObj=
if exist ".\*.obj" (
    for /f "tokens=* delims=" %%x in ('dir .\*.obj /S /B') do (
        set pathFilesObj=!pathFilesObj!%%x!delimiter!
    )
)

lib /out:.\objs\mydx.lib !pathFilesObj!
del *.obj
pause