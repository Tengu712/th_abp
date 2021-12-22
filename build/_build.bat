@echo off
setlocal enabledelayedexpansion

set kOptInclude=^
/I "C:\Program Files (x86)\Microsoft Visual Studio\2022\BuildTools\VC\Tools\MSVC\14.30.30705\include" ^
/I "C:\Program Files (x86)\Microsoft Visual Studio\2022\BuildTools\VC\Auxiliary\VS\include" ^
/I "C:\Program Files (x86)\Windows Kits\10\Include\10.0.19041.0\ucrt" ^
/I "C:\Program Files (x86)\Windows Kits\10\Include\10.0.19041.0\um" ^
/I "C:\Program Files (x86)\Windows Kits\10\Include\10.0.19041.0\shared" ^
/I "C:\Program Files (x86)\Windows Kits\10\Include\10.0.19041.0\winrt" ^
/I "C:\Program Files (x86)\Windows Kits\10\Include\10.0.19041.0\cppwinrt"
set kOptLibrary=/link ^
/LIBPATH:"C:\Program Files (x86)\Microsoft Visual Studio\2022\BuildTools\VC\Tools\MSVC\14.30.30705\lib\x86" ^
/LIBPATH:"C:\Program Files (x86)\Microsoft Visual Studio\2022\BuildTools\VC\Auxiliary\VS\lib\x86" ^
/LIBPATH:"C:\Program Files (x86)\Windows Kits\10\lib\10.0.19041.0\ucrt\x86" ^
/LIBPATH:"C:\Program Files (x86)\Windows Kits\10\lib\10.0.19041.0\um\x86" ^
/LIBPATH:"."

set delimiter= 
set pathFiles=
if exist "..\src\*.cpp" (
    for /f "tokens=* delims=" %%x in ('dir ..\src\*.cpp /S /B') do (
        set pathFiles=!pathFiles!%%x!delimiter!
    )
)

if "!pathFiles!"=="" (
    echo There is no file that should be compiled.
    exit /B
)

cl /EHsc /Fe:th_abp.exe resource_private.res !kOptInclude! !pathFiles! !kOptLibrary!
del *.obj
pause