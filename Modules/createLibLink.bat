@echo off

if %~1 == "" (
    echo "No directory specified"
) else (
    mklink /D "%1"\libJModule ..\..\JMADF\libJModule
    echo %~1
)

pause