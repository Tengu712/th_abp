@echo off
rc /fo resource.res ../res/resource.rc
link /NOENTRY /MACHINE:X86 /DLL resource.res
del resource.res