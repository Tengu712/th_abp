@echo off
rc /fo resource.res ../res/resource.rc
rc /fo resource_private.res ../res/resource_private.rc
link /NOENTRY /MACHINE:X86 /DLL resource.res
del resource.res