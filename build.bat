@echo off
if not exist build mkdir build
gcc -O0 -g src\main.c -o build\simple_shell.exe -Wall