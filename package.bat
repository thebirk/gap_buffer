@echo off
call build.bat
if not exist package mkdir package
copy buffer.exe package\
copy *.dll package\
copy *.ttf package\
copy *.txt package\