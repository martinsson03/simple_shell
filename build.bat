@echo off
if not exist build mkdir build
gcc src\main.c -o build\simple_shell.exe -Wall