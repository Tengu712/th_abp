@echo off
rc ../res/resource.rc
link /NOENTRY /MACHINE:X86 /DLL ../res/resource.res