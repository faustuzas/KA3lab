tasm.exe /l mytest.asm
tlink.exe /t mytest.obj

tasm.exe main
tlink.exe main
del *.obj
del *.map
main.exe mytest.com testrez.asm