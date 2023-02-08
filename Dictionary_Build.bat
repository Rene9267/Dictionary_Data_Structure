@echo off

if exist "Bin" (
    RD /S /Q "Bin"
)
MD "Bin"
clang -o Bin\dictionary.exe Dictionary_Data_Structure.c
.\Bin\dictionary.exe