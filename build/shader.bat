@echo off
"C:\Program Files (x86)\Windows Kits\10\bin\10.0.19041.0\x86\fxc.exe" ^
/T vs_5_0 /E vs_main /Fh ../include/vshader.h ../src/shader.vsh
"C:\Program Files (x86)\Windows Kits\10\bin\10.0.19041.0\x86\fxc.exe" ^
/T ps_5_0 /E ps_main /Fh ../include/pshader.h ../src/shader.psh 