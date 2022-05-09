@echo off

if %~1 == "" (
    echo "No directory specified"
) else (
    rd "%1"\libJModule
    echo %~1
)

pause