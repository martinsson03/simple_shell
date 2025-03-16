@echo off
if not exist build mkdir build
gcc -O0 -g src\*.c  -I.\headers -o build\simple_shell.exe -Wall