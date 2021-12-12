@echo off
fxc /T vs_5_0 /E vs_main /Fh ../directx/_vshader.h ../directx/shader.vsh
fxc /T ps_5_0 /E ps_main /Fh ../directx/_pshader.h ../directx/shader.psh 