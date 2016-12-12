@echo off
setlocal
set INCLUDES=-Ilibs\GLFW\include
set LIBS=opengl32.lib libs\GLFW\lib-vc2015\glfw3dll.lib
set LIBDIRS=-libpath:libs\GLFW\lib-vc2015

REM Subsystem:Console
cl -nologo -Zi -Fdobj\ -Foobj\ %INCLUDES% -Febuffer src/main.c src/buffer.c %LIBS% -link -incremental:no %LIBDIRS% -nodefaultlib:msvcrt

REM Subsystem:Windows
REM cl -nologo -Zi -Fdobj\ -Foobj\ %INCLUDES% -Febuffer src/main.c src/buffer.c %LIBS% -link -incremental:no %LIBDIRS% -nodefaultlib:msvcrt -SUBSYSTEM:windows -ENTRY:mainCRTStartup

endlocal