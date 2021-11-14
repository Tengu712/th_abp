@echo off
"C:\Program Files (x86)\Windows Kits\10\bin\10.0.19041.0\x86\fxc.exe" ^
/T vs_5_0 /E vs_main /Fh ../directx/vshader.h ../directx/shader.vsh
"C:\Program Files (x86)\Windows Kits\10\bin\10.0.19041.0\x86\fxc.exe" ^
/T ps_5_0 /E ps_main /Fh ../directx/pshader.h ../directx/shader.psh 